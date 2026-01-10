## Key Mapping and Definition

A touch-sensitive keyboard matrix has inputs and outputs. In this project, I used an M-Audio Axiom Pro keyboard, which has an 11x12 matrix.
Each key consists of four contacts; the lower and upper contacts have Zener diodes.
This keyboard did not require diode inversion.

In the model.h file, 23 pins were defined (PIN_Ax and PIN_Bx) using pins 22 to 44 of the Arduino Mega.

  `byte output_pins[] =  {
   PIN_B1,  
   PIN_B1, 
  }`

  `byte input_pins[] = {
   PIN_A1, 0
   PIN_A2,
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
