# Moura's Keyboard Scanner

Turn your broken (or unused) keyboard into a MIDI controller

This Arduino sketch was the one that I used to make the project demonstrated
in [this](https://www.youtube.com/watch?v=z840N9P-T2k) video.
It is about a keyboard controller that I've made using an old Alesis QS6 Keyboard
directly connected to an Arduino Mega rev3 acting as keyboard scanner with
velocity reading and sustain pedal support.

In 2017 I did the same with another keyboard (an old Casio from a friend).
The code was refactored and a great library called [DIO2](https://github.com/FryDay/DIO2)
was used to speed up the scanning and clean up the old code.

In 2020, thanks to Leandro Meucchi, from Argentina, the code improved to be used with any keyboard.
He made the PDF showing the keyboard wiring for Yamaha PSR530 keyboard, that helps a lot to understand what needs to be done.

In 2025 Emerson Seiler did a great job adding Kurzweil SP76II configuration + schematics, velocity sensitivity curves and potentiomers support, as he shows [here](https://www.youtube.com/watch?v=GndR5BkHnv0). He also suggested to explode the code into small pieces.

I started 2026 working on Emerson Seiler's job as an opportunity to do a refactoring, creating the concept of "models" and merging his code into the main line. I also experimented mapping the rows x cols in a most professional way (matching the cols to Arduino Mega PORTs) to observe the gain in scans per second (Hz). It is about 7 times. I've got a stable 7KHz using my M-Audio Keystation 88 II keybed.

## Features

- Sustain pedal
- Velocity sensitivity with curves support
- Potentiometers

![keyboardscanner](assets/keyboardscanner.jpg)

## How velocity works

Normally it is a ribbon rubber with two contacts for each key that touch the board in two diffent moments:
since the key was pressed until it slopes the board completly. The code measure the difference, varying between
2 and 20 ms (useful range), depending on the keyboard. It is transformed in a MIDI value from 0 to 127.

## Diagram of one rubber key

This scheme makes clear how to identify input and output pins. This has been the main question of guys on Youtube.
I hope it helps:

![key](assets/key_scheme.png)

## How to make your own MIDI controller

1. Disassemble the keyboard to have access to the flat cables (one, two or even three, depending on the number of keys and manufacturer);
2. Using a multimeter with the diode testing function selected, find out and understand the matrix, starting from the first key. Some keyboards have a logical pattern, some doesn't;
3. Connect the ribbon pins **directly** to the Arduino Mega (because it has enough pins to connect any keyboard with velocity). You **dont't** need to change anything in the keyboard circuit board;
4. Duplicate one of the existing models and change the pins in the model.h (output_pins + input_pins), uncomment DEBUG_MIDI_MESSAGE in globals.h and see the console output;
5. If the MIDI messages looks good, comment DEBUG_MIDI_MESSAGE back and use some Serial<->MIDI Bridge (I use [serialmidi](https://github.com/raspy135/serialmidi) on macOS) to test the keyboard with some softsynth to make sure that everything goes well;
6. Optionally, consider turning your Arduino into a MIDI device using [HIDUINO](https://github.com/ddiakopoulos/hiduino), [mocoLUFA](https://github.com/kuwatay/mocolufa) or other similar firmware.
7. Enjoy!

## Converting an Arduino Mega into a MIDI device

To accomplish this we need to overwrite Arduino Mega's firmware with one that boots up as a regular MIDI (and allows us switch the boot mode to the default Arduino USB/serial, when needed). We just need to:

- Make sure that SERIAL_SPEED is set to 31250 in the code;
- Download dualMoco.hex firmware from [here](https://github.com/kuwatay/mocolufa/tree/master/HEX);
- Write the firmware using [this procedure](https://support.arduino.cc/hc/en-us/articles/4408887452434-Flash-the-USB-to-serial-firmware-for-UNO-Rev3-and-earlier-and-Mega-boards);
- Now the device should be recognized as a regular MIDI device.
