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
#define SUSTAIN_PEDAL_PIN DP17
#define MIN_TIME_MS       10 // Decrease to capture faster taps
#define MAX_TIME_MS       80 // Increase for greater dynamic range

// KEYBOARD MATRIX PIN DEFINITION
// SMALLER BOARD 32 KEYS
#define PIN_CA1  DP18
#define PIN_CA2  DP19
#define PIN_CA3  DP22
#define PIN_CA4  DP24
#define PIN_CA5  DP26
#define PIN_CA6  DP28
#define PIN_CA7  DP30
#define PIN_CA8  DP32
#define PIN_CA9  DP20
#define PIN_CA10 DP21
#define PIN_CA11 DP23
#define PIN_CA12 DP25
#define PIN_CA13 DP27
#define PIN_CA14 DP29
#define PIN_CA15 DP31
#define PIN_CA16 DP33

// LARGER BOARD 44 KEYS
#define PIN_CB17 DP34
#define PIN_CB18 DP36
#define PIN_CB19 DP38
#define PIN_CB20 DP40
#define PIN_CB21 DP42
#define PIN_CB22 DP44
#define PIN_CB23 DP46
#define PIN_CB24 DP48
#define PIN_CB25 DP50
#define PIN_CB26 DP52
#define PIN_CB27 DP35
#define PIN_CB28 DP37
#define PIN_CB29 DP39
#define PIN_CB30 DP41
#define PIN_CB31 DP43
#define PIN_CB32 DP45
#define PIN_CB33 DP47
#define PIN_CB34 DP49
#define PIN_CB35 DP51
#define PIN_CB36 DP53
