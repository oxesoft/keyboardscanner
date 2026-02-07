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

#import "globals.h"
#define MODEL_PINS_DEF models/MODEL_NAME/pins.h
extern byte sustain_pedal_signal;
#include <DIO2.h> // install the library DIO2

#ifdef SETTLING_TIME_MICROSECONDS
#define SETTLING_TIME_DELAY delayMicroseconds(SETTLING_TIME_MICROSECONDS);
#else
#define SETTLING_TIME_DELAY
#endif

void initIOPins()
{
    pinMode2(LED_BUILTIN, OUTPUT);
    digitalWrite2(LED_BUILTIN, LOW);
    #define PINS(output_pin, input_pin) \
    pinMode2(output_pin, OUTPUT); \
    digitalWrite2(output_pin, HIGH); \
    pinMode2(input_pin, INPUT_PULLUP);
    #include STR(MODEL_PINS_DEF)
    #undef PINS
    pinMode2(SUSTAIN_PEDAL_PIN, INPUT_PULLUP);
}

#ifdef DIRECT_PORTS_READING

extern byte curr_mask[KEYS_NUMBER >> 2];

void scanMatrix()
{
    // the code below was tested using a maudio_keystation88ii
    byte tmp;

    digitalWrite2(OUT_A_00, LOW);
    SETTLING_TIME_DELAY
    curr_mask[0] = ~PINC;
    digitalWrite2(OUT_A_00, HIGH);
    digitalWrite2(OUT_A_01, LOW);
    SETTLING_TIME_DELAY
    curr_mask[1] = ~PINC;
    digitalWrite2(OUT_A_01, HIGH);

    digitalWrite2(OUT_A_02, LOW);
    SETTLING_TIME_DELAY
    curr_mask[2] = ~PINC;
    digitalWrite2(OUT_A_02, HIGH);
    digitalWrite2(OUT_A_03, LOW);
    SETTLING_TIME_DELAY
    curr_mask[3] = ~PINC;
    digitalWrite2(OUT_A_03, HIGH);

    digitalWrite2(OUT_A_04, LOW);
    SETTLING_TIME_DELAY
    curr_mask[4] = ~PINC;
    digitalWrite2(OUT_A_04, HIGH);
    digitalWrite2(OUT_A_05, LOW);
    SETTLING_TIME_DELAY
    curr_mask[5] = ~PINC;
    digitalWrite2(OUT_A_05, HIGH);

    digitalWrite2(OUT_A_06, LOW);
    SETTLING_TIME_DELAY
    curr_mask[6] = ~PINC;
    digitalWrite2(OUT_A_06, HIGH);
    digitalWrite2(OUT_A_07, LOW);
    SETTLING_TIME_DELAY
    curr_mask[7] = ~PINC;
    digitalWrite2(OUT_A_07, HIGH);

    digitalWrite2(OUT_A_08, LOW);
    SETTLING_TIME_DELAY
    curr_mask[8] = ~PINC;
    digitalWrite2(OUT_A_08, HIGH);
    digitalWrite2(OUT_A_09, LOW);
    SETTLING_TIME_DELAY
    curr_mask[9] = ~PINC;
    digitalWrite2(OUT_A_09, HIGH);

    digitalWrite2(OUT_B_08, LOW);
    SETTLING_TIME_DELAY
    tmp = ~PINK;
    curr_mask[8] |= tmp & 1 ? 0b10000000 : 0;
    digitalWrite2(OUT_B_08, HIGH);
    digitalWrite2(OUT_B_09, LOW);
    SETTLING_TIME_DELAY
    tmp = ~PINK;
    curr_mask[9] |= tmp & 1 ? 0b10000000 : 0;
    digitalWrite2(OUT_B_09, HIGH);

    digitalWrite2(OUT_B_00, LOW);
    SETTLING_TIME_DELAY
    curr_mask[10] = ~PINF;
    digitalWrite2(OUT_B_00, HIGH);
    digitalWrite2(OUT_B_01, LOW);
    SETTLING_TIME_DELAY
    curr_mask[11] = ~PINF;
    digitalWrite2(OUT_B_01, HIGH);

    digitalWrite2(OUT_B_02, LOW);
    SETTLING_TIME_DELAY
    curr_mask[12] = ~PINF;
    digitalWrite2(OUT_B_02, HIGH);
    digitalWrite2(OUT_B_03, LOW);
    SETTLING_TIME_DELAY
    curr_mask[13] = ~PINF;
    digitalWrite2(OUT_B_03, HIGH);

    digitalWrite2(OUT_B_04, LOW);
    SETTLING_TIME_DELAY
    curr_mask[14] = ~PINF;
    digitalWrite2(OUT_B_04, HIGH);
    digitalWrite2(OUT_B_05, LOW);
    SETTLING_TIME_DELAY
    curr_mask[15] = ~PINF;
    digitalWrite2(OUT_B_05, HIGH);

    digitalWrite2(OUT_B_06, LOW);
    SETTLING_TIME_DELAY
    curr_mask[16] = ~PINF;
    digitalWrite2(OUT_B_06, HIGH);
    digitalWrite2(OUT_B_07, LOW);
    SETTLING_TIME_DELAY
    curr_mask[17] = ~PINF;
    digitalWrite2(OUT_B_07, HIGH);

    digitalWrite2(OUT_B_08, LOW);
    SETTLING_TIME_DELAY
    curr_mask[18] = ~PINF;
    digitalWrite2(OUT_B_08, HIGH);
    digitalWrite2(OUT_B_09, LOW);
    SETTLING_TIME_DELAY
    curr_mask[19] = ~PINF;
    digitalWrite2(OUT_B_09, HIGH);

    digitalWrite2(OUT_B_10, LOW);
    SETTLING_TIME_DELAY
    curr_mask[20] = ~PINF;
    digitalWrite2(OUT_B_10, HIGH);
    digitalWrite2(OUT_B_11, LOW);
    SETTLING_TIME_DELAY
    curr_mask[21] = ~PINF;
    digitalWrite2(OUT_B_11, HIGH);

    sustain_pedal_signal = digitalRead2(SUSTAIN_PEDAL_PIN);
}

#else

extern boolean matrix_signals[KEYS_NUMBER * 2];

void scanMatrix()
{
    boolean *s = matrix_signals;
    #define PINS(output_pin, input_pin) \
    digitalWrite2(output_pin, LOW); \
    SETTLING_TIME_DELAY \
    *(s++) = !digitalRead2(input_pin); \
    digitalWrite2(output_pin, HIGH);
    #include STR(MODEL_PINS_DEF)
    #undef PINS
    sustain_pedal_signal = digitalRead2(SUSTAIN_PEDAL_PIN);
}

#endif