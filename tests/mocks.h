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

#ifndef MOCKS_H
#define MOCKS_H

#include <stdlib.h>

// Mock specific definitions
#define RUBBER_KEY_RELEASED 1
#define RUBBER_KEY_PRESSED  2

// Arduino type definitions
typedef unsigned char boolean;
typedef unsigned char byte;

// Arduino utility macros
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define min(a,b) ((a)<(b)?(a):(b))
#define map(value,fromLow,fromHigh,toLow,toHigh) (((value)-(fromLow))*((toHigh)-(toLow))/((fromHigh)-(fromLow))+(toLow))

// Arduino pin mode constants
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

// Arduino digital pin states
#define LOW 0
#define HIGH 1

// Arduino analog pins constants
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define A8 8
#define A9 9
#define A10 10
#define A11 11
#define A12 12
#define A13 13
#define A14 14
#define A15 15

// DIO2 compatibility macros
#define digitalWrite2 digitalWrite
#define digitalRead2 digitalRead

// Function declarations for Arduino pin operations
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
int analogRead(int pin);

// Function declarations for timing operations
unsigned long millis();

// Helper function declarations for mock control
void advanceMockMillis(unsigned long ms);
void setRubberKey(int rubber_key, byte state);
void setSustainPedal(bool pressed);
void setPotentiometerValue(int index, int value);
int getAnalogReadsCount();

// Arduino main function declarations
void setup();
void loop();

// SerialMock class declaration
class SerialMock {
private:
    byte circular_buffer[1024];
    int available_bytes;
    int current_position;
public:
    void begin(unsigned long baud);
    void print(const char* str);
    void print(int value);
    void println(const char* str);
    void println(int value);
    void write(byte byte);
    int  available();
    int  read();
};

// Global Serial instance declaration
extern SerialMock Serial;

#endif // MOCKS_H
