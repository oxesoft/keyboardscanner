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

#include "../globals.h"
#include <assert.h>

// please change config.h to include "models/kurzweil_sp76ii/model.h" and turn off velocity curve
int main()
{
    setup();
    assert(Serial.available() == 0);

#ifdef ENABLE_POTENTIOMETER_SUPPORT
    // please change config.h with the following configuration
    /*
    #define ENABLE_POTENTIOMETER_SUPPORT
    #define POTS_RESOLUTION_MILISECONDS   5
    #define POTS_THREASHOLD_VALUE         8 // 1024 divided by 128
    #define POTS_PB_CENTER_DEADZONE       4
    #define POTS_NUMBER                   2
    const int POTS_ANALOG_PINS[POTS_NUMBER] = {
        A0,
        A1
    };
    const int POTS_TYPES[POTS_NUMBER] = {
        POT_TYPE_PITCHBEND,
        POT_TYPE_MODWHEEL
    };
    */
    setPotentiometerValue(0, 512);
    // test rate control
    assert(getAnalogReadsCount() == 0);
    advanceMockMillis(POTS_RESOLUTION_MILISECONDS);
    loop();
    assert(getAnalogReadsCount() == POTS_NUMBER);
    // test that nothing happens with pitch bend at center position (no bend)
    advanceMockMillis(POTS_RESOLUTION_MILISECONDS);
    loop();
    assert(Serial.available() == 0);
    // tests that a MIDI message is sent moving pith bend all the way up
    setPotentiometerValue(0, 1023);
    advanceMockMillis(POTS_RESOLUTION_MILISECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xE0);
    assert(Serial.read() == 0x7F);
    assert(Serial.read() == 0x7F);
    // tests that a MIDI message is sent moving pith bend all the way down
    setPotentiometerValue(0, 0);
    advanceMockMillis(POTS_RESOLUTION_MILISECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xE0);
    assert(Serial.read() == 0x00);
    assert(Serial.read() == 0x00);
    // tests that a MIDI message is sent moving modulation wheel to the center position
    setPotentiometerValue(1, 512);
    advanceMockMillis(POTS_RESOLUTION_MILISECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x01);
    assert(Serial.read() == 0x40);
    // tests that a MIDI message is sent moving modulation wheel all the way up
    setPotentiometerValue(1, 1023);
    advanceMockMillis(POTS_RESOLUTION_MILISECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x01);
    assert(Serial.read() == 0x7F);
    // tests that a MIDI message is sent moving modulation wheel all the down
    setPotentiometerValue(1, 0);
    advanceMockMillis(POTS_RESOLUTION_MILISECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x01);
    assert(Serial.read() == 0x00);
#endif

    // start pressing first key
    setRubberKey(0, RUBBER_KEY_PRESSED);
    loop();
    // fully immediate pressing first key
    setRubberKey(1, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x90);
    assert(Serial.read() == 0x1C);
    assert(Serial.read() == 0x7F);

    // start releasing first key
    setRubberKey(1, RUBBER_KEY_RELEASED);
    loop();
    // fully pressing first key with maximum velocity
    setRubberKey(0, RUBBER_KEY_RELEASED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x80);
    assert(Serial.read() == 0x1C);
    assert(Serial.read() == 0x7F);

    // start pressing first key
    setRubberKey(0, RUBBER_KEY_PRESSED);
    loop();
    // fully pressing first key with minimum velocity
    advanceMockMillis(MAX_TIME_MS);
    setRubberKey(1, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x90);
    assert(Serial.read() == 0x1C);
    assert(Serial.read() == 0x00);

    // start releasing first key
    setRubberKey(1, RUBBER_KEY_RELEASED);
    loop();
    // fully releasing first key with minimum velocity
    advanceMockMillis(MAX_TIME_MS);
    setRubberKey(0, RUBBER_KEY_RELEASED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x80);
    assert(Serial.read() == 0x1C);
    assert(Serial.read() == 0x00);

    // start pressing second key
    setRubberKey(2, RUBBER_KEY_PRESSED);
    loop();
    // fully pressing second key with medium velocity
    advanceMockMillis(MIN_TIME_MS + ((MAX_TIME_MS - MIN_TIME_MS) / 2));
    setRubberKey(3, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x90);
    assert(Serial.read() == 0x1D);
    assert(Serial.read() == 0x40);

    // pressing sustain pedal
    setSustainPedal(true);

    // start releasing second key
    setRubberKey(3, RUBBER_KEY_RELEASED);
    loop();
    // fully releasing second key with minimum velocity
    setRubberKey(2, RUBBER_KEY_RELEASED);
    advanceMockMillis(MAX_TIME_MS);
    loop();
    assert(Serial.available() == 0);

    // releasing sustain pedal
    setSustainPedal(false);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x80);
    assert(Serial.read() == 0x1D);
    assert(Serial.read() == 0x00);

    Serial.println("Tests OK");
}
