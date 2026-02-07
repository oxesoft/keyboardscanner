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

#define KEYS_NUMBER                    88
#define FIRST_KEY                      21
#define SUSTAIN_PEDAL_PIN              14
#define MIN_TIME_US                    2500
#define MAX_TIME_US                    15000
#define BLACK_KEYS_VELOCITY_MULTIPLIER 110 // 127 is the central value (corresponding to 1.0)

// Board A
#define IN_A_00 29
#define IN_A_01 28
#define IN_A_02 27
#define IN_A_03 26
#define IN_A_04 25
#define IN_A_05 24
#define IN_A_06 23
#define IN_A_07 22

#define OUT_A_00 30
#define OUT_A_01 31
#define OUT_A_02 32
#define OUT_A_03 33
#define OUT_A_04 34
#define OUT_A_05 35
#define OUT_A_06 36
#define OUT_A_07 37
#define OUT_A_08 40
#define OUT_A_09 41

// Board B
#define IN_B_00 A15
#define IN_B_01 A14
#define IN_B_02 A13
#define IN_B_03 A12
#define IN_B_04 A11
#define IN_B_05 A10
#define IN_B_06 A9
#define IN_B_07 A8
#define IN_B_08 A7

#define OUT_B_00 42
#define OUT_B_01 43
#define OUT_B_02 44
#define OUT_B_03 45
#define OUT_B_04 46
#define OUT_B_05 47
#define OUT_B_06 48
#define OUT_B_07 49
#define OUT_B_08 50
#define OUT_B_09 51
#define OUT_B_10 52
#define OUT_B_11 53
