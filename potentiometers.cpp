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

#define POTS_RESOLUTION_MICROSECONDS  5000
#define POTS_THRESHOLD_VALUE          8 // 1024 divided by 128
#define POTS_PB_CENTER_DEADZONE       4
#define POTS_NUMBER                   2
#define POT_TYPE_PITCHBEND       0xE000
#define POT_TYPE_MODWHEEL        0xB001
#define POT_TYPE_VOLUME          0xB007
#define POT_TYPE_PAN             0xB00A
#define POT_TYPE_EXPRESSION      0xB00B
#define POT_TYPE_RESONANCE       0xB047
#define POT_TYPE_FILTER          0xB04A
#define POT_TYPE_REVERB          0xB05B
#define POT_TYPE_CHORUS          0xB05D

const int POTS_ANALOG_PINS[POTS_NUMBER] = {
    A0,
    A1
};
const int POTS_TYPES[POTS_NUMBER] = {
    POT_TYPE_PITCHBEND,
    POT_TYPE_MODWHEEL
};

/*
    5V  ────┬────────────
            │
        [ POT ] ← linear 10 kΩ
            │
            └────── A0
            │
    GND ────┴────────────

    TIPS:
    - Use linear potentiometers (B-taper) of 10 kΩ
    - Avoid logarithmic/audio (A-taper)
    - For extra precision, add 100 nF from A0 to GND
    - If this module is enabled and there is no potentiometers connected, connects the configured inputs to GND
*/

int analogRawValues[POTS_NUMBER] = {0}; // 10-bit ADC (0–1023)
int midiValues[POTS_NUMBER] = {0};
unsigned long lastReadingTime = 0;

void potentiometersSetup()
{
    for (int i = 0; i < POTS_NUMBER; i++)
    {
        if (POTS_TYPES[i] == POT_TYPE_PITCHBEND)
        {
            midiValues[i] = 8192;
        }
    }
}

void potentiometersLoop()
{
    unsigned long currentTime = micros();
    if (currentTime - lastReadingTime < POTS_RESOLUTION_MICROSECONDS)
    {
        return;
    }
    for (int i = 0; i < POTS_NUMBER; i++)
    {
        int raw = analogRead(POTS_ANALOG_PINS[i]);
        if (POTS_TYPES[i] == POT_TYPE_PITCHBEND)
        {
            const int CENTER = 512;
            int value;
            if (abs(raw - CENTER) <= POTS_PB_CENTER_DEADZONE)
            {
                value = 8192;
            }
            else if (raw < CENTER)
            {
                value = map(raw, 0, CENTER - POTS_PB_CENTER_DEADZONE - 1, 0, 8191);
            }
            else
            {
                value = map(raw, CENTER + POTS_PB_CENTER_DEADZONE + 1, 1023, 8192, 16383);
            }
            if (midiValues[i] == value)
            {
                continue;
            }
            byte status = (POTS_TYPES[i] & 0xFF00) >> 8;
            midiValues[i] = value;
            byte lsb = value & 0x7F;
            byte msb = value >> 7;
            sendMidiEvent(status, lsb, msb);
        }
        else
        {
            int lastRaw = analogRawValues[i];
            if (abs(raw - lastRaw) < POTS_THRESHOLD_VALUE)
            {
                continue;
            }
            analogRawValues[i] = raw;
            byte status = (POTS_TYPES[i] & 0xFF00) >> 8;
            byte cc     = POTS_TYPES[i] & 0x00FF;
            byte value  = raw >> 3; // scale to MIDI range (0–127)
            sendMidiEvent(status, cc, value);
        }
    }
    lastReadingTime = currentTime;
}
