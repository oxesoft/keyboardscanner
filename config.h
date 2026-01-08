#include "models/kurzweil_sp76ii/model.h" // change here your keyboard model

// Uncomment the next line to inspect the number of scans per seconds
// #define DEBUG_SCANS_PER_SECOND
/*
For reference, I've got these results at the time that I tested it:
426 cyles per second (2,35ms per cycle) using standard digitalWrite/digitalRead
896 cyles per second (1,11ms per cycle) using DIO2 digitalWrite2/digitalRead2
*/

// Uncoment the next line to get text midi message at output
// #define DEBUG_MIDI_MESSAGE

// Enables debug mode to check MIN_TIME_MS and MAX_TIME_MS values
// #define DEBUG_VELOCITY_TIMES

// Uncomment for using one of the available velocity curves
// #define VELOCITY_CURVE VELOCITY_CURVE_LINEAR
// #define VELOCITY_CURVE VELOCITY_CURVE_CONVEX
// #define VELOCITY_CURVE VELOCITY_CURVE_SATURATED
// #define VELOCITY_CURVE VELOCITY_CURVE_CONCAVE

// Cheap keyboards often has the black keys softer or harder than the white ones
// Uncomment the two next lines to allow a soft correction
// #define BLACK_KEYS_CORRECTION
// #define BLACK_KEYS_MULTIPLIER 192 // 127 is the central value (corresponding to 1.0)
// Copy the following code snippet to your model.h and configure it
// #ifdef DEFINE_BLACK_KEYS_MAP
// byte black_keys[KEYS_NUMBER] = { // each index corresponds to the white (zero) and black (one) keys
//     0,1,0,1,0,0,1,0,1,0,1,0,
//     0,1,0,1,0,0,1,0,1,0,1,0,
//     0,1,0,1,0,0,1,0,1,0,1,0,
//     0,1,0,1,0,0,1,0,1,0,1,0,
//     0,1,0,1,0,0,1,0,1,0,1,0,
//     0
// };
// #endif

// Uncoment the following line to use potentiometers (pitch bend, modulation wheel, knobs and sliders)
// #define ENABLE_POTENTIOMETER_SUPPORT

#define SERIAL_SPEED 31250 // 115200 for hairless; 31250 for MOCO lufa
