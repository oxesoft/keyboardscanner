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

#define KEYS_NUMBER       76
#define FIRST_KEY         28
#define SUSTAIN_PEDAL_PIN 17
#define MIN_TIME_US       3000
#define MAX_TIME_US       25000

// KEYBOARD MATRIX PIN DEFINITION
// SMALLER BOARD 32 KEYS
#define PIN_CA1  18
#define PIN_CA2  19
#define PIN_CA3  22
#define PIN_CA4  24
#define PIN_CA5  26
#define PIN_CA6  28
#define PIN_CA7  30
#define PIN_CA8  32
#define PIN_CA9  20
#define PIN_CA10 21
#define PIN_CA11 23
#define PIN_CA12 25
#define PIN_CA13 27
#define PIN_CA14 29
#define PIN_CA15 31
#define PIN_CA16 33

// LARGER BOARD 44 KEYS
#define PIN_CB17 34
#define PIN_CB18 36
#define PIN_CB19 38
#define PIN_CB20 40
#define PIN_CB21 42
#define PIN_CB22 44
#define PIN_CB23 46
#define PIN_CB24 48
#define PIN_CB25 50
#define PIN_CB26 52
#define PIN_CB27 35
#define PIN_CB28 37
#define PIN_CB29 39
#define PIN_CB30 41
#define PIN_CB31 43
#define PIN_CB32 45
#define PIN_CB33 47
#define PIN_CB34 49
#define PIN_CB35 51
#define PIN_CB36 53
