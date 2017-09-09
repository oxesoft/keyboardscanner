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

#include "arduino2.h" // install the library from https://github.com/FryDay/DIO2

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
byte output_pins[] = {
    38,
    40,
    42,
    44,
    46,
    23,
    25,
    27
};
byte input_pins[] = {
    22, 24,
    26, 28,
    30, 32,
    34, 36,
    29, 31,
    33, 35,
    37, 39,
    41, 43
};

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
boolean       signals[sizeof(input_pins) * sizeof(output_pins)];
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
    for (byte section_index = 0; section_index < sizeof(input_pins); section_index += 2)
    {
        for (byte o = 0; o < sizeof(output_pins); o++)
        {
            byte output_pin = output_pins[o];
            for (byte i = 0; i < 2; i++)
            {
                byte input_pin = input_pins[section_index + i];
                digitalWrite2(output_pin, LOW);
                *(s++) = !digitalRead2(input_pin);
                digitalWrite2(output_pin, HIGH);
            }
        }
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
