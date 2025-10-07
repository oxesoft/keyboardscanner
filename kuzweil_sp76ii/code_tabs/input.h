void readInputs() {
  // byte pedal = LOW;
  if (pedal_enabled) {
    pedal = digitalRead2(PEDAL_PIN);
  }

  boolean *s = signals;
  for (byte i = 0; i < KEYS_NUMBER * 2; i++) {
    byte output_pin = output_pins[i];
    byte input_pin = input_pins[i];
    digitalWrite2(output_pin, LOW);
    *(s++) = !digitalRead2(input_pin);
    digitalWrite2(output_pin, HIGH);
  }
}


void countCicles() {
  static unsigned long cycles = 0;
  static unsigned long start = 0;
  static unsigned long current = 0;
  cycles++;
  current = millis();
  if (current - start >= 1000) {
    Serial.println(cycles);
    cycles = 0;
    start = current;
  }
}
