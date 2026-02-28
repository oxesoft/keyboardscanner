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

// Uncomment the next line if you connected buttons or potentiometers to the controller's mappable function on the Arduino.
#define ENABLE_MIDI_ASSIGNMENTS_SUPPORT

// Uncomment the next line to activate buttons.
// The default CH will be 1 and the default CC will be = 20 + respective index, you must change it via the interface if you want to customize it.
#define ENABLE_MIDI_ASSIGN_BUTTONS
#define BUTTONS_ASSIGN_NUMBER          4
const int MIDI_BUTTONS_PINS[BUTTONS_ASSIGN_NUMBER] = { 8, 9, 10, 11};

// Uncomment the next lines to activate potentiometers (CC ONLY).
// The default CH will be 1 and the default CC will be = 30 + respective index, you must change it via the interface if you want to customize it.
// #define ENABLE_MIDI_ASSIGN_POTS
// #define POTS_ASSIGN_NUMBER             2
// const int MIDI_POTS_ANALOG_PINS[POTS_ASSIGN_NUMBER] = { A2, A3 }; // Start from A2, as A0 and A1 are reserved for pitchband and modulation.
// #define POTS_ASSIGN_RESOLUTION_MICROSECONDS  5000
// #define POTS_ASSIGN_THRESHOLD_VALUE          8 // ~1 MIDI step

// Uncomment the next lines from the following line if you have connected an I2C display, an encoder for navigation and back button.
#define ENABLE_UI
// Display OLED I2C 128x64 
// I2C uses pins 20 and 21 on the Arduino Mega; ensure the matrix does not use these pins.
#define UI_USE_OLED_128x64 1
#define UI_OLED_ADDR 0x3C
#define UI_OLED_RESET_PIN -1

// Encoder + back button
#define UI_PIN_BACK_BTN 7 // back button
#define UI_PIN_ENC_CLK  2
#define UI_PIN_ENC_DT   3
#define UI_PIN_ENC_SW   4

#ifdef ENABLE_MIDI_ASSIGNMENTS_SUPPORT
void initMidiAssignments();
void readMidiAssignments();

enum MidiBtnMode : uint8_t {
    MIDI_BTN_MODE_TOGGLE = 0,
    MIDI_BTN_MODE_PUSH   = 1
};

struct MidiButtonMapping {
    uint8_t channel;  // 1-16
    uint8_t cc;       // 0-127
    uint8_t mode;     // MidiBtnMode
};

struct MidiPotMapping {
    uint8_t channel;  // 1-16
    uint8_t cc;       // 0-127 (fixed by config for now)
    uint8_t min;      // 0-127
    uint8_t max;      // 0-127
};

MidiButtonMapping getMidiAssignButtonMapping(uint8_t idx);
void setMidiAssignButtonMapping(uint8_t idx, const MidiButtonMapping& m);

MidiPotMapping getMidiAssignPotMapping(uint8_t idx);
void setMidiAssignPotMapping(uint8_t idx, const MidiPotMapping& m);
#endif

#ifdef ENABLE_UI
void initUi();
void updateUi();
#endif
