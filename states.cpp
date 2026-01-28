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

#define KEY_OFF                  0
#define KEY_START                1
#define KEY_ON                   2
#define KEY_RELEASED             3

boolean       matrix_signals[KEYS_NUMBER * 2] = {LOW};
byte          keys_state    [KEYS_NUMBER]     = {KEY_OFF};
unsigned long keys_time     [KEYS_NUMBER]     = {0};
byte          sustain_pedal_signal;
byte          sustain_pedal_signal_previous = HIGH;

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
                if (state_index == 0 && *signal == HIGH)
                {
                    *state = KEY_START;
                    *ktime = micros();
                }
                break;
            case KEY_START:
                if (state_index == 0 && *signal == LOW)
                {
                    *state = KEY_OFF;
                    break;
                }
                if (state_index == 1 && *signal == HIGH)
                {
                    *state = KEY_ON;
                    sendKeyEvent(0x90, key, micros() - *ktime);
                }
                break;
            case KEY_ON:
                if (state_index == 1 && *signal == LOW)
                {
                    *state = KEY_RELEASED;
                    *ktime = micros();
                }
                break;
            case KEY_RELEASED:
                if (state_index == 0 && *signal == LOW)
                {
                    *state = KEY_OFF;
                    sendKeyEvent(0x80, key, micros() - *ktime);
                }
                break;
            }
            signal++;
        }
        state++;
        ktime++;
    }
    if (sustain_pedal_signal_previous != sustain_pedal_signal)
    {
        sendSustainPedalEvent(sustain_pedal_signal == LOW);
    }
    sustain_pedal_signal_previous = sustain_pedal_signal;
}
