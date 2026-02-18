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
#include "mocks.h"
#include <memory.h>
#include <stdio.h>

// #define PRINT_SERIAL_WRITE

#define MODEL_PINS_DEF ../models/MODEL_NAME/pins.h

byte io_pins[KEYS_NUMBER * 4] = {
#define PINS(output_pin, input_pin) \
    output_pin, \
    input_pin,
#include STR(MODEL_PINS_DEF)
#undef PINS
};

byte rubber_keys[KEYS_NUMBER * 2] = {RUBBER_KEY_RELEASED};
byte pin_modes[NUM_DIGITAL_PINS] = {INPUT};
int output_pin;
boolean sustain_pedal = HIGH;
int analog_pins[16] = {0};
int analog_reads_counter = 0;

// Static variable for mock timing
static unsigned long mockMicros = 0;

// Arduino pin operations implementations
void pinMode2(int pin, byte mode)
{
    pin_modes[pin] = mode;
}

void digitalWrite2(int pin, int value)
{
    output_pin = value == LOW ? pin : 0;
}

int digitalRead2(int pin)
{
    if (output_pin)
    {
        int input_pin = pin;
        for (int i = 0; i < KEYS_NUMBER * 4; i+=2)
        {
            if (io_pins[i] == output_pin && io_pins[i + 1] == input_pin)
            {
                return rubber_keys[i >> 1] == RUBBER_KEY_PRESSED ? LOW : HIGH;
            }
        }
    }
    else if (pin == SUSTAIN_PEDAL_PIN)
    {
        return sustain_pedal;
    }
    printf("Unexpected digitalRead2(%d)\n", pin);
    return LOW;
}

int analogRead(int pin)
{
    analog_reads_counter++;
    for (int i = 0; i < POTS_NUMBER; i++)
    {
        if (pin == POTS_ANALOG_PINS[i])
        {
            return analog_pins[i];
        }
    }
    printf("Unexpected analogRead(%d)\n", pin);
    return 0;
}

// Timing functions implementation
unsigned long micros()
{
    return mockMicros;
}

void delayMicroseconds(unsigned long ms)
{
}

// Helper functions for mock control
void advanceMockMicros(unsigned long us)
{
    mockMicros += us;
}

void setRubberKey(int rubber_key, byte state)
{
    rubber_keys[rubber_key] = state;
}

void setSustainPedal(bool pressed)
{
    sustain_pedal = pressed == true ? LOW : HIGH;
}

byte getPinMode(int pin)
{
    return pin_modes[pin];
}

void setPotentiometerValue(int index, int value)
{
    analog_pins[index] = value;
}

int getAnalogReadsCount()
{
    return analog_reads_counter;
}

// SerialMock class implementations
void SerialMock::begin(unsigned long baud)
{
    memset(this->circular_buffer, 0, sizeof(this->circular_buffer));
    this->available_bytes = 0;
    this->current_position = 0;
}

// Helper function to print binary representation
static void printBinary(unsigned long value, bool negative = false)
{
    if (negative) printf("-");
    if (value == 0) {
        printf("0");
        return;
    }
    
    char buffer[65];
    int i = 0;
    while (value > 0) {
        buffer[i++] = (value % 2) + '0';
        value /= 2;
    }
    for (int j = i - 1; j >= 0; j--) {
        printf("%c", buffer[j]);
    }
}

void SerialMock::print(const char* str)
{
    printf("%s", str);
}

void SerialMock::print(int value)
{
    printf("%d", value);
}

void SerialMock::print(int value, int base)
{
    switch (base) {
        case 2:  printBinary(value < 0 ? -value : value, value < 0); break;
        case 8:  printf("%o", value); break;
        case 10: printf("%d", value); break;
        case 16: printf("%x", value); break;
        default: printf("%d", value); break;
    }
}

void SerialMock::print(unsigned int value, int base)
{
    switch (base) {
        case 2:  printBinary(value); break;
        case 8:  printf("%o", value); break;
        case 10: printf("%u", value); break;
        case 16: printf("%x", value); break;
        default: printf("%u", value); break;
    }
}

void SerialMock::print(long value, int base)
{
    switch (base) {
        case 2:  printBinary(value < 0 ? -value : value, value < 0); break;
        case 8:  printf("%lo", value); break;
        case 10: printf("%ld", value); break;
        case 16: printf("%lx", value); break;
        default: printf("%ld", value); break;
    }
}

void SerialMock::print(unsigned long value, int base)
{
    switch (base) {
        case 2:  printBinary(value); break;
        case 8:  printf("%lo", value); break;
        case 10: printf("%lu", value); break;
        case 16: printf("%lx", value); break;
        default: printf("%lu", value); break;
    }
}

void SerialMock::print(unsigned char value, int base)
{
    switch (base) {
        case 2:  printBinary(value); break;
        case 8:  printf("%o", value); break;
        case 10: printf("%u", value); break;
        case 16: printf("%x", value); break;
        default: printf("%u", value); break;
    }
}

void SerialMock::println(const char* str)
{
    printf("%s\n", str);
}

void SerialMock::println(int value)
{
    printf("%d\n", value);
}

void SerialMock::println(int value, int base)
{
    print(value, base);
    printf("\n");
}

void SerialMock::println(unsigned int value, int base)
{
    print(value, base);
    printf("\n");
}

void SerialMock::println(long value, int base)
{
    print(value, base);
    printf("\n");
}

void SerialMock::println(unsigned long value, int base)
{
    print(value, base);
    printf("\n");
}

void SerialMock::println(unsigned char value, int base)
{
    print(value, base);
    printf("\n");
}

void SerialMock::write(byte byte)
{
#ifdef PRINT_SERIAL_WRITE
    printf("%02X\n", byte);
#endif
    this->circular_buffer[this->current_position++] = byte;
    if (this->current_position > sizeof(SerialMock::circular_buffer))
    {
        this->current_position = 0;
    };
    this->available_bytes++;
}

int SerialMock::available()
{
    return this->available_bytes;
}

int SerialMock::read()
{
    return this->circular_buffer[this->current_position - this->available_bytes--];
}

// Global Serial instance
SerialMock Serial;