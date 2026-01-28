
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

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

#define MODEL_HEADER_PATH models/MODEL_NAME/model.h
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#include STR(MODEL_HEADER_PATH)

void initIOPins();
#ifdef DEBUG_SCANS_PER_SECOND
void countCycles();
#endif
void scanMatrix();
void updateStates();
void sendKeyEvent(byte status_byte, byte key_index, unsigned long time);
void sendSustainPedalEvent(boolean pressed);
void sendMidiEvent(byte status_byte, byte data1, byte data2);
#ifdef ENABLE_POTENTIOMETER_SUPPORT
void initPotentiometers();
void readPotentiometers();
#endif

#endif
