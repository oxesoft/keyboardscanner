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
#include <stdio.h>
#include <memory.h>

extern byte output_pins[KEYS_NUMBER * 2];
extern byte input_pins[KEYS_NUMBER * 2];
byte rubber_keys[KEYS_NUMBER * 2] = {RUBBER_KEY_RELEASED};
int output_pin;
boolean sustain_pedal = LOW;

// Static variable for mock timing
static unsigned long mockMillis = 0;

// Arduino pin operations implementations
void pinMode(int pin, int mode) {
}

void digitalWrite(int pin, int value) {
    output_pin = value == LOW ? pin : 0;
}

int digitalRead(int pin) {
    if (output_pin) {
        int input_pin = pin;
        for (int i; i < KEYS_NUMBER * 2; i++) {
            if (output_pins[i] == output_pin && input_pins[i] == input_pin) {
                return rubber_keys[i] == RUBBER_KEY_PRESSED ? LOW : HIGH;
            }
        }
    } else if (pin == SUSTAIN_PEDAL_PIN) {
        return sustain_pedal;
    }
    printf("Unexpected pin=%d\n", pin);
    return LOW;
}

// Timing functions implementation
unsigned long millis() {
    return mockMillis;
}

// Helper functions for mock control

void advanceMockMillis(unsigned long ms) {
    mockMillis += ms;
}

void setRubberKey(int rubber_key, byte state) {
    rubber_keys[rubber_key] = state;
}

void setSustainPedal(bool pressed) {
    sustain_pedal = pressed == true ? HIGH : LOW;
}

// SerialMock class implementations
void SerialMock::begin(unsigned long baud) {
    memset(this->circular_buffer, 0, sizeof(this->circular_buffer));
    this->available_bytes = 0;
    this->current_position = 0;
}

void SerialMock::print(const char* str) {
    printf("%s", str);
}

void SerialMock::print(int value) {
    printf("%d", value);
}

void SerialMock::println(const char* str) {
    printf("%s\n", str);
}

void SerialMock::println(int value) {
    printf("%d\n", value);
}

void SerialMock::write(uint8_t byte) {
    this->circular_buffer[this->current_position++] = byte;
    if (this->current_position > sizeof(SerialMock::circular_buffer)) {
        this->current_position = 0;
    };
    this->available_bytes++;
}

int SerialMock::getAvailableBytes() {
    return this->available_bytes;
}

byte SerialMock::readWrittenByte() {
    return this->circular_buffer[this->current_position - this->available_bytes--];
}

// Global Serial instance
SerialMock Serial;