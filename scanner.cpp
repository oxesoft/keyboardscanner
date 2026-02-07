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
    byte *mask = curr_mask;
    // the code below was tested using a maudio_keystation88ii
    for (byte row = 0; row < 8; row++)
    {
        PORTA = ~(1 << row);
        SETTLING_TIME_DELAY
        *(mask++) = ~PINC;
        PORTA = 0xFF;
    }
    for (byte row = 0; row < 2; row++)
    {
        PORTD = ~(1 << row);
        SETTLING_TIME_DELAY
        *(mask++) = ~PINC;
        PORTD = 0xFF;
    }
    for (byte row = 0; row < 2; row++)
    {
        PORTB = ~(1 << row);
        SETTLING_TIME_DELAY
        byte tmp = ~PINK;
        curr_mask[row + 8] |= tmp & 1 ? 0b10000000 : 0;
        PORTB = 0xFF;
    }
    for (byte row = 0; row < 8; row++)
    {
        PORTL = ~(1 << row);
        SETTLING_TIME_DELAY
        *(mask++) = ~PINF;
        PORTL = 0xFF;
    }
    for (byte row = 0; row < 4; row++)
    {
        PORTB = ~(1 << row);
        SETTLING_TIME_DELAY
        *(mask++) = ~PINF;
        PORTB = 0xFF;
    }
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