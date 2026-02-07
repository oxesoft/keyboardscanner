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

#define KEY_OFF      0
#define KEY_START    1
#define KEY_ON       2
#define KEY_RELEASED 3

byte          keys_state[KEYS_NUMBER] = {KEY_OFF};
unsigned long keys_time [KEYS_NUMBER] = {0};
byte          sustain_pedal_signal;
byte          sustain_pedal_signal_previous = HIGH;

void handleKey(byte key, boolean upper, boolean lower)
{
    switch (keys_state[key])
    {
    case KEY_OFF:
        if (upper)
        {
            keys_state[key] = KEY_START;
            keys_time [key] = micros();
        }
        break;
    case KEY_START:
        if (!upper)
        {
            keys_state[key] = KEY_OFF;
            break;
        }
        if (lower)
        {
            keys_state[key] = KEY_ON;
            unsigned long time = micros() - keys_time[key];
            sendKeyEvent(0x90, key, time);
        }
        break;
    case KEY_ON:
        if (!lower)
        {
            keys_state[key] = KEY_RELEASED;
            keys_time [key] = micros();
        }
        break;
    case KEY_RELEASED:
        if (!upper)
        {
            keys_state[key] = KEY_OFF;
            unsigned long time = micros() - keys_time[key];
            sendKeyEvent(0x80, key, time);
        }
        break;
    }
}

#ifdef DIRECT_PORTS_READING

#define NUM_GROUPS (KEYS_NUMBER >> 3)
byte prev_mask[KEYS_NUMBER >> 2];
byte curr_mask[KEYS_NUMBER >> 2];

void updateStates()
{
    for (uint8_t g = 0; g < NUM_GROUPS; g++) {

        byte up_curr   = curr_mask[2*g + 0];
        byte dn_curr   = curr_mask[2*g + 1];
        byte up_prev   = prev_mask[2*g + 0];
        byte dn_prev   = prev_mask[2*g + 1];

        byte up_changed = up_curr ^ up_prev;
        byte dn_changed = dn_curr ^ dn_prev;

        if ((up_changed | dn_changed) == 0)
            continue;   // nothing changed in this group

        byte changed = up_changed | dn_changed;

        while (changed) {

            uint8_t bit = changed & -changed;   // isolate lowest set bit
            changed ^= bit;                     // clear it

            byte key_in_group = __builtin_ctz(bit);  // 0..7
            byte key = g * 8 + key_in_group;

            bool up = up_curr & bit;
            bool dn = dn_curr & bit;

            handleKey(key, up, dn);
        }
        prev_mask[2*g + 0] = curr_mask[2*g + 0];
        prev_mask[2*g + 1] = curr_mask[2*g + 1];
    }
    if (sustain_pedal_signal_previous != sustain_pedal_signal)
    {
        sendSustainPedalEvent(sustain_pedal_signal == LOW);
    }
    sustain_pedal_signal_previous = sustain_pedal_signal;
}

#else

boolean matrix_signals[KEYS_NUMBER * 2] = {LOW};

void updateStates()
{
    boolean *signal = matrix_signals;
    for (byte key = 0; key < KEYS_NUMBER; key++)
    {
        boolean upper = *(signal++);
        boolean lower = *(signal++);
        handleKey(key, upper, lower);
    }
    if (sustain_pedal_signal_previous != sustain_pedal_signal)
    {
        sendSustainPedalEvent(sustain_pedal_signal == LOW);
    }
    sustain_pedal_signal_previous = sustain_pedal_signal;
}

#endif
