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

// This file was created to mock Arduino stuff

typedef unsigned char boolean;
typedef unsigned char byte;

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define min(a,b) ((a)<(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define map(value,fromLow,fromHigh,toLow,toHigh) (((value)-(fromLow))*((toHigh)-(toLow))/((fromHigh)-(fromLow))+(toLow))

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define LOW 0
#define HIGH 1
#define NUM_DIGITAL_PINS 70

#define LED_BUILTIN 13

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

int analogRead(int pin);

unsigned long micros();
void delayMicroseconds(unsigned long ms);

void setup();
void loop();

class SerialMock {
private:
    byte circular_buffer[1024];
    int available_bytes;
    int current_position;
public:
    void begin(unsigned long baud);
    void print(const char* str);
    void print(int value);
    void print(int value, int base);
    void print(unsigned int value, int base);
    void print(long value, int base);
    void print(unsigned long value, int base);
    void print(unsigned char value, int base);
    void println(const char* str);
    void println(int value);
    void println(int value, int base);
    void println(unsigned int value, int base);
    void println(long value, int base);
    void println(unsigned long value, int base);
    void println(unsigned char value, int base);
    void write(byte byte);
    int  available();
    int  read();
};

extern SerialMock Serial;
