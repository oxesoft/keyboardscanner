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

#define KEYS_NUMBER       61
#define FIRST_KEY         36
#define SUSTAIN_PEDAL_PIN DP17
#define MIN_TIME_MS       3   // Decrease to capture faster taps
#define MAX_TIME_MS       150 // Increase for greater dynamic range

// KEYBOARD MATRIX PIN DEFINITION
#define PIN_A1  DP22
#define PIN_A2  DP24
#define PIN_A3  DP26
#define PIN_A4  DP28
#define PIN_A5  DP30
#define PIN_A6  DP32
#define PIN_A7  DP34
#define PIN_A8  DP36
#define PIN_A9  DP38
#define PIN_A10 DP40
#define PIN_A11 DP42
#define PIN_A12 DP44

#define PIN_B1  DP23
#define PIN_B2  DP25
#define PIN_B3  DP27
#define PIN_B4  DP29
#define PIN_B5  DP31
#define PIN_B6  DP33
#define PIN_B7  DP35
#define PIN_B8  DP37
#define PIN_B9  DP39
#define PIN_B10 DP41
#define PIN_B11 DP43
