## Key Mapping and Definition

A touch-sensitive keyboard matrix has inputs and outputs. In this project, I used a Kurzweil SP76II keyboard, which has a 16x20 matrix.
Each key consists of four contacts; the lower and upper contacts have Zener diodes (see the image below).
For proper operation, the diodes on all keys were reversed; the keyboard has 152 diodes. Some keyboards will not require this.

All the details of the pin mapping and the matrix are in the pdf [kurzweil_sp76ii.pdf](schematics.pdf)

In the model.h file, 36 pins were defined (PIN_CAx and PIN_CBx) using pins 18 to 53 of the Arduino Mega.
Looking at the matrix, we see that the keys are divided into groups, which can vary from keyboard to keyboard. On this keyboard, the groups are 8 keys, with 1 wire responsible for 8 keys.
At the top, each key has two wires, and these wires are obviously connected to other keys to form the matrix.
Now the mapping becomes simple. In the model.h file, look for output_pins and input_pins within the object and define the pins for each wire coming out of each key.

- The inputs (input_pins) are the wires that are not connected to a diode.
- The output pins are the wires that are directly connected to the diodes
- In the code, each key must be represented by 2 output_pins and 2 input_pins. In this context, my E1 (Mi 1 - first key) was:

  `byte output_pins[] =  {
  PIN_CA9, //E1
  PIN_CA9, //E1
  }`

  `byte input_pins[] = {
  PIN_CA1,
  PIN_CA2,
  }`

- If the diodes in your matrix are in a different configuration, the inputs may need to be swapped for the outputs and vice versa.

## SENSITIVITY CURVE

Four methods have been added for calculating the sensitivity curve (midi.cpp).
By default, the concave curve is selected; I liked its performance.
It is essential to calibrate MIN_TIME_MS and MAX_TIME_MS. To do this, you need to:

1. Leave SERIAL_SPEED at 115200
2. Uncomment DEBUG_VELOCITY
3. Compile the code and open the serial monitor
4. Play very softly and copy the relevant log excerpt.
5. Tap medium and copy the relevant log excerpt.
6. Tap strong and copy the relevant log excerpt.
7. Play very hard and copy the relevant log excerpt.
8. After that, take the code along with the log divided by force and present it to an AI and ask it to calculate the ideal MIN_TIME_MS and MAX_TIME_MS. Analyze the keystroke timing and speed data provided below and calculate the optimal MIN_TIME_MS and MAX_TIME_MS for the given code.
9. Finally, ask the AI to adjust the array of the curve used, based on the calculated MIN_TIME_MS and MAX_TIME_MS. Suggest a new smooth concave curve based on the MIN_TIME_MS and MAX_TIME_MS values.

## OPERATION

To see the project in progress, visit [my YouTube channel](https://youtube.com/@emersonseiler?sub_confirmation=1)

#### Salmos 150 | Psalm 150 🙏🏼
