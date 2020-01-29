# Roland HP-101 Overhaul
This repo adapts the code provided [here](https://github.com/oxesoft/keyboardscanner) for use with a [Roland HP-101](https://www.roland.com/nz/products/hp-101/) 88-key digital piano and an [Arduino Due](https://www.arduino.cc/en/Guide/ArduinoDue). It also details the required wiring, though it will vary for pianos other than the HP-101.

The HP-101 I picked up was completely dead, so the Arduino Due replaces the main boards inside the piano to restore its MIDI functionality. Sound is only generated from the other end of the MIDI connection (computer e.t.c), not from the Arduino itself, so the piano can no longer be used on its own. In a malfunctioning piano, the actual speakers are likely to be in working order, so it is simple enough to connect a speaker amplifier and run audio back from the device you have generating it.


![Overview](https://raw.githubusercontent.com/hobbsjacob/keyboardscanner/master/overview.png)


## Changes 
### Arduino Due

The original repo was designed for use with an Arduino Mega which runs on a 16MHz clock like most other Arduino boards. It was able to poll all keys 896 times per second on a 61-key keyboard. The Due runs on an 84 MHz clock and polls at 728 times per second on an 88-key keyboard using the standard digitalRead() and 2380 per second when optimized with serial enabled.

### Native USB

The Due has a native USB port. The [MIDIUSB](https://github.com/arduino-libraries/MIDIUSB) library allows the Due to appear as a class compliant MIDI device, making it plug and play and reducing the need for a program to pipe data from a serial port to a virtual MIDI device (see [Hairless MIDI Serial](https://projectgus.github.io/hairless-midiserial/)).

### Pedals

The original repo handled only a single sustain pedal and did so by not sending any MIDI note off commands if the pedal is depressed. The proper way to handle pedals with MIDI is to use a control. This repo supports all three pedals using their corresponding MIDI controls.


### Microseconds

Each key actuates two switches which are slightly offset. Keyboards find the velocity of each key press by measuring the length of time between the actuation of the two switches. The original repo used milliseconds through the standard millis() function, though the code uses a maximum time difference of 47 milliseconds. When adjusting the sensitivity to require a heavier touch, even smaller time differences must be used. MIDI notes have 127 velocity stops, meaning that measuring in milliseconds is not making use of the granularity available. Simply switching to the standard micros() function solves this.


### Velocity Adjustment

Most digital pianos offer a method of adjusting the velocity of the keys to suit light or heavy touches. The HP-101 has a volume control knob which has been repurposed to set the velocity sensitivity as the Arduino boots. Polling the ADC to update the sensitivity during operation is too time consuming, so it is only checked on boot and can be updated by resetting the Arduino.



## Connections


![Schematic](https://raw.githubusercontent.com/hobbsjacob/keyboardscanner/master/schematic.png)

With the conductor of the FPC cable facing upwards, the side of the connector facing towards you is numbered left to right.