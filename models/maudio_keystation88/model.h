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

#define KEYS_NUMBER       88
#define FIRST_KEY         21
#define SUSTAIN_PEDAL_PIN 14
#define MIN_TIME_MS       3  // Decrease to capture faster taps
#define MAX_TIME_MS       83 // Increase for greater dynamic range

// Board A
#define IN_A_00 39
#define IN_A_01 41
#define IN_A_02 43
#define IN_A_03 45
#define IN_A_04 47
#define IN_A_05 49
#define IN_A_06 51
#define IN_A_07 53

#define OUT_A_00 37
#define OUT_A_01 35
#define OUT_A_02 33
#define OUT_A_03 31
#define OUT_A_04 29
#define OUT_A_05 27
#define OUT_A_06 25
#define OUT_A_07 23
#define OUT_A_08 22
#define OUT_A_09 24

// Board B
#define IN_B_00 52
#define IN_B_01 50
#define IN_B_02 48
#define IN_B_03 46
#define IN_B_04 44
#define IN_B_05 42
#define IN_B_06 40
#define IN_B_07 38
#define IN_B_08 36

#define OUT_B_00 34
#define OUT_B_01 32
#define OUT_B_02 30
#define OUT_B_03 28
#define OUT_B_04 26
#define OUT_B_05 21
#define OUT_B_06 20
#define OUT_B_07 19
#define OUT_B_08 18
#define OUT_B_09 17
#define OUT_B_10 16
#define OUT_B_11 15
