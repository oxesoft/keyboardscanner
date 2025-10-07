void debug_velocity_times(byte key_index, unsigned long time) {
  unsigned long t = constrain(time, MIN_TIME_MS, MAX_TIME_MS);
  t -= MIN_TIME_MS;

  byte velocity = constrain(
    127 - ((t * 127) / MAX_TIME_MS_N),
    0,
    127);

  char debug_msg[32];
  sprintf(debug_msg, "KEY:%02d TIME:%03lu VEL:%03d",
          key_index,
          time,
          velocity);
  Serial.println(debug_msg);
}

void send_midi_event(byte status_byte, byte key_index, unsigned long time) {

  unsigned long t = constrain(time, MIN_TIME_MS, MAX_TIME_MS);
  t -= MIN_TIME_MS;

#ifdef BLACK_KEYS_CORRECTION
  if (black_keys[key_index]) {
    t = (t * MULTIPLIER) >> 7;
  }
#endif
  if (t > MAX_TIME_MS)
    t = MAX_TIME_MS;
  if (t < MIN_TIME_MS)
    t = MIN_TIME_MS;
  t -= MIN_TIME_MS;

  // Calculo de velocidade baseado na curva
  byte velocity = concaveCurve[min(MAX_VEL_CURVE_INDEX, time)];

  // byte vel = velocity;
  byte vel = (((velocity * velocity) >> 7) * velocity) >> 7;
  byte key = KEY_FIRST + key_index;

#ifdef DEBUG_MIDI_MESSAGE
  char out[32];
  sprintf(out, "%02X %02X %03d %d", status_byte, key, vel, time);
  Serial.println(out);
#else
  Serial.write(status_byte);
  Serial.write(key);
  Serial.write(vel);
#endif

#ifdef DEBUG_VELOCITY
  debug_velocity_times(key_index, time);
#endif
}

void send_midi_eventcc(byte status_byte, byte key_index, unsigned long time) {
  unsigned long t = time;
#ifdef DEBUG_MIDI_MESSAGE
  char out[32];
  sprintf(out, "%02X %02X %03d %d", status_byte, key_index, vel, t);
  Serial.println(out);
#else
  Serial.write(status_byte);
  Serial.write(key_index);
  Serial.write(t);
#endif
}