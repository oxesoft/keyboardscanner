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

/*
    5V  ────┬────────────
            │
        [ POT ] ← linear 10 kΩ
            │
            └────── A0
            │
    GND ────┴────────────

    TIPS:
    - Use linear potentiometers (B-taper) of 10 kΩ
    - Avoid logarithmic/audio (A-taper)
    - For extra precision, add 100 nF from A0 to GND
    - If this module is enabled and there is no potentiometers connected, connects the configured inputs to GND
*/

int analogRawValues[POTS_NUMBER] = {0}; // 10-bit ADC (0–1023)
int midiValues[POTS_NUMBER] = {0};
unsigned long lastReadingTime = 0;

void initPotentiometers()
{
    for (int i = 0; i < POTS_NUMBER; i++)
    {
        if (POTS_TYPES[i] == POT_TYPE_PITCHBEND)
        {
            midiValues[i] = 8192;
        }
    }
}

void readPotentiometers()
{
    unsigned long currentTime = millis();
    if (currentTime - lastReadingTime < POTS_RESOLUTION_MILISECONDS)
    {
        return;
    }
    for (int i = 0; i < POTS_NUMBER; i++)
    {
        int raw = analogRead(POTS_ANALOG_PINS[i]);
        if (POTS_TYPES[i] == POT_TYPE_PITCHBEND)
        {
            const int CENTER = 512;
            int delta = raw - CENTER;
            if (abs(delta) <= POTS_PB_CENTER_DEADZONE)
            {
                delta = 0;  // force perfect center
            }
            byte status = (POTS_TYPES[i] & 0xFF00) >> 8;
            int value = (delta << 4) + 8192; // scale to MIDI range (0–16383)
            if (midiValues[i] == value)
            {
                continue;
            }
            midiValues[i] = value;
            byte lsb = value & 0x7F;
            byte msb = value >> 7;
            sendMidiEvent(status, lsb, msb);
        }
        else
        {
            int lastRaw = analogRawValues[i];
            if (abs(raw - lastRaw) < POTS_THREASHOLD_VALUE)
            {
                continue;
            }
            analogRawValues[i] = raw;
            byte status = (POTS_TYPES[i] & 0xFF00) >> 8;
            byte cc     = POTS_TYPES[i] & 0x00FF;
            byte value  = raw >> 3; // scale to MIDI range (0–127)
            sendMidiEvent(status, cc, value);
        }
    }
    lastReadingTime = currentTime;
}

#endif
