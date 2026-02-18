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

#define EXTENSION(name) void name##Setup(); void name##Loop();
#include "extensions.h"
#undef EXTENSION

void setup()
{
    Serial.begin(31250); // 115200 for hairless; 31250 for MOCO lufa
    scannerSetup();
    #define EXTENSION(name) name##Setup();
    #include "extensions.h"
    #undef EXTENSION
}

void loop()
{
    scannerLoop();
    statesLoop();
    #define EXTENSION(name) name##Loop();
    #include "extensions.h"
    #undef EXTENSION
}
