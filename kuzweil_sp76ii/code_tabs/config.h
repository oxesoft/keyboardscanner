// ========= DEBUG =========

//#define DEBUG_SCANS_PER_SECOND //uncomment the next line to inspect the number of scans per seconds
//#define DEBUG_MIDI_MESSAGE //uncoment the next line to get text midi message at output
/*
426 cyles per second (2,35ms per cycle) using standard digitalWrite/digitalRead
896 cyles per second (1,11ms per cycle) using DIO2 digitalWrite2/digitalRead2
*/

// #define DEBUG_VELOCITY  // Ativa modo debug para verificar valores de MIN_TIME_MS e MAX_TIME_MS
#define SERIAL_SPEED 115200 // 115200 for hairless - 31250 for MOCO lufa  VELOCIDADE SERIAL

// ========= KEYS =========
#define KEYS_NUMBER 76 // Number of keyboard keys
#define KEY_FIRST 28 // Number midi of first key
#define KEY_OFF 0
#define KEY_START 1
#define KEY_ON 2
#define KEY_RELEASED 3
#define KEY_SUSTAINED 4
#define KEY_SUSTAINED_RESTART 5

// ========= SENSIBILIDADE TECLAS =========
#define MIN_TIME_MS 10 // Decrease to capture faster taps
#define MAX_TIME_MS 80 // Increase for greater dynamic range
#define MAX_TIME_MS_N (MAX_TIME_MS - MIN_TIME_MS)

#define MAX_VEL_CURVE_INDEX 80

const byte linearCurve[MAX_VEL_CURVE_INDEX+1]={127, 127, 127, 125, 124, 122, 121, 119, 118, 116, 114, 113, 111, 110, 108, 107, 105, 103, 102, 100, 99, 97, 95, 94, 92, 91, 89, 88, 86, 84, 83, 81, 80, 78, 77, 75, 73, 72, 70, 69, 67, 66, 64, 62, 61, 59, 58, 56, 54, 53, 51, 50, 48, 47, 45, 43, 42, 40, 39, 37, 36, 34, 32, 31, 29, 28, 26, 25, 23, 21, 20, 18, 17, 15, 13, 12, 10, 9, 7, 6, 4};
const byte convexCurve[MAX_VEL_CURVE_INDEX+1]={127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 125, 124, 123, 123, 122, 121, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 109, 108, 107, 105, 104, 102, 101, 99, 98, 96, 94, 93, 91, 89, 87, 86, 84, 82, 80, 78, 76, 74, 72, 70, 68, 66, 63, 61, 59, 57, 54, 52, 50, 48, 45, 43, 41, 38, 36, 33, 31, 29, 26, 24, 21, 19, 16, 14, 11, 9, 6, 4};
const byte saturatedCurve[MAX_VEL_CURVE_INDEX+1]={127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 126, 125, 125, 124, 122, 121, 119, 118, 116, 114, 111, 109, 106, 104, 101, 98, 94, 91, 87, 84, 80, 76, 72, 68, 64, 60, 55, 51, 47, 42, 37, 33, 28, 23, 18, 14, 9, 4};
const byte concaveCurve[MAX_VEL_CURVE_INDEX+1] = {127, 127, 127, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 115, 113, 111, 109, 107, 105, 103, 101, 99, 97, 95, 93, 91, 89, 87, 85, 83, 81, 79, 77, 75, 73, 71, 69, 67, 65, 63, 61, 59, 57, 55, 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 1};

const byte* velocityCurves[]={
  linearCurve,
  convexCurve,
  saturatedCurve,
  concaveCurve
};

// SUSTAIN PEDAL PIN DEFINITION
#define PEDAL_PIN 17

// KEYBOARD MATRIX PIN DEFINITION
// SMALLER BOARD 32 KEYS
#define PIN_CA1 18
#define PIN_CA2 19
#define PIN_CA3 22
#define PIN_CA4 24
#define PIN_CA5 26
#define PIN_CA6 28
#define PIN_CA7 30
#define PIN_CA8 32
#define PIN_CA9 20
#define PIN_CA10 21
#define PIN_CA11 23
#define PIN_CA12 25
#define PIN_CA13 27
#define PIN_CA14 29
#define PIN_CA15 31
#define PIN_CA16 33

// LARGER BOARD 44 KEYS
#define PIN_CB17 34
#define PIN_CB18 36
#define PIN_CB19 38
#define PIN_CB20 40
#define PIN_CB21 42
#define PIN_CB22 44
#define PIN_CB23 46
#define PIN_CB24 48
#define PIN_CB25 50
#define PIN_CB26 52
#define PIN_CB27 35
#define PIN_CB28 37
#define PIN_CB29 39
#define PIN_CB30 41
#define PIN_CB31 43
#define PIN_CB32 45
#define PIN_CB33 47
#define PIN_CB34 49
#define PIN_CB35 51
#define PIN_CB36 53

byte output_pins[] = {
 // SMALLER BOARD 32 KEYS
  PIN_CA9,
  PIN_CA9,
  PIN_CA10,
  PIN_CA10,
  PIN_CA11,
  PIN_CA11,
  PIN_CA12,
  PIN_CA12,
  PIN_CA13,
  PIN_CA13,
  PIN_CA14,
  PIN_CA14,
  PIN_CA15,
  PIN_CA15,
  PIN_CA16,
  PIN_CA16,

  PIN_CA9,
  PIN_CA9,
  PIN_CA10,
  PIN_CA10,
  PIN_CA11,
  PIN_CA11,
  PIN_CA12,
  PIN_CA12,
  PIN_CA13,
  PIN_CA13,
  PIN_CA14,
  PIN_CA14,
  PIN_CA15,
  PIN_CA15,
  PIN_CA16,
  PIN_CA16,

  PIN_CA9,
  PIN_CA9,
  PIN_CA10,
  PIN_CA10,
  PIN_CA11,
  PIN_CA11,
  PIN_CA12,
  PIN_CA12,
  PIN_CA13,
  PIN_CA13,
  PIN_CA14,
  PIN_CA14,
  PIN_CA15,
  PIN_CA15,
  PIN_CA16,
  PIN_CA16,

  PIN_CA9,
  PIN_CA9,
  PIN_CA10,
  PIN_CA10,
  PIN_CA11,
  PIN_CA11,
  PIN_CA12,
  PIN_CA12,
  PIN_CA13,
  PIN_CA13,
  PIN_CA14,
  PIN_CA14,
  PIN_CA15,
  PIN_CA15,
  PIN_CA16,
  PIN_CA16,

  // LARGER BOARD 44 KEYS

  PIN_CB27,
  PIN_CB27,
  PIN_CB28,
  PIN_CB28,
  PIN_CB29,
  PIN_CB29,
  PIN_CB30,
  PIN_CB30,
  PIN_CB31,
  PIN_CB31,
  PIN_CB32,
  PIN_CB32,
  PIN_CB33,
  PIN_CB33,
  PIN_CB34,
  PIN_CB34,

  PIN_CB27,
  PIN_CB27,
  PIN_CB28,
  PIN_CB28,
  PIN_CB29,
  PIN_CB29,
  PIN_CB30,
  PIN_CB30,
  PIN_CB31,
  PIN_CB31,
  PIN_CB32,
  PIN_CB32,
  PIN_CB33,
  PIN_CB33,
  PIN_CB34,
  PIN_CB34,

  PIN_CB27,
  PIN_CB27,
  PIN_CB28,
  PIN_CB28,
  PIN_CB29,
  PIN_CB29,
  PIN_CB30,
  PIN_CB30,
  PIN_CB31,
  PIN_CB31,
  PIN_CB32,
  PIN_CB32,
  PIN_CB33,
  PIN_CB33,
  PIN_CB34,
  PIN_CB34,

  PIN_CB27,
  PIN_CB27,
  PIN_CB28,
  PIN_CB28,
  PIN_CB29,
  PIN_CB29,
  PIN_CB30,
  PIN_CB30,
  PIN_CB31,
  PIN_CB31,
  PIN_CB32,
  PIN_CB32,
  PIN_CB33,
  PIN_CB33,
  PIN_CB34,
  PIN_CB34,

  PIN_CB27,
  PIN_CB27,
  PIN_CB28,
  PIN_CB28,
  PIN_CB29,
  PIN_CB29,
  PIN_CB30,
  PIN_CB30,
  PIN_CB31,
  PIN_CB31,
  PIN_CB32,
  PIN_CB32,
  PIN_CB33,
  PIN_CB33,
  PIN_CB34,
  PIN_CB34,

  PIN_CB27,
  PIN_CB27,
  PIN_CB28,
  PIN_CB28,
  PIN_CB29,
  PIN_CB29,
  PIN_CB30,
  PIN_CB30
};

byte input_pins[] = {
 // SMALLER BOARD 32 KEYS
  PIN_CA1,
  PIN_CA2,
  PIN_CA1,
  PIN_CA2,
  PIN_CA1,
  PIN_CA2,
  PIN_CA1,
  PIN_CA2,
  PIN_CA1,
  PIN_CA2,
  PIN_CA1,
  PIN_CA2,
  PIN_CA1,
  PIN_CA2,
  PIN_CA1,
  PIN_CA2,

  PIN_CA3,
  PIN_CA4,
  PIN_CA3,
  PIN_CA4,
  PIN_CA3,
  PIN_CA4,
  PIN_CA3,
  PIN_CA4,
  PIN_CA3,
  PIN_CA4,
  PIN_CA3,
  PIN_CA4,
  PIN_CA3,
  PIN_CA4,
  PIN_CA3,
  PIN_CA4,

  PIN_CA5,
  PIN_CA6,
  PIN_CA5,
  PIN_CA6,
  PIN_CA5,
  PIN_CA6,
  PIN_CA5,
  PIN_CA6,
  PIN_CA5,
  PIN_CA6,
  PIN_CA5,
  PIN_CA6,
  PIN_CA5,
  PIN_CA6,
  PIN_CA5,
  PIN_CA6,

  PIN_CA7,
  PIN_CA8,
  PIN_CA7,
  PIN_CA8,
  PIN_CA7,
  PIN_CA8,
  PIN_CA7,
  PIN_CA8,
  PIN_CA7,
  PIN_CA8,
  PIN_CA7,
  PIN_CA8,
  PIN_CA7,
  PIN_CA8,
  PIN_CA7,
  PIN_CA8,

  // LARGER BOARD 44 KEYS
  PIN_CB18,
  PIN_CB17,
  PIN_CB18,
  PIN_CB17,
  PIN_CB18,
  PIN_CB17,
  PIN_CB18,
  PIN_CB17,
  PIN_CB18,
  PIN_CB17,
  PIN_CB18,
  PIN_CB17,
  PIN_CB18,
  PIN_CB17,
  PIN_CB18,
  PIN_CB17,

  PIN_CB20,
  PIN_CB19,
  PIN_CB20,
  PIN_CB19,
  PIN_CB20,
  PIN_CB19,
  PIN_CB20,
  PIN_CB19,
  PIN_CB20,
  PIN_CB19,
  PIN_CB20,
  PIN_CB19,
  PIN_CB20,
  PIN_CB19,
  PIN_CB20,
  PIN_CB19,

  PIN_CB22,
  PIN_CB21,
  PIN_CB22,
  PIN_CB21,
  PIN_CB22,
  PIN_CB21,
  PIN_CB22,
  PIN_CB21,
  PIN_CB22,
  PIN_CB21,
  PIN_CB22,
  PIN_CB21,
  PIN_CB22,
  PIN_CB21,
  PIN_CB22,
  PIN_CB21,

  PIN_CB24,
  PIN_CB23,
  PIN_CB24,
  PIN_CB23,
  PIN_CB24,
  PIN_CB23,
  PIN_CB24,
  PIN_CB23,
  PIN_CB24,
  PIN_CB23,
  PIN_CB24,
  PIN_CB23,
  PIN_CB24,
  PIN_CB23,
  PIN_CB24,
  PIN_CB23,

  PIN_CB26,
  PIN_CB25,
  PIN_CB26,
  PIN_CB25,
  PIN_CB26,
  PIN_CB25,
  PIN_CB26,
  PIN_CB25,
  PIN_CB26,
  PIN_CB25,
  PIN_CB26,
  PIN_CB25,
  PIN_CB26,
  PIN_CB25,
  PIN_CB26,
  PIN_CB25,

  PIN_CB36,
  PIN_CB35,
  PIN_CB36,
  PIN_CB35,
  PIN_CB36,
  PIN_CB35,
  PIN_CB36,
  PIN_CB35
};

// Global variables
byte keys_state[KEYS_NUMBER];
unsigned long keys_time[KEYS_NUMBER];
boolean signals[KEYS_NUMBER * 2];
boolean pedal_enabled;
byte pedal = LOW;

// ========= POTENTIOMETERS =========
#define PITCHBEND_CENTER 8192
#define PITCHBEND_DEADZONE 100
const int POT_MIN = 200;  // Adjustment based on actual minimum reading
const int POT_MAX = 800;  // Adjustment based on actual maximum reading

const int N_POTS = 3; // total number of pots (slide and rotary)
const int POT_ARDUINO_PIN[N_POTS] = {A0, A1, A2}; // pins of each pot connected directly to the Arduino
int potCState[N_POTS] = {0}; // current status of the analog port
int potPState[N_POTS] = {0}; // previous state of the analog port
int potVar = 0; // variation between the value of the previous and current state of the analog port
int midiCState[N_POTS] = {0}; // Current status of midi value
int midiPState[N_POTS] = {0}; // Previous state of midi value
const int TIMEOUT = 150; // amount of time the potentiometer will be read after exceeding the varThreshold
const int varThreshold = 50; // threshold for the variation in the potentiometer signal
boolean potMoving = true; // if the potentiometer is moving
unsigned long PTime[N_POTS] = {0}; // previously stored time
unsigned long timer[N_POTS] = {0}; // stores the time that has passed since the timer was reset
boolean pit =false;
boolean mod=false;
byte cc = 1; // Lowest MIDI CC to use


// ============= BLACK KEYS CORRECTION =============
// #define BLACK_KEYS_CORRECTION // ENABLES AND DISABLES BLACK KEY CORRECTION
#ifdef BLACK_KEYS_CORRECTION
#define MULTIPLIER 192  // 127 is the central value (corresponding to 1.0)
byte black_keys[] = {
  0, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 0, 1, 0,
};
#endif


void startConfig() {
  Serial.begin(SERIAL_SPEED); // 115200 for hairless - 31250 for MOCO lufa  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  int i;
  for (i = 0; i < KEYS_NUMBER; i++) {
    keys_state[i] = KEY_OFF;
    keys_time[i] = 0;
  }
  for (byte pin = 0; pin < sizeof(output_pins); pin++) {
    pinMode(output_pins[pin], OUTPUT);
  }
  for (byte pin = 0; pin < sizeof(input_pins); pin++) {
    pinMode(input_pins[pin], INPUT_PULLUP);
  }
  pinMode(PEDAL_PIN, INPUT_PULLUP);
  pedal_enabled = digitalRead(PEDAL_PIN) != HIGH;
}
