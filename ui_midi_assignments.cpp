/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

This code is originally hosted at https://github.com/oxesoft/keyboardscanner

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "globals.h"
#include <EEPROM.h>
#include "ui.h"

#ifdef ENABLE_MIDI_ASSIGNMENTS_SUPPORT

#ifndef POTS_ASSIGN_RESOLUTION_MICROSECONDS
#define POTS_ASSIGN_RESOLUTION_MICROSECONDS 5000
#endif

#ifndef POTS_ASSIGN_THRESHOLD_VALUE
#define POTS_ASSIGN_THRESHOLD_VALUE 8
#endif

// EEPROM layout
static constexpr uint16_t EEPROM_MAGIC_ADDR = 0;
static constexpr uint16_t EEPROM_DATA_ADDR = 8;
static constexpr uint32_t EEPROM_MAGIC = 0x4B534D31; // 'KSM1'
static constexpr uint8_t EEPROM_VER = 2;             // bump when change layout

static uint8_t clampU8(int v, int lo, int hi)
{
    if (v < lo)
        return (uint8_t)lo;
    if (v > hi)
        return (uint8_t)hi;
    return (uint8_t)v;
}

// Reserved sizes in EEPROM (0 if feature not enabled)
#if defined(ENABLE_MIDI_ASSIGN_BUTTONS)
static constexpr int EEPROM_BYTES_BUTTONS =
    (int)(BUTTONS_ASSIGN_NUMBER * (int)sizeof(MidiButtonMapping));
#else
static constexpr int EEPROM_BYTES_BUTTONS = 0;
#endif

#if defined(ENABLE_MIDI_ASSIGN_POTS)
static constexpr int EEPROM_BYTES_POTS =
    (int)(POTS_ASSIGN_NUMBER * (int)sizeof(MidiPotMapping));
#else
static constexpr int EEPROM_BYTES_POTS = 0;
#endif

//  State: Buttons
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
static MidiButtonMapping g_btnMap[BUTTONS_ASSIGN_NUMBER];
static bool g_toggleState[BUTTONS_ASSIGN_NUMBER];
static bool g_lastPressedBtn[BUTTONS_ASSIGN_NUMBER];
#endif

//  State: Pots
#ifdef ENABLE_MIDI_ASSIGN_POTS
static MidiPotMapping g_potMap[POTS_ASSIGN_NUMBER];
static int g_lastRawPot[POTS_ASSIGN_NUMBER];
static uint8_t g_lastMidiPot[POTS_ASSIGN_NUMBER];
static unsigned long g_lastPotReadUs = 0;
#endif

//  EEPROM helpers
static void saveToEeprom()
{
    int addr = EEPROM_DATA_ADDR;

// Buttons block
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    for (int i = 0; i < BUTTONS_ASSIGN_NUMBER; i++) {
        EEPROM.put(addr, g_btnMap[i]);
        addr += (int)sizeof(MidiButtonMapping);
    }
#else
    addr += EEPROM_BYTES_BUTTONS; // reserves space even without a feature.
#endif

    // Pots block
#ifdef ENABLE_MIDI_ASSIGN_POTS
    for (int i = 0; i < POTS_ASSIGN_NUMBER; i++) {
        EEPROM.put(addr, g_potMap[i]);
        addr += (int)sizeof(MidiPotMapping);
    }
#else
    addr += EEPROM_BYTES_POTS; // reserves space even without a feature.
#endif
}

static void loadFromEeprom()
{
    uint32_t magic = 0;
    EEPROM.get(EEPROM_MAGIC_ADDR, magic);
    uint8_t ver = EEPROM.read(EEPROM_MAGIC_ADDR + 4);

    if (magic != EEPROM_MAGIC || ver != EEPROM_VER) {
        EEPROM.put(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);
        EEPROM.write(EEPROM_MAGIC_ADDR + 4, EEPROM_VER);
        //  5..7 bytes reserved
        saveToEeprom();
        return;
    }

    int addr = EEPROM_DATA_ADDR;

// Buttons block
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    for (int i = 0; i < BUTTONS_ASSIGN_NUMBER; i++) {
        MidiButtonMapping m;
        EEPROM.get(addr, m);
        addr += (int)sizeof(MidiButtonMapping);

        m.channel = clampU8(m.channel, 1, 16);
        m.cc = clampU8(m.cc, 0, 127);
        if (m.mode > MIDI_BTN_MODE_PUSH)
            m.mode = MIDI_BTN_MODE_TOGGLE;

        g_btnMap[i] = m;
    }
#else
    addr += EEPROM_BYTES_BUTTONS;
#endif

// Pots block
#ifdef ENABLE_MIDI_ASSIGN_POTS
    for (int i = 0; i < POTS_ASSIGN_NUMBER; i++) {
        MidiPotMapping p;
        EEPROM.get(addr, p);
        addr += (int)sizeof(MidiPotMapping);

        p.channel = clampU8(p.channel, 1, 16);
        p.cc = clampU8(p.cc, 0, 127);
        p.min = clampU8(p.min, 0, 127);
        p.max = clampU8(p.max, 0, 127);
        if (p.min > p.max) {
            uint8_t tmp = p.min;
            p.min = p.max;
            p.max = tmp;
        }

        g_potMap[i] = p;
    }
#else
    addr += EEPROM_BYTES_POTS;
#endif
}

// Button logic
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
static void handleButton(uint8_t idx)
{
    bool pressed = (digitalRead(MIDI_BUTTONS_PINS[idx]) == LOW);

    bool wasPressed = g_lastPressedBtn[idx];
    g_lastPressedBtn[idx] = pressed;

    uint8_t ch = clampU8(g_btnMap[idx].channel, 1, 16);
    uint8_t status = 0xB0 | ((ch - 1) & 0x0F);

    if (g_btnMap[idx].mode == MIDI_BTN_MODE_TOGGLE) {
        if (pressed && !wasPressed) {
            g_toggleState[idx] = !g_toggleState[idx];
            uint8_t value = g_toggleState[idx] ? 127 : 0;
            sendMidiEvent(status, g_btnMap[idx].cc, value);
        }
    } else {
        if (pressed != wasPressed) {
            uint8_t value = pressed ? 127 : 0;
            sendMidiEvent(status, g_btnMap[idx].cc, value);
        }
    }
}
#endif

// Pot logic
#ifdef ENABLE_MIDI_ASSIGN_POTS
static void readPots()
{
    unsigned long nowUs = micros();
    if (nowUs - g_lastPotReadUs <
        (unsigned long)POTS_ASSIGN_RESOLUTION_MICROSECONDS) {
        return;
    }

    for (int i = 0; i < POTS_ASSIGN_NUMBER; i++) {
        int raw = analogRead(MIDI_POTS_ANALOG_PINS[i]);

        int lastRaw = g_lastRawPot[i];
        if (abs(raw - lastRaw) < POTS_ASSIGN_THRESHOLD_VALUE) {
            continue;
        }
        g_lastRawPot[i] = raw;

        uint8_t base = (uint8_t)(raw >> 3);

        MidiPotMapping p = g_potMap[i];
        uint8_t out;
        if (p.min == 0 && p.max == 127) {
            out = base;
        } else {
            out = (uint8_t)map((int)base, 0, 127, (int)p.min, (int)p.max);
        }

        if (out == g_lastMidiPot[i])
            continue;
        g_lastMidiPot[i] = out;

        uint8_t ch = clampU8(p.channel, 1, 16);
        uint8_t status = 0xB0 | ((ch - 1) & 0x0F);
        sendMidiEvent(status, p.cc, out);
    }

    g_lastPotReadUs = nowUs;
}
#endif

void initMidiAssignments()
{
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    for (int i = 0; i < BUTTONS_ASSIGN_NUMBER; i++) {
        pinMode(MIDI_BUTTONS_PINS[i], INPUT_PULLUP);
        g_btnMap[i].channel = 1;
        g_btnMap[i].cc = (uint8_t)(20 + i); // CC = 20 + index
        g_btnMap[i].mode = MIDI_BTN_MODE_TOGGLE;

        g_toggleState[i] = false;
        g_lastPressedBtn[i] = false;
    }
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    for (int i = 0; i < POTS_ASSIGN_NUMBER; i++) {
        g_potMap[i].channel = 1;
        g_potMap[i].cc = (uint8_t)(30 + i); // CC = 30 + index
        g_potMap[i].min = 0;
        g_potMap[i].max = 127;

        g_lastRawPot[i] = analogRead(MIDI_POTS_ANALOG_PINS[i]);
        g_lastMidiPot[i] = 255;
    }
    g_lastPotReadUs = micros();
#endif

    loadFromEeprom();
}

void readMidiAssignments()
{
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    for (uint8_t i = 0; i < BUTTONS_ASSIGN_NUMBER; i++) {
        handleButton(i);
    }
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    readPots();
#endif
}

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
MidiButtonMapping getMidiAssignButtonMapping(uint8_t idx)
{
    if (idx >= BUTTONS_ASSIGN_NUMBER)
        idx = BUTTONS_ASSIGN_NUMBER - 1;
    return g_btnMap[idx];
}

void setMidiAssignButtonMapping(uint8_t idx, const MidiButtonMapping &m)
{
    if (idx >= BUTTONS_ASSIGN_NUMBER)
        return;

    g_btnMap[idx].channel = clampU8(m.channel, 1, 16);
    g_btnMap[idx].cc = clampU8(m.cc, 0, 127);

    uint8_t mode = m.mode;
    if (mode > MIDI_BTN_MODE_PUSH)
        mode = MIDI_BTN_MODE_TOGGLE;
    g_btnMap[idx].mode = mode;

    saveToEeprom();
}
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
MidiPotMapping getMidiAssignPotMapping(uint8_t idx)
{
    if (idx >= POTS_ASSIGN_NUMBER)
        idx = POTS_ASSIGN_NUMBER - 1;
    return g_potMap[idx];
}

void setMidiAssignPotMapping(uint8_t idx, const MidiPotMapping &m)
{
    if (idx >= POTS_ASSIGN_NUMBER)
        return;

    MidiPotMapping p = m;
    p.channel = clampU8(p.channel, 1, 16);
    p.cc = clampU8(p.cc, 0, 127);
    p.min = clampU8(p.min, 0, 127);
    p.max = clampU8(p.max, 0, 127);
    if (p.min > p.max) {
        uint8_t tmp = p.min;
        p.min = p.max;
        p.max = tmp;
    }

    g_potMap[idx] = p;
    saveToEeprom();
}
#endif

#endif
