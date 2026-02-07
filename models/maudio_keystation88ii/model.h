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
#define BLACK_KEYS_VELOCITY_MULTIPLIER 140 // 127 is the central value (corresponding to 1.0)
#define SETTLING_TIME_MICROSECONDS     4   // Increase it to prevent false readings due to matrix capacitance (something between 2 and 5 should be enough)

// Board A
#define IN_A_00 37 // PC0
#define IN_A_01 36 // PC1
#define IN_A_02 35 // PC2
#define IN_A_03 34 // PC3
#define IN_A_04 33 // PC4
#define IN_A_05 32 // PC5
#define IN_A_06 31 // PC6
#define IN_A_07 30 // PC7

#define OUT_A_00 22 // PA0
#define OUT_A_01 23 // PA1
#define OUT_A_02 24 // PA2
#define OUT_A_03 25 // PA3
#define OUT_A_04 26 // PA4
#define OUT_A_05 27 // PA5
#define OUT_A_06 28 // PA6
#define OUT_A_07 29 // PA7
#define OUT_A_08 21 // PD0
#define OUT_A_09 20 // PD1

// Board B
#define IN_B_00 A0 // PF0
#define IN_B_01 A1 // PF1
#define IN_B_02 A2 // PF2
#define IN_B_03 A3 // PF3
#define IN_B_04 A4 // PF4
#define IN_B_05 A5 // PF5
#define IN_B_06 A6 // PF6
#define IN_B_07 A7 // PF7
#define IN_B_08 A8 // PK0

#define OUT_B_00 49 // PL0
#define OUT_B_01 48 // PL1
#define OUT_B_02 47 // PL2
#define OUT_B_03 46 // PL3
#define OUT_B_04 45 // PL4
#define OUT_B_05 44 // PL5
#define OUT_B_06 43 // PL6
#define OUT_B_07 42 // PL7
#define OUT_B_08 53 // PB0
#define OUT_B_09 52 // PB1
#define OUT_B_10 51 // PB2
#define OUT_B_11 50 // PB3
