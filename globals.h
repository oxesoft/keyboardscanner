
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

#ifdef ENABLE_MIDI_ASSIGNMENTS_SUPPORT
void initMidiAssignments();
void readMidiAssignments();

enum MidiBtnMode : uint8_t {
  MIDI_BTN_MODE_TOGGLE = 0,
  MIDI_BTN_MODE_PUSH   = 1
};

struct MidiButtonMapping {
  uint8_t channel;  // 1-16
  uint8_t cc;       // 0-127
  uint8_t mode;     // MidiBtnMode
};

struct MidiPotMapping {
  uint8_t channel;  // 1-16
  uint8_t cc;       // 0-127 (fixed by config for now)
  uint8_t min;      // 0-127
  uint8_t max;      // 0-127
};

MidiButtonMapping getMidiAssignButtonMapping(uint8_t idx);
void setMidiAssignButtonMapping(uint8_t idx, const MidiButtonMapping& m);

MidiPotMapping getMidiAssignPotMapping(uint8_t idx);
void setMidiAssignPotMapping(uint8_t idx, const MidiPotMapping& m);
#endif

#ifdef ENABLE_UI
void initUi();
void updateUi();
#endif
#endif
