#define MODEL_NAME fatar_tp40 // change here your keyboard model

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
// The gain on scans per second (Hz) is around 7 times compared to the random pin mapping
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

// Uncomment the next line if you connected buttons or potentiometers to the controller's mappable function on the Arduino.
#define ENABLE_MIDI_ASSIGNMENTS_SUPPORT

// Uncomment the next line to activate buttons.
// The default CH will be 1 and the default CC will be = 20 + respective index, you must change it via the interface if you want to customize it.
#define ENABLE_MIDI_ASSIGN_BUTTONS
#define BUTTONS_ASSIGN_NUMBER          4
const int MIDI_BUTTONS_PINS[BUTTONS_ASSIGN_NUMBER] = { 8, 9, 10, 11};

// Uncomment the next lines to activate potentiometers (CC ONLY).
// The default CH will be 1 and the default CC will be = 30 + respective index, you must change it via the interface if you want to customize it.
// #define ENABLE_MIDI_ASSIGN_POTS
// #define POTS_ASSIGN_NUMBER             2
// const int MIDI_POTS_ANALOG_PINS[POTS_ASSIGN_NUMBER] = { A2, A3 }; // Start from A2, as A0 and A1 are reserved for pitchband and modulation.
// #define POTS_ASSIGN_RESOLUTION_MICROSECONDS  5000
// #define POTS_ASSIGN_THRESHOLD_VALUE          8 // ~1 MIDI step

// Uncomment the next lines from the following line if you have connected an I2C display, an encoder for navigation and back button.
#define ENABLE_UI
// Display OLED I2C 128x64 
// I2C uses pins 20 and 21 on the Arduino Mega; ensure the matrix does not use these pins.
#define UI_USE_OLED_128x64 1
#define UI_OLED_ADDR 0x3C
#define UI_OLED_RESET_PIN -1

// Encoder + back button
#define UI_PIN_BACK_BTN 7 // back button
#define UI_PIN_ENC_CLK  2
#define UI_PIN_ENC_DT   3
#define UI_PIN_ENC_SW   4

#define SERIAL_SPEED 115200 // 115200 for hairless; 31250 for MOCO lufa
