# Moura's Keyboard Scanner
Turn your broken (or unused) keyboard in a MIDI controller (with pedal and veloticy)

This Arduino sketch was the one that I used to make the project demonstrated
in [this](https://www.youtube.com/watch?v=z840N9P-T2k) video.
It is about a keyboard controller that I've made using an old Alesis QS6 Keyboard
directly connected to an Arduino Mega rev3 acting as keyboard scanner with 
velocity reading and sustain pedal support.

In 2017 I did the same with another keyboard (an old Casio from a friend).
The code was refactored and a great library called [DIO2](https://github.com/FryDay/DIO2)
was used to speed up the scanning and clean up the old code.

In 2020, thanks to Leandro Meucchi, from Argentina, the code is simpler to be used with any keyboard.
He made the PDF showing the keyboard wiring for Yamaha PSR530 keyboard that helps a lot what need to be done.

Warning: this sketch is for keyboard with velocity support only.

![keyboardscanner](https://raw.githubusercontent.com/oxesoft/keyboardscanner/master/keyboardscanner.jpg)

## How to make your own MIDI controller
1) Disassemble the keyboard to have access to the flat cables (one or two, depending on the number of keys);
2) Using a multimeter with the diode testing find out and understand the matrix order, starting from the first key;
3) Connect the cable directly to the Arduino Mega (because it has enough pins to a 61 keys keyboard with velocity);
4) Change the pins (if necessary) in output_pins and input_pins arrays and have fun :-)

## How velocity works
Normally it is a ribbon rubber with two contacts for each key that touch the board in two diffent moments:
since the key was pressed until it slopes the board completly. The code measure the difference, varying between
2 and 120 ms, depending on the keyboard. It is transformed in a MIDI value from 0 to 127.