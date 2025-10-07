// Salmos 150

/*
Project based design:

Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxe@oxesoft.com>

This code is originally hosted at https://github.com/oxesoft/keyboardscanner
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <DIO2.h>           // DIO2 library for fast I/O
#include "config.h"         // Pin definitions and global variables
#include "input.h"          // Reading keyboard and pedal inputs
#include "midi.h"           // Function to send MIDI events
#include "states.h"         // Functions for managing key states
#include "potentiometer.h"  // Functions to manage potentiometer states

void setup() {
  Serial.begin(SERIAL_SPEED);  // 115200 for hairless - 31250 for MOCO lufa  pinMode(13, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  startConfig();
}

void loop() {
#ifdef DEBUG_SCANS_PER_SECOND
  countCicles();  // Function to count cycles per second (debug)
#endif
  readInputs();    // Reads the state of the keys and pedal
  updateStates();  // Update key states and send MIDI
    // potentiometers(); // Reads potentiometers and send MIDI - needs review
}