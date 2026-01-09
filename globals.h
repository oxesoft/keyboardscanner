
#ifndef GLOBALS_H
#define GLOBALS_H

#if TEST_MODE
#include "tests/mocks.h"
#else
#include <Arduino.h>
#include <DIO2.h> // install the library DIO2
#endif

#define VELOCITY_CURVE_LINEAR    0
#define VELOCITY_CURVE_CONVEX    1
#define VELOCITY_CURVE_SATURATED 2
#define VELOCITY_CURVE_CONCAVE   3

#define POT_TYPE_PITCHBEND       0xE000
#define POT_TYPE_MODWHEEL        0xB001
#define POT_TYPE_VOLUME          0xB007
#define POT_TYPE_PAN             0xB00A
#define POT_TYPE_EXPRESSION      0xB00B
#define POT_TYPE_RESONANCE       0xB047
#define POT_TYPE_FILTER          0xB04A
#define POT_TYPE_REVERB          0xB05B
#define POT_TYPE_CHORUS          0xB05D

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
void initPotentiometers();
void readPotentiometers();
#endif

extern boolean matrix_signals[KEYS_NUMBER * 2];
extern byte    sustain_pedal_signal;

#endif
