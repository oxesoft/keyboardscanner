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
int main() {
    setup();
    assert(Serial.getAvailableBytes() == 0);
    loop();
    assert(Serial.getAvailableBytes() == 0);

    // start pressing first key
    setRubberKey(0, RUBBER_KEY_PRESSED);
    loop();
    // fully immediate pressing first key
    setRubberKey(1, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.getAvailableBytes() == 3);
    assert(Serial.readWrittenByte() == 0x90);
    assert(Serial.readWrittenByte() == 0x1C);
    assert(Serial.readWrittenByte() == 0x7F);

    // start releasing first key
    setRubberKey(1, RUBBER_KEY_RELEASED);
    loop();
    // fully pressing first key with maximum velocity
    setRubberKey(0, RUBBER_KEY_RELEASED);
    loop();
    assert(Serial.getAvailableBytes() == 3);
    assert(Serial.readWrittenByte() == 0x80);
    assert(Serial.readWrittenByte() == 0x1C);
    assert(Serial.readWrittenByte() == 0x7F);

    // start pressing first key
    setRubberKey(0, RUBBER_KEY_PRESSED);
    loop();
    // fully pressing first key with minimum velocity
    advanceMockMillis(MAX_TIME_MS);
    setRubberKey(1, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.getAvailableBytes() == 3);
    assert(Serial.readWrittenByte() == 0x90);
    assert(Serial.readWrittenByte() == 0x1C);
    assert(Serial.readWrittenByte() == 0x00);

    // start releasing first key
    setRubberKey(1, RUBBER_KEY_RELEASED);
    loop();
    // fully releasing first key with minimum velocity
    advanceMockMillis(MAX_TIME_MS);
    setRubberKey(0, RUBBER_KEY_RELEASED);
    loop();
    assert(Serial.getAvailableBytes() == 3);
    assert(Serial.readWrittenByte() == 0x80);
    assert(Serial.readWrittenByte() == 0x1C);
    assert(Serial.readWrittenByte() == 0x00);

    // start pressing second key
    setRubberKey(2, RUBBER_KEY_PRESSED);
    loop();
    // fully pressing second key with medium velocity
    advanceMockMillis(MIN_TIME_MS + ((MAX_TIME_MS - MIN_TIME_MS) / 2));
    setRubberKey(3, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.getAvailableBytes() == 3);
    assert(Serial.readWrittenByte() == 0x90);
    assert(Serial.readWrittenByte() == 0x1D);
    assert(Serial.readWrittenByte() == 0x40);

    // pressing sustain pedal
    setSustainPedal(true);

    // start releasing second key
    setRubberKey(3, RUBBER_KEY_RELEASED);
    loop();
    // fully releasing second key with minimum velocity
    setRubberKey(2, RUBBER_KEY_RELEASED);
    advanceMockMillis(MAX_TIME_MS);
    loop();
    assert(Serial.getAvailableBytes() == 0);

    // releasing sustain pedal
    setSustainPedal(false);
    loop();
    assert(Serial.getAvailableBytes() == 3);
    assert(Serial.readWrittenByte() == 0x80);
    assert(Serial.readWrittenByte() == 0x1D);
    assert(Serial.readWrittenByte() == 0x00);

    printf("Tests OK\n");
}
