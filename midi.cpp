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

#define MAX_TIME_MS_N (MAX_TIME_MS - MIN_TIME_MS)

#ifdef VELOCITY_CURVE
#define SIZEOF_VELOCITY_CURVE 81
#define MAX_VEL_CURVE_INDEX SIZEOF_VELOCITY_CURVE - 1
// inverted curves
const byte linearCurve   [SIZEOF_VELOCITY_CURVE] = {127, 127, 127, 125, 124, 122, 121, 119, 118, 116, 114, 113, 111, 110, 108, 107, 105, 103, 102, 100, 99, 97, 95, 94, 92, 91, 89, 88, 86, 84, 83, 81, 80, 78, 77, 75, 73, 72, 70, 69, 67, 66, 64, 62, 61, 59, 58, 56, 54, 53, 51, 50, 48, 47, 45, 43, 42, 40, 39, 37, 36, 34, 32, 31, 29, 28, 26, 25, 23, 21, 20, 18, 17, 15, 13, 12, 10, 9, 7, 6, 4};
const byte convexCurve   [SIZEOF_VELOCITY_CURVE] = {127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 125, 124, 123, 123, 122, 121, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 109, 108, 107, 105, 104, 102, 101, 99, 98, 96, 94, 93, 91, 89, 87, 86, 84, 82, 80, 78, 76, 74, 72, 70, 68, 66, 63, 61, 59, 57, 54, 52, 50, 48, 45, 43, 41, 38, 36, 33, 31, 29, 26, 24, 21, 19, 16, 14, 11, 9, 6, 4};
const byte saturatedCurve[SIZEOF_VELOCITY_CURVE] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 126, 125, 125, 124, 122, 121, 119, 118, 116, 114, 111, 109, 106, 104, 101, 98, 94, 91, 87, 84, 80, 76, 72, 68, 64, 60, 55, 51, 47, 42, 37, 33, 28, 23, 18, 14, 9, 4};
const byte concaveCurve  [SIZEOF_VELOCITY_CURVE] = {127, 127, 127, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 115, 113, 111, 109, 107, 105, 103, 101, 99, 97, 95, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 63, 61, 59, 57, 55, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 1, 1};

const byte *velocityCurves[] = {
    linearCurve,
    convexCurve,
    saturatedCurve,
    concaveCurve
};
#endif

#ifdef DEBUG_VELOCITY
void debugVelocityTimes(byte key_index, unsigned long time)
{
    unsigned long t = constrain(time, MIN_TIME_MS, MAX_TIME_MS);
    t -= MIN_TIME_MS;

    byte velocity = constrain(
        127 - ((t * 127) / MAX_TIME_MS_N),
        0,
        127
    );

    char debug_msg[32];
    sprintf(
        debug_msg,
        "KEY:%02d TIME:%03lu VEL:%03d",
        key_index,
        time,
        velocity
    );
    Serial.println(debug_msg);
}
#endif

void sendKeyEvent(byte status_byte, byte key_index, unsigned long time)
{
#ifdef DEBUG_VELOCITY
    debugVelocityTimes(key_index, time);
#else
    unsigned long t = constrain(time, MIN_TIME_MS, MAX_TIME_MS);
    t -= MIN_TIME_MS;

#ifdef BLACK_KEYS_CORRECTION
    if (black_keys[key_index])
    {
        t = (t * BLACK_KEYS_MULTIPLIER) >> 7;
    }
#endif
    unsigned long velocity = 127 - ((t * 127) / MAX_TIME_MS_N);
#ifdef VELOCITY_CURVE
    byte vel = velocityCurves[VELOCITY_CURVE][MAX_VEL_CURVE_INDEX - min(MAX_VEL_CURVE_INDEX, velocity)];
#else
    // default cubic response curve
    byte vel = (((velocity * velocity) >> 7) * velocity) >> 7;
#endif
    byte key = FIRST_KEY + key_index;

#ifdef DEBUG_MIDI_MESSAGE
    char out[32];
    sprintf(out, "%02X %02X %03d %d", status_byte, key, vel, time);
    Serial.println(out);
#else
    sendMidiEvent(status_byte, key, vel);
#endif
#endif // DEBUG_VELOCITY
}

void sendMidiEvent(byte status_byte, byte data1, byte data2)
{
#ifdef DEBUG_MIDI_MESSAGE
    char out[32];
    sprintf(out, "%02X %02X %02X", status_byte, data1, data2);
    Serial.println(out);
#else
    Serial.write(status_byte);
    Serial.write(data1);
    Serial.write(data2);
#endif
}
