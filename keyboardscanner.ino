/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxe@oxesoft.com>

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

#include <DIO2.h> // install the library DIO2

#define KEYS_NUMBER 61

#define KEY_OFF               0
#define KEY_START             1
#define KEY_ON                2
#define KEY_RELEASED          3
#define KEY_SUSTAINED         4
#define KEY_SUSTAINED_RESTART 5

#define MIN_TIME_MS   3
#define MAX_TIME_MS   50
#define MAX_TIME_MS_N (MAX_TIME_MS - MIN_TIME_MS)

#define PEDAL_PIN     21

//find out the pins using a multimeter, starting from the first key
//see the picture key_scheme.png to understand how to map the inputs and outputs

//the following configuration is specific for PSR530
//thanks Leandro Meucchi, from Argentina, by the PDF
//take a look at the scheme detailed in PSR530.pdf and modify the following mapping according to the wiring of your keyboard
#define PIN_A1  33
#define PIN_A2  32
#define PIN_A3  31
#define PIN_A4  30
#define PIN_A5  29
#define PIN_A6  28
#define PIN_A7  27
#define PIN_A8  26
#define PIN_A9  25
#define PIN_A10 24
#define PIN_A11 23
#define PIN_A12 22
#define PIN_B1  40
#define PIN_B2  39
#define PIN_B3  38
#define PIN_B4  37
#define PIN_B5  36
#define PIN_B6  35
#define PIN_B7  34
#define PIN_C1  45
#define PIN_C2  44
#define PIN_C3  43
#define PIN_C4  42
#define PIN_C5  41

byte output_pins[] = {
    PIN_B6, //C0
    PIN_B6,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B7,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5,
    PIN_B5, //C1
    PIN_B5,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B4,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3,
    PIN_B3, //C2
    PIN_B3,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B2,
    PIN_B1,
    PIN_B1,
    PIN_B1,
    PIN_B1,
    PIN_B1,
    PIN_B1,
    PIN_C1,
    PIN_C1,
    PIN_C1,
    PIN_C1,
    PIN_C1, //C3
    PIN_C1,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C2,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3,
    PIN_C3, //C4
    PIN_C3,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C4,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5,
    PIN_C5, //C5
    PIN_C5
};
byte input_pins[] = {
    PIN_A9, //C0
    PIN_A10,
    PIN_A9,
    PIN_A10,
    PIN_A6,
    PIN_A5,
    PIN_A8,
    PIN_A7,
    PIN_A3,
    PIN_A4,
    PIN_A1,
    PIN_A2,
    PIN_A11,
    PIN_A12,
    PIN_A11,
    PIN_A12,
    PIN_A1,
    PIN_A2,
    PIN_A3,
    PIN_A4,
    PIN_A8,
    PIN_A7,
    PIN_A6,
    PIN_A5,
    PIN_A9, //C1
    PIN_A10,
    PIN_A9,
    PIN_A10,
    PIN_A6,
    PIN_A5,
    PIN_A8,
    PIN_A7,
    PIN_A3,
    PIN_A4,
    PIN_A1,
    PIN_A2,
    PIN_A11,
    PIN_A12,
    PIN_A11,
    PIN_A12,
    PIN_A1,
    PIN_A2,
    PIN_A3,
    PIN_A4,
    PIN_A8,
    PIN_A7,
    PIN_A6,
    PIN_A5,
    PIN_A9, //C2
    PIN_A10,
    PIN_A9,
    PIN_A10,
    PIN_A1,
    PIN_A2,
    PIN_A6,
    PIN_A5,
    PIN_A8,
    PIN_A7,
    PIN_A3,
    PIN_A4,
    PIN_A11,
    PIN_A12,
    PIN_A11,
    PIN_A12,
    PIN_A3,
    PIN_A4,
    PIN_A9,
    PIN_A10,
    PIN_A6,
    PIN_A5,
    PIN_A8,
    PIN_A7,
    PIN_A1, //C3
    PIN_A2,
    PIN_A1,
    PIN_A2,
    PIN_A6,
    PIN_A5,
    PIN_A8,
    PIN_A7,
    PIN_A3,
    PIN_A4,
    PIN_A9,
    PIN_A10,
    PIN_A11,
    PIN_A12,
    PIN_A11,
    PIN_A12,
    PIN_A1,
    PIN_A2,
    PIN_A3,
    PIN_A4,
    PIN_A8,
    PIN_A7,
    PIN_A6,
    PIN_A5,
    PIN_A9, //C4
    PIN_A10,
    PIN_A9,
    PIN_A10,
    PIN_A11,
    PIN_A12,
    PIN_A6,
    PIN_A5,
    PIN_A8,
    PIN_A7,
    PIN_A3,
    PIN_A4,
    PIN_A1,
    PIN_A2,
    PIN_A1,
    PIN_A2,
    PIN_A11,
    PIN_A12,
    PIN_A3,
    PIN_A4,
    PIN_A8,
    PIN_A7,
    PIN_A6,
    PIN_A5,
    PIN_A9, //C5
    PIN_A10
};

//cheap keyboards often has the black keys softer or harder than the white ones
//uncomment the next line to allow a soft correction
//#define BLACK_KEYS_CORRECTION

#ifdef BLACK_KEYS_CORRECTION
#define MULTIPLIER 192 // 127 is the central value (corresponding to 1.0)
byte black_keys[] = {
    0,1,0,1,0,0,1,0,1,0,1,0,
    0,1,0,1,0,0,1,0,1,0,1,0,
    0,1,0,1,0,0,1,0,1,0,1,0,
    0,1,0,1,0,0,1,0,1,0,1,0,
    0,1,0,1,0,0,1,0,1,0,1,0,
    0
};
#endif

//uncomment the next line to inspect the number of scans per seconds
//#define DEBUG_SCANS_PER_SECOND

/*
426 cyles per second (2,35ms per cycle) using standard digitalWrite/digitalRead
896 cyles per second (1,11ms per cycle) using DIO2 digitalWrite2/digitalRead2
*/

//uncoment the next line to get text midi message at output
//#define DEBUG_MIDI_MESSAGE

byte          keys_state[KEYS_NUMBER];
unsigned long keys_time[KEYS_NUMBER];
boolean       signals[KEYS_NUMBER * 2];
boolean       pedal_enabled;

void setup() {
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    int i;
    for (i = 0; i < KEYS_NUMBER; i++)
    {
        keys_state[i] = KEY_OFF;
        keys_time[i] = 0;
    }
    for (byte pin = 0; pin < sizeof(output_pins); pin++)
    {
        pinMode(output_pins[pin], OUTPUT);
    }
    for (byte pin = 0; pin < sizeof(input_pins); pin++)
    {
        pinMode(input_pins[pin], INPUT_PULLUP);
    }
    pinMode(PEDAL_PIN, INPUT_PULLUP);
    pedal_enabled = digitalRead(PEDAL_PIN) != HIGH;
}

void send_midi_event(byte status_byte, byte key_index, unsigned long time)
{
    unsigned long t = time;
#ifdef BLACK_KEYS_CORRECTION
    if (black_keys[key_index])
    {
        t = (t * MULTIPLIER) >> 7;
    }
#endif
    if (t > MAX_TIME_MS)
        t = MAX_TIME_MS;
    if (t < MIN_TIME_MS)
        t = MIN_TIME_MS;
    t -= MIN_TIME_MS;
    unsigned long velocity = 127 - (t * 127 / MAX_TIME_MS_N);
    byte vel = (((velocity * velocity) >> 7) * velocity) >> 7;
    byte key = 36 + key_index;
#ifdef DEBUG_MIDI_MESSAGE
    char out[32];
    sprintf(out, "%02X %02X %03d %d", status_byte, key, vel, time);
    Serial.println(out);
#else
    Serial.write(status_byte);
    Serial.write(key);
    Serial.write(vel);
#endif
}

void loop() {
#ifdef DEBUG_SCANS_PER_SECOND
    static unsigned long cycles = 0;
    static unsigned long start = 0;
    static unsigned long current = 0;
    cycles++;
    current = millis();
    if (current - start >= 1000)
    {
        Serial.println(cycles);
        cycles = 0;
        start = current;
    }
#endif
    byte pedal = LOW;
    if (pedal_enabled)
    {
        pedal = digitalRead2(PEDAL_PIN);
    }
   
    boolean *s = signals;
    for (byte i = 0; i < KEYS_NUMBER * 2; i++)
    {
        byte output_pin = output_pins[i];
        byte input_pin = input_pins[i];
        digitalWrite2(output_pin, LOW);
        *(s++) = !digitalRead2(input_pin);
        digitalWrite2(output_pin, HIGH);
    }

    byte          *state  = keys_state;
    unsigned long *ktime  = keys_time;
    boolean       *signal = signals;
    for (byte key = 0; key < KEYS_NUMBER; key++)
    {
        for (byte state_index = 0; state_index < 2; state_index++)
        {
            switch (*state)
            {
            case KEY_OFF:
                if (state_index == 0 && *signal)
                {
                    *state = KEY_START;
                    *ktime = millis();
                }
                break;
            case KEY_START:
                if (state_index == 0 && !*signal)
                {
                    *state = KEY_OFF;
                    break;
                }
                if (state_index == 1 && *signal)
                {
                    *state = KEY_ON;
                    send_midi_event(0x90, key, millis() - *ktime);
                }
                break;
            case KEY_ON:
                if (state_index == 1 && !*signal)
                {
                    *state = KEY_RELEASED;
                    *ktime = millis();
                }
                break;
            case KEY_RELEASED:
                if (state_index == 0 && !*signal)
                {
                    if (pedal)
                    {
                        *state = KEY_SUSTAINED;
                        break;
                    }
                    *state = KEY_OFF;
                    send_midi_event(0x80, key, millis() - *ktime);
                }
                break;
            case KEY_SUSTAINED:
                if (!pedal)
                {
                    *state = KEY_OFF;
                    send_midi_event(0x80, key, MAX_TIME_MS);
                }
                if (state_index == 0 && *signal)
                {
                    *state = KEY_SUSTAINED_RESTART;
                    *ktime = millis();
                }
                break;
            case KEY_SUSTAINED_RESTART:
                if (state_index == 0 && !*signal)
                {
                    *state = KEY_SUSTAINED;
                    digitalWrite(13, HIGH);
                    break;
                }
                if (state_index == 1 && *signal)
                {
                    *state = KEY_ON;
                    send_midi_event(0x80, key, MAX_TIME_MS);
                    send_midi_event(0x90, key, millis() - *ktime);
                }
                break;
            }
            signal++;
        }
        state++;
        ktime++;
    }
}
