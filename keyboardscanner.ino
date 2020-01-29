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

#include "MIDIUSB.h"

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

inline void digitalWriteDirect(int pin, boolean val){
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

inline int digitalReadDirect(int pin){
  return !!(g_APinDescription[pin].pPort -> PIO_PDSR & g_APinDescription[pin].ulPin);
}



#define KEYS_NUMBER 88

#define KEY_OFF               0
#define KEY_START             1
#define KEY_ON                2
#define KEY_RELEASED          3

#define MIN_TIME_MS   3000
#define MAX_TIME_MS   20000
#define MAX_TIME_MS_N (MAX_TIME_MS - MIN_TIME_MS)

#define PEDAL_MIDDLE  8
#define PEDAL_LEFT    9
#define PEDAL_SUS     7

byte output_pins[] = {46,47,48,49,50,51,52,53};
byte input_pins[] = {37,36,35,34,33,32,31,30,29,28,27,26,25,24,39,38,41,40,43,42,45,44};

//uncomment the next line to get text midi message at output
//#define DEBUG_MIDI_MESSAGE

byte          keys_state[KEYS_NUMBER];
unsigned long keys_time[KEYS_NUMBER];
boolean       signals[sizeof(input_pins) * sizeof(output_pins)];
boolean       pedal_enabled;
boolean       is_sus;
boolean       is_soft;
boolean       is_middle;
unsigned long minimumMicros;
unsigned long maximumMicros;

void setup() {
    minimumMicros = 100;
    maximumMicros = map(analogRead(A0), 0, 1023, 20000, 80000);     //Read potentiometer
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

    pinMode(PEDAL_MIDDLE, INPUT_PULLUP);
    pinMode(PEDAL_LEFT, INPUT_PULLUP);
    pinMode(PEDAL_SUS, INPUT_PULLUP);
    is_sus = false;
    is_soft = false;
    is_middle=false;
}

void loop() {

    byte a = digitalReadDirect(PEDAL_SUS);
    byte b = digitalReadDirect(PEDAL_LEFT);
    byte c = digitalReadDirect(PEDAL_MIDDLE);
    //control 64 = sustain, 67 = soft, sostenuto = 66

    if(a == 0){
      if(is_sus == true){
       controlChange(0, 64, 0);
       is_sus = false; 
      }
    }
    else{
      if(is_sus == false){
        controlChange(0, 64, 127);
        is_sus = true;
      }
    }

    if(b == 0){
      if(is_soft == true){
       controlChange(0, 67, 0);
       is_soft = false; 
      }
    }
    else{
      if(is_soft == false){
        controlChange(0, 67, 127);
        is_soft = true;
      }
    }

    if(c == 0){
      if(is_middle == true){
       controlChange(0, 66, 0);
       is_middle = false; 
      }
    }
    else{
      if(is_middle == false){
        controlChange(0, 66, 127);
        is_middle = true;
      }
    }
    

    

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
    
    boolean *s = signals;
    for (byte section_index = 0; section_index < sizeof(input_pins); section_index += 2)  //for each input pin pair
    {
        for (byte o = 0; o < sizeof(output_pins); o++)                      //For each output pin
        {
            byte output_pin = output_pins[o];
            for (byte i = 0; i < 2; i++)                          //for each in the pair
            {
                byte input_pin = input_pins[section_index + i];
                digitalWriteDirect(output_pin, LOW);
                *(s++) = !digitalReadDirect(input_pin);
                digitalWriteDirect(output_pin, HIGH);
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
                    *ktime = micros();
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
                    unsigned long t = micros() - *ktime;
                    *state = KEY_ON;
                    if (t > maximumMicros)
                    t = maximumMicros;
                    if (t < minimumMicros)
                    t = minimumMicros;
                    t -= minimumMicros;
                    byte velocity = 127 - map(t, 0, maximumMicros - minimumMicros, 1, 127);
                    byte keys = 21 + key;
                    //send_midi_event(0x90, key, millis() - *ktime);
                    noteOn(0, keys, velocity);
                    #ifdef DEBUG_MIDI_MESSAGE
                      char out[32];
                      sprintf(out, "%02X %02X %03d %d", 0x90, keys, velocity, t);
                    #endif
                }
                break;
            case KEY_ON:
                if (state_index == 1 && !*signal)
                {
                    *state = KEY_RELEASED;
                    *ktime = micros();
                }
                break;
            case KEY_RELEASED:
                if (state_index == 0 && !*signal)
                {
                  unsigned long t = micros() - *ktime;
                    *state = KEY_OFF;
                    if (t > maximumMicros)
                    t = maximumMicros;
                    if (t < minimumMicros)
                    t = minimumMicros;
                    t -= minimumMicros;
                    byte velocity = 127 - map(t, minimumMicros, maximumMicros, 1, 127);
                    
                    byte keys = 21 + key;
                    //send_midi_event(0x80, key, millis() - *ktime);
                    noteOff(0, keys, velocity);
                    #ifdef DEBUG_MIDI_MESSAGE
                      char out[32];
                      sprintf(out, "%02X %02X %03d %d", 0x80, keys, velocity, t);
                      Serial.println(out);
                    #endif
                }
                break;
            }
            signal++;
        }
        state++;
        ktime++;
    }
}
