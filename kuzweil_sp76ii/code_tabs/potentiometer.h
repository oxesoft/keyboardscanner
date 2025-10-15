void potentiometers()
{
    // Debug only
    for (int i = 0; i < N_POTS; i++)
    {
        // Serial.print(potCState[i]); Serial.print(" ");
        Serial.print(midiCState[i]);
        Serial.print(" ");
    }
    Serial.println();

    for (int i = 0; i < N_POTS; i++)
    {
        potCState[i] = analogRead(POT_ARDUINO_PIN[i]);

        if (i == 0)
        { // Modulation wheel
            midiCState[i] = map(potCState[i], 450, 880, 0, 127);
        }
        else if (i == 1)
        { // Pitchbend
            // Apply filter to smooth reading
            int rawValue = map(potCState[i], 215, 795, 0, 16383);
            midiCState[i] = (midiCState[i] * 0.7) + (rawValue * 0.3);

            // Implementa zona morta mais precisa
            if (abs(midiCState[i] - 8192) < 100)
            {
                midiCState[i] = 8192;
            }
        }
        else if (i == 2)
        { // Volume
            midiCState[i] = map(potCState[i], 0, 1023, 0, 127);
        }

        potVar = abs(potCState[i] - potPState[i]);

        if (potVar > varThreshold)
        {
            PTime[i] = millis();
        }

        timer[i] = millis() - PTime[i];
        potMoving = (timer[i] < TIMEOUT);

        if (potMoving && midiPState[i] != midiCState[i])
        {
            if (i == 0)
            { // Modulation
                send_midi_eventcc(0xB0, cc + i, midiCState[i]);
                mod = false;
            }
            else if (i == 1)
            { // Pitchbend
                byte lsb = midiCState[i] & 0x7F;
                byte msb = (midiCState[i] >> 7) & 0x7F;
                send_midi_eventcc(0xE0, lsb, msb);
            }
            else if (i == 2)
            { // Volume
                send_midi_eventcc(0xB0, 7, midiCState[i]);
            }

            potPState[i] = potCState[i];
            midiPState[i] = midiCState[i];
        }

        // Reset controls when stopped
        if (!potMoving)
        {
            if (i == 1 && abs(midiCState[i] - 8192) < 100)
            { // Pitchbend center
                midiCState[i] = 8192;
                send_midi_eventcc(0xE0, 0, 64); // Exact center
            }
            else if (i == 0 && midiCState[i] < 10 && !mod)
            { // Modulation reset
                midiCState[i] = 0;
                send_midi_eventcc(0xB0, 1, 0);
                mod = true;
            }
        }
    }
}