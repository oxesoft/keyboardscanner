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

// Arduino digital pin constants
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define LOW 0
#define HIGH 1
#define NUM_DIGITAL_PINS 70

// Arduino built-in LED pin
#define LED_BUILTIN 13

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

// DIO2 digital pin definitions
#define DP0 0
#define DP1 1
#define DP2 2
#define DP3 3
#define DP4 4
#define DP5 5
#define DP6 6
#define DP7 7
#define DP8 8
#define DP9 9
#define DP10 10
#define DP11 11
#define DP12 12
#define DP13 13
#define DP14 14
#define DP15 15
#define DP16 16
#define DP17 17
#define DP18 18
#define DP19 19
#define DP20 20
#define DP21 21
#define DP22 22
#define DP23 23
#define DP24 24
#define DP25 25
#define DP26 26
#define DP27 27
#define DP28 28
#define DP29 29
#define DP30 30
#define DP31 31
#define DP32 32
#define DP33 33
#define DP34 34
#define DP35 35
#define DP36 36
#define DP37 37
#define DP38 38
#define DP39 39
#define DP40 40
#define DP41 41
#define DP42 42
#define DP43 43
#define DP44 44
#define DP45 45
#define DP46 46
#define DP47 47
#define DP48 48
#define DP49 49
#define DP50 50
#define DP51 51
#define DP52 52
#define DP53 53

// DIO2 dummy declaration
#define Arduino_to_GPIO_pin(pin) (pin)

// Function declarations for DIO2 pin operations
void pinMode2f(int pin, byte mode);
void digitalWrite2f(int pin, int value);
int digitalRead2f(int pin);

// Function declarations for Arduino pin operations
int analogRead(int pin);

// Function declarations for timing operations
unsigned long millis();

// Helper function declarations for mock control
void advanceMockMillis(unsigned long ms);
void setRubberKey(int rubber_key, byte state);
void setSustainPedal(bool pressed);
void setPotentiometerValue(int index, int value);
int getAnalogReadsCount();
byte getPinMode(int i);

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
