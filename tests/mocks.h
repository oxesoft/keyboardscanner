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

#define RUBBER_KEY_RELEASED 1
#define RUBBER_KEY_PRESSED  2

void advanceMockMicros(unsigned long ms);
void setRubberKey(int rubber_key, byte state);
void setSustainPedal(bool pressed);
void setPotentiometerValue(int index, int value);
int getAnalogReadsCount();
byte getPinMode(int i);

// please copy the following configuration from potentiometers.cpp
#define POTS_RESOLUTION_MICROSECONDS  5000
#define POTS_THRESHOLD_VALUE          8 // 1024 divided by 128
#define POTS_PB_CENTER_DEADZONE       4
#define POTS_NUMBER                   2
#define POT_TYPE_PITCHBEND       0xE000
#define POT_TYPE_MODWHEEL        0xB001
#define POT_TYPE_VOLUME          0xB007
#define POT_TYPE_PAN             0xB00A
#define POT_TYPE_EXPRESSION      0xB00B
#define POT_TYPE_RESONANCE       0xB047
#define POT_TYPE_FILTER          0xB04A
#define POT_TYPE_REVERB          0xB05B
#define POT_TYPE_CHORUS          0xB05D
const int POTS_ANALOG_PINS[POTS_NUMBER] = {
    A0,
    A1
};
const int POTS_TYPES[POTS_NUMBER] = {
    POT_TYPE_PITCHBEND,
    POT_TYPE_MODWHEEL
};
