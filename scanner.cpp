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
extern boolean matrix_signals[KEYS_NUMBER * 2];
extern byte    sustain_pedal_signal;

void initIOPins()
{
    #define PINS(output_pin, input_pin) \
    pinMode2f(output_pin, OUTPUT);      \
    pinMode2f(input_pin, INPUT_PULLUP);
    #include STR(MODEL_PINS_DEF)
    #undef PINS
    pinMode2f(SUSTAIN_PEDAL_PIN, INPUT_PULLUP);
}

void scanMatrix()
{
    boolean *s = matrix_signals;
    #define PINS(output_pin, input_pin) \
    digitalWrite2f(output_pin, LOW);    \
    *(s++) = !digitalRead2f(input_pin); \
    digitalWrite2f(output_pin, HIGH);
    #include STR(MODEL_PINS_DEF)
    #undef PINS
    sustain_pedal_signal = digitalRead2f(SUSTAIN_PEDAL_PIN);
}
