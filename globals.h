#include <Arduino.h>

#ifndef GLOBALS_H
#define GLOBALS_H

#include <DIO2.h> // install the library DIO2

#define VELOCITY_CURVE_LINEAR    0
#define VELOCITY_CURVE_CONVEX    1
#define VELOCITY_CURVE_SATURATED 2
#define VELOCITY_CURVE_CONCAVE   3

#include "config.h"

#define KEY_OFF                  0
#define KEY_START                1
#define KEY_ON                   2
#define KEY_RELEASED             3
#define KEY_SUSTAINED            4
#define KEY_SUSTAINED_RESTART    5

void initStates();
void initIOPins();
#ifdef DEBUG_SCANS_PER_SECOND
void countCycles();
#endif
void scanMatrix();
void updateStates();
void sendKeyEvent(byte status_byte, byte key_index, unsigned long time);
void sendMidiEvent(byte status_byte, byte data1, byte data2);
#ifdef ENABLE_POTENTIOMETER_SUPPORT
void readPotentiometers();
#endif

extern boolean matrix_signals[KEYS_NUMBER * 2];
extern byte    sustain_pedal_signal;

#endif
