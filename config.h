#define MODEL_NAME maudio_keystation88ii // change here your keyboard model

// #define DEBUG_SCANS_PER_SECOND
/*
For reference, I've got these results at the time that I tested it for 61 keys and random pin mapping:
 426 cyles per second (2,35ms per cycle) using standard digitalWrite/digitalRead
 896 cyles per second (1,11ms per cycle) using DIO2 digitalWrite2/digitalRead2
1185 cyles per second (0,94ms per cycle) using DIO2 digitalWrite2/digitalRead2 with static variables
*/

// Uncoment the next line if you have connected the matrix columns mapped to Arduino's PORTs
// It requiers modifying scanner.cpp code to match your PORTs mapping
// See https://devboards.info/boards/arduino-mega2560-rev3
// The gain on scans per second (Hz) is around 5 times compared to the random pin mapping
// #define DIRECT_PORTS_READING

// Uncoment the next line to get text midi message at output
// #define DEBUG_MIDI_MESSAGE

// Enables debug mode to check MIN_TIME_US and MAX_TIME_US values
// #define DEBUG_VELOCITY_TIMES

// Uncoment the following lines to use potentiometers (pitch bend, modulation wheel, knobs and sliders)
// #define ENABLE_POTENTIOMETER_SUPPORT
// #define POTS_RESOLUTION_MICROSECONDS  5000
// #define POTS_THRESHOLD_VALUE          8 // 1024 divided by 128
// #define POTS_PB_CENTER_DEADZONE       4
// #define POTS_NUMBER                   2
// const int POTS_ANALOG_PINS[POTS_NUMBER] = {
//     A0,
//     A1
// };
// const int POTS_TYPES[POTS_NUMBER] = {
//     POT_TYPE_PITCHBEND,
//     POT_TYPE_MODWHEEL
// };

#define SERIAL_SPEED 31250 // 115200 for hairless; 31250 for MOCO lufa
