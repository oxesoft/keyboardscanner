void updateStates()
{
    byte *state = keys_state;
    unsigned long *ktime = keys_time;
    boolean *signal = signals;
    for (byte key = 0; key < KEYS_NUMBER; key++)
    {
        for (byte state_index = 0; state_index < 2; state_index++)
        {
            switch (*state)
            {
            case KEY_OFF:
                if (state_index == 0 && *signal)
                {
                    *state = KEY_START;
                    *ktime = millis();
                }
                break;
            case KEY_START:
                if (state_index == 0 && !*signal)
                {
                    *state = KEY_OFF;
                    break;
                }
                if (state_index == 1 && *signal)
                {
                    *state = KEY_ON;
                    send_midi_event(0x90, key, millis() - *ktime);
                }
                break;
            case KEY_ON:
                if (state_index == 1 && !*signal)
                {
                    *state = KEY_RELEASED;
                    *ktime = millis();
                }
                break;
            case KEY_RELEASED:
                if (state_index == 0 && !*signal)
                {
                    if (pedal)
                    {
                        *state = KEY_SUSTAINED;
                        break;
                    }
                    *state = KEY_OFF;
                    send_midi_event(0x80, key, millis() - *ktime);
                }
                break;
            case KEY_SUSTAINED:
                if (!pedal)
                {
                    *state = KEY_OFF;
                    send_midi_event(0x80, key, MAX_TIME_MS);
                }
                if (state_index == 0 && *signal)
                {
                    *state = KEY_SUSTAINED_RESTART;
                    *ktime = millis();
                }
                break;
            case KEY_SUSTAINED_RESTART:
                if (state_index == 0 && !*signal)
                {
                    *state = KEY_SUSTAINED;
                    digitalWrite(13, HIGH);
                    break;
                }
                if (state_index == 1 && *signal)
                {
                    *state = KEY_ON;
                    send_midi_event(0x80, key, MAX_TIME_MS);
                    send_midi_event(0x90, key, millis() - *ktime);
                }
                break;
            }
            signal++;
        }
        state++;
        ktime++;
    }
}