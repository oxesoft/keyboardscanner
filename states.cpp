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

boolean       matrix_signals[KEYS_NUMBER * 2] = {HIGH};
byte          keys_state    [KEYS_NUMBER]     = {KEY_OFF};
unsigned long keys_time     [KEYS_NUMBER]     = {0};
byte          sustain_pedal_signal;

void updateStates()
{
    byte          *state  = keys_state;
    unsigned long *ktime  = keys_time;
    boolean       *signal = matrix_signals;
    for (byte key = 0; key < KEYS_NUMBER; key++)
    {
        for (byte state_index = 0; state_index < 2; state_index++)
        {
            switch (*state)
            {
            case KEY_OFF:
                if (state_index == 0 && *signal)
                {
                    *state = KEY_START;
                    *ktime = millis();
                }
                break;
            case KEY_START:
                if (state_index == 0 && !*signal)
                {
                    *state = KEY_OFF;
                    break;
                }
                if (state_index == 1 && *signal)
                {
                    *state = KEY_ON;
                    sendKeyEvent(0x90, key, millis() - *ktime);
                }
                break;
            case KEY_ON:
                if (state_index == 1 && !*signal)
                {
                    *state = KEY_RELEASED;
                    *ktime = millis();
                }
                break;
            case KEY_RELEASED:
                if (state_index == 0 && !*signal)
                {
                    if (sustain_pedal_signal)
                    {
                        *state = KEY_SUSTAINED;
                        break;
                    }
                    *state = KEY_OFF;
                    sendKeyEvent(0x80, key, millis() - *ktime);
                }
                break;
            case KEY_SUSTAINED:
                if (!sustain_pedal_signal)
                {
                    *state = KEY_OFF;
                    sendKeyEvent(0x80, key, MAX_TIME_MS);
                }
                if (state_index == 0 && *signal)
                {
                    *state = KEY_SUSTAINED_RESTART;
                    *ktime = millis();
                }
                break;
            case KEY_SUSTAINED_RESTART:
                if (state_index == 0 && !*signal)
                {
                    *state = KEY_SUSTAINED;
                    break;
                }
                if (state_index == 1 && *signal)
                {
                    *state = KEY_ON;
                    sendKeyEvent(0x80, key, MAX_TIME_MS);
                    sendKeyEvent(0x90, key, millis() - *ktime);
                }
                break;
            }
            signal++;
        }
        state++;
        ktime++;
    }
}
