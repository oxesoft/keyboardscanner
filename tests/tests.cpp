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
#include "../velocity.h"
#include "mocks.h"
#include <assert.h>

#define MODEL_PINS_DEF ../models/MODEL_NAME/pins.h

// please change config.h to turn off velocity curve
int main()
{
    setup();
    #define PINS(output_pin, input_pin)               \
        assert(getPinMode(output_pin) == OUTPUT);     \
        assert(getPinMode(input_pin ) == INPUT_PULLUP);
    #include STR(MODEL_PINS_DEF)
    #undef PINS
    assert(getPinMode(SUSTAIN_PEDAL_PIN) == INPUT_PULLUP);
    assert(Serial.available() == 0);

#ifdef ENABLE_POTENTIOMETER_SUPPORT
    // please change config.h with the following configuration
    /*
    #define ENABLE_POTENTIOMETER_SUPPORT
    #define POTS_RESOLUTION_MICROSECONDS  5000
    #define POTS_THRESHOLD_VALUE          8 // 1024 divided by 128
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
    advanceMockMicros(POTS_RESOLUTION_MICROSECONDS);
    loop();
    assert(getAnalogReadsCount() == POTS_NUMBER);
    // test that nothing happens with pitch bend at center position (no bend)
    advanceMockMicros(POTS_RESOLUTION_MICROSECONDS);
    loop();
    assert(Serial.available() == 0);
    // tests that a MIDI message is sent moving pith bend all the way up
    setPotentiometerValue(0, 1023);
    advanceMockMicros(POTS_RESOLUTION_MICROSECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xE0);
    assert(Serial.read() == 0x7F);
    assert(Serial.read() == 0x7F);
    // tests that a MIDI message is sent moving pith bend all the way down
    setPotentiometerValue(0, 0);
    advanceMockMicros(POTS_RESOLUTION_MICROSECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xE0);
    assert(Serial.read() == 0x00);
    assert(Serial.read() == 0x00);
    // tests that a MIDI message is sent moving modulation wheel to the center position
    setPotentiometerValue(1, 512);
    advanceMockMicros(POTS_RESOLUTION_MICROSECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x01);
    assert(Serial.read() == 0x40);
    // tests that a MIDI message is sent moving modulation wheel all the way up
    setPotentiometerValue(1, 1023);
    advanceMockMicros(POTS_RESOLUTION_MICROSECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x01);
    assert(Serial.read() == 0x7F);
    // tests that a MIDI message is sent moving modulation wheel all the down
    setPotentiometerValue(1, 0);
    advanceMockMicros(POTS_RESOLUTION_MICROSECONDS);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x01);
    assert(Serial.read() == 0x00);
#endif

    // pressing sustain pedal
    setSustainPedal(true);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x40);
    assert(Serial.read() == 0x7F);

    // releasing sustain pedal
    setSustainPedal(false);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0xB0);
    assert(Serial.read() == 0x40);
    assert(Serial.read() == 0x00);

    int index = 0;

    // start pressing key
    setRubberKey(index, RUBBER_KEY_PRESSED);
    loop();
    // fully immediate pressing key
    setRubberKey(index + 1, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x90);
    assert(Serial.read() == (FIRST_KEY + (index >> 1)));
    assert(Serial.read() == 0x7F);

    // start releasing key
    setRubberKey(index + 1, RUBBER_KEY_RELEASED);
    loop();
    // fully pressing key with maximum velocity
    setRubberKey(index, RUBBER_KEY_RELEASED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x80);
    assert(Serial.read() == (FIRST_KEY + (index >> 1)));
    assert(Serial.read() == 0x7F);

    index += 2;
    const unsigned long MINIMUM_VELOCITY_MICROSECONDS = MAX_TIME_US;

    // start pressing key
    setRubberKey(index, RUBBER_KEY_PRESSED);
    loop();
    // fully pressing key with minimum velocity
    advanceMockMicros(MINIMUM_VELOCITY_MICROSECONDS);
    setRubberKey(index + 1, RUBBER_KEY_PRESSED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x90);
    assert(Serial.read() == (FIRST_KEY + (index >> 1)));
    assert(Serial.read() == VELOCITY_CURVE[0]);

    // start releasing key
    setRubberKey(index + 1, RUBBER_KEY_RELEASED);
    loop();
    // fully releasing key with minimum velocity
    advanceMockMicros(MINIMUM_VELOCITY_MICROSECONDS);
    setRubberKey(index, RUBBER_KEY_RELEASED);
    loop();
    assert(Serial.available() == 3);
    assert(Serial.read() == 0x80);
    assert(Serial.read() == (FIRST_KEY + (index >> 1)));
    assert(Serial.read() == VELOCITY_CURVE[0]);

    index += 2;
    const unsigned long MEDIUM_VELOCITY_MICROSECONDS = MIN_TIME_US + ((MAX_TIME_US - MIN_TIME_US) / 2);

    while (index < KEYS_NUMBER * 2)
    {
        // start pressing next key
        setRubberKey(index, RUBBER_KEY_PRESSED);
        loop();
        // fully pressing key with medium velocity
        advanceMockMicros(MEDIUM_VELOCITY_MICROSECONDS);
        setRubberKey(index + 1, RUBBER_KEY_PRESSED);
        loop();
        assert(Serial.available() == 3);
        assert(Serial.read() == 0x90);
        assert(Serial.read() == (FIRST_KEY + (index >> 1)));
        assert(Serial.read() == VELOCITY_CURVE[64]);

        // start releasing key
        setRubberKey(index + 1, RUBBER_KEY_RELEASED);
        loop();
        // fully releasing key with medium velocity
        setRubberKey(index, RUBBER_KEY_RELEASED);
        advanceMockMicros(MEDIUM_VELOCITY_MICROSECONDS);
        loop();
        assert(Serial.available() == 3);
        assert(Serial.read() == 0x80);
        assert(Serial.read() == (FIRST_KEY + (index >> 1)));
        assert(Serial.read() == VELOCITY_CURVE[64]);

        index += 2;
    }

    Serial.println("Tests OK");
}
