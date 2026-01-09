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

#define DEFINE_MODEL_IO_PINS
#import "globals.h"

boolean matrix_signals[KEYS_NUMBER * 2];
byte sustain_pedal_signal;

void initIOPins()
{
    for (byte pin = 0; pin < (KEYS_NUMBER * 2); pin++)
    {
        pinMode(output_pins[pin], OUTPUT);
        pinMode(input_pins[pin], INPUT_PULLUP);
        matrix_signals[pin] = HIGH;
    }
    pinMode(SUSTAIN_PEDAL_PIN, INPUT_PULLUP);

}

void scanMatrix()
{
    boolean *s = matrix_signals;
    for (byte i = 0; i < KEYS_NUMBER * 2; i++)
    {
        byte output_pin = output_pins[i];
        byte input_pin = input_pins[i];
        digitalWrite2(output_pin, LOW);
        *(s++) = !digitalRead2(input_pin);
        digitalWrite2(output_pin, HIGH);
    }
    sustain_pedal_signal = digitalRead2(SUSTAIN_PEDAL_PIN);
}
