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

#include "globals.h"

#ifdef ENABLE_POTENTIOMETER_SUPPORT

#define PITCHBEND_CENTER 8192
#define PITCHBEND_DEADZONE 100
const int POT_MIN = 200; // Adjustment based on actual minimum reading
const int POT_MAX = 800; // Adjustment based on actual maximum reading

const int N_POTS = 3;                             // total number of pots (slide and rotary)
const int POT_ARDUINO_PIN[N_POTS] = {A0, A1, A2}; // pins of each pot connected directly to the Arduino
int potCState[N_POTS] = {0};                      // current status of the analog port
int potPState[N_POTS] = {0};                      // previous state of the analog port
int potVar = 0;                                   // variation between the value of the previous and current state of the analog port
int midiCState[N_POTS] = {0};                     // Current status of midi value
int midiPState[N_POTS] = {0};                     // Previous state of midi value
const int TIMEOUT = 150;                          // amount of time the potentiometer will be read after exceeding the varThreshold
const int varThreshold = 50;                      // threshold for the variation in the potentiometer signal
boolean potMoving = true;                         // if the potentiometer is moving
unsigned long PTime[N_POTS] = {0};                // previously stored time
unsigned long timer[N_POTS] = {0};                // stores the time that has passed since the timer was reset
boolean pit = false;
boolean mod = false;
byte cc = 1; // Lowest MIDI CC to use

void readPotentiometers()
{
    // Debug only
    for (int i = 0; i < N_POTS; i++)
    {
        // Serial.print(potCState[i]); Serial.print(" ");
        Serial.print(midiCState[i]);
        Serial.print(" ");
    }
    Serial.println();

    for (int i = 0; i < N_POTS; i++)
    {
        potCState[i] = analogRead(POT_ARDUINO_PIN[i]);

        if (i == 0)
        { // Modulation wheel
            midiCState[i] = map(potCState[i], 450, 880, 0, 127);
        }
        else if (i == 1)
        { // Pitchbend
            // Apply filter to smooth reading
            int rawValue = map(potCState[i], 215, 795, 0, 16383);
            midiCState[i] = (midiCState[i] * 0.7) + (rawValue * 0.3);

            // Implementa zona morta mais precisa
            if (abs(midiCState[i] - 8192) < 100)
            {
                midiCState[i] = 8192;
            }
        }
        else if (i == 2)
        { // Volume
            midiCState[i] = map(potCState[i], 0, 1023, 0, 127);
        }

        potVar = abs(potCState[i] - potPState[i]);

        if (potVar > varThreshold)
        {
            PTime[i] = millis();
        }

        timer[i] = millis() - PTime[i];
        potMoving = (timer[i] < TIMEOUT);

        if (potMoving && midiPState[i] != midiCState[i])
        {
            if (i == 0)
            { // Modulation
                sendMidiEvent(0xB0, cc + i, midiCState[i]);
                mod = false;
            }
            else if (i == 1)
            { // Pitchbend
                byte lsb = midiCState[i] & 0x7F;
                byte msb = (midiCState[i] >> 7) & 0x7F;
                sendMidiEvent(0xE0, lsb, msb);
            }
            else if (i == 2)
            { // Volume
                sendMidiEvent(0xB0, 7, midiCState[i]);
            }

            potPState[i] = potCState[i];
            midiPState[i] = midiCState[i];
        }

        // Reset controls when stopped
        if (!potMoving)
        {
            if (i == 1 && abs(midiCState[i] - 8192) < 100)
            { // Pitchbend center
                midiCState[i] = 8192;
                sendMidiEvent(0xE0, 0, 64); // Exact center
            }
            else if (i == 0 && midiCState[i] < 10 && !mod)
            { // Modulation reset
                midiCState[i] = 0;
                sendMidiEvent(0xB0, 1, 0);
                mod = true;
            }
        }
    }
}

#endif
