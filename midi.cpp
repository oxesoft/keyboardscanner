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

#define DEFINE_BLACK_KEYS_MAP
#include "globals.h"
#include "velocity.h"
#include <assert.h>
#define SIZEOF_VELOCITY_CURVE 128
static_assert(sizeof(VELOCITY_CURVE) == SIZEOF_VELOCITY_CURVE, "Invalid velocity curve");

bool isBlack(byte note) {
    switch (note % 12) {
        case 1:
        case 3:
        case 6:
        case 8:
        case 10:
            return true;
        default:
            return false;
    }
}

void sendKeyEvent(byte status_byte, byte key_index, unsigned long time)
{
    byte key = FIRST_KEY + key_index;
#ifdef BLACK_KEYS_VELOCITY_MULTIPLIER
    if (isBlack(key))
    {
        time = (time * BLACK_KEYS_VELOCITY_MULTIPLIER) >> 7;
    }
#endif
    unsigned long t = constrain(time, MIN_TIME_US, MAX_TIME_US);
    t -= MIN_TIME_US;

    unsigned long linear_velocity = 127 - ((t * 127) / (MAX_TIME_US - MIN_TIME_US));
    byte vel = VELOCITY_CURVE[linear_velocity];

#ifdef DEBUG_VELOCITY_TIMES
    Serial.print("KEY_");
    Serial.print(status_byte == 0x90 ? "ON " : "OFF");
    Serial.print(":");
    Serial.print(key_index);
    Serial.print(" TIME:");
    Serial.print(time);
    Serial.print(" RAW:");
    Serial.print(linear_velocity);
    Serial.print(" VEL:");
    Serial.println(vel);
#else
    sendMidiEvent(status_byte, key, vel);
#endif
}

void sendSustainPedalEvent(boolean pressed)
{
    sendMidiEvent(0xB0, 0x40, pressed ? 0x7F : 0x00);
}

void sendMidiEvent(byte status_byte, byte data1, byte data2)
{
#ifdef DEBUG_MIDI_MESSAGE
    Serial.print(status_byte, 16);
    Serial.print(" ");
    Serial.print(data1, 16);
    Serial.print(" ");
    Serial.println(data2, 16);
#else
    Serial.write(status_byte);
    Serial.write(data1);
    Serial.write(data2);
#endif
}
