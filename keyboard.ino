
#define KEYS_NUMBER 61
#define LAST_PIN    53

#define KEY_OFF               0
#define KEY_START             1
#define KEY_ON                2
#define KEY_RELEASED          3
#define KEY_SUSTAINED         4
#define KEY_SUSTAINED_RESTART 5

#define MIN_TIME_MS   2
#define MAX_TIME_MS   120
#define MAX_TIME_MS_N (MAX_TIME_MS - MIN_TIME_MS)

#define GET_SIGNAL(out, output_bit, in, input_bit) \
    out &= ~output_bit; /* delayMicroseconds(2); */ \
    *(s++) = (in & input_bit) ? false : true;      \
    out |= output_bit;

byte          keys_state[KEYS_NUMBER];
unsigned long keys_time[KEYS_NUMBER];
boolean       signals[KEYS_NUMBER * 2];
boolean       pedal_enabled;

void setup() {
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    int i;
    for (i = 0; i < KEYS_NUMBER; i++)
    {
        keys_state[i] = KEY_OFF;
        keys_time[i] = 0;
    }
    i = LAST_PIN;
    while (i > 22)
    {
        pinMode(i--, OUTPUT);
        pinMode(i--, INPUT_PULLUP);
    }
    pinMode(21, INPUT_PULLUP);
    pedal_enabled = digitalRead(21) != HIGH;
}

void send_midi_event(byte status_byte, byte key_index, unsigned long time)
{
    unsigned long t = time;
    if (t > MAX_TIME_MS)
        t = MAX_TIME_MS;
    if (t < MIN_TIME_MS)
        t = MIN_TIME_MS;
    t -= MIN_TIME_MS;
    unsigned long velocity = 127 - (t * 127 / MAX_TIME_MS_N);
    byte vel = (velocity * velocity) >> 7;
    byte key = 36 + key_index;
    Serial.write(status_byte);
    Serial.write(key);
    Serial.write(vel);
}

boolean first = true;
void loop() {
#if 0
    static unsigned long cycles = 0;
    static unsigned long start = 0;
    static unsigned long current = 0;
    cycles++;
    current = millis();
    if (current - start >= 1000)
    {
        Serial.println(cycles);
        cycles = 0;
        start = current;
    }
#endif
    byte pedal = LOW;
    if (pedal_enabled)
        pedal = digitalRead(21);
    
    boolean *s = signals;
    
    GET_SIGNAL(PORTB, 0x04, PINB, 0x02)
    GET_SIGNAL(PORTB, 0x01, PINB, 0x02)
    GET_SIGNAL(PORTB, 0x04, PINB, 0x08)
    GET_SIGNAL(PORTB, 0x01, PINB, 0x08)
    GET_SIGNAL(PORTB, 0x04, PINL, 0x02)
    GET_SIGNAL(PORTB, 0x01, PINL, 0x02)
    GET_SIGNAL(PORTB, 0x04, PINL, 0x08)
    GET_SIGNAL(PORTB, 0x01, PINL, 0x08)
    GET_SIGNAL(PORTB, 0x04, PINL, 0x20)
    GET_SIGNAL(PORTB, 0x01, PINL, 0x20)
    GET_SIGNAL(PORTB, 0x04, PINL, 0x80)
    GET_SIGNAL(PORTB, 0x01, PINL, 0x80)
    GET_SIGNAL(PORTB, 0x04, PING, 0x02)
    GET_SIGNAL(PORTB, 0x01, PING, 0x02)
    GET_SIGNAL(PORTB, 0x04, PIND, 0x80)
    GET_SIGNAL(PORTB, 0x01, PIND, 0x80)
    GET_SIGNAL(PORTL, 0x04, PINB, 0x02)
    GET_SIGNAL(PORTL, 0x01, PINB, 0x02)
    GET_SIGNAL(PORTL, 0x04, PINB, 0x08)
    GET_SIGNAL(PORTL, 0x01, PINB, 0x08)
    GET_SIGNAL(PORTL, 0x04, PINL, 0x02)
    GET_SIGNAL(PORTL, 0x01, PINL, 0x02)
    GET_SIGNAL(PORTL, 0x04, PINL, 0x08)
    GET_SIGNAL(PORTL, 0x01, PINL, 0x08)
    GET_SIGNAL(PORTL, 0x04, PINL, 0x20)
    GET_SIGNAL(PORTL, 0x01, PINL, 0x20)
    GET_SIGNAL(PORTL, 0x04, PINL, 0x80)
    GET_SIGNAL(PORTL, 0x01, PINL, 0x80)
    GET_SIGNAL(PORTL, 0x04, PING, 0x02)
    GET_SIGNAL(PORTL, 0x01, PING, 0x02)
    GET_SIGNAL(PORTL, 0x04, PIND, 0x80)
    GET_SIGNAL(PORTL, 0x01, PIND, 0x80)
    GET_SIGNAL(PORTL, 0x40, PINB, 0x02)
    GET_SIGNAL(PORTL, 0x10, PINB, 0x02)
    GET_SIGNAL(PORTL, 0x40, PINB, 0x08)
    GET_SIGNAL(PORTL, 0x10, PINB, 0x08)
    GET_SIGNAL(PORTL, 0x40, PINL, 0x02)
    GET_SIGNAL(PORTL, 0x10, PINL, 0x02)
    GET_SIGNAL(PORTL, 0x40, PINL, 0x08)
    GET_SIGNAL(PORTL, 0x10, PINL, 0x08)
    GET_SIGNAL(PORTL, 0x40, PINL, 0x20)
    GET_SIGNAL(PORTL, 0x10, PINL, 0x20)
    GET_SIGNAL(PORTL, 0x40, PINL, 0x80)
    GET_SIGNAL(PORTL, 0x10, PINL, 0x80)
    GET_SIGNAL(PORTL, 0x40, PING, 0x02)
    GET_SIGNAL(PORTL, 0x10, PING, 0x02)
    GET_SIGNAL(PORTL, 0x40, PIND, 0x80)
    GET_SIGNAL(PORTL, 0x10, PIND, 0x80)
    GET_SIGNAL(PORTG, 0x04, PINB, 0x02)
    GET_SIGNAL(PORTG, 0x01, PINB, 0x02)
    GET_SIGNAL(PORTG, 0x04, PINB, 0x08)
    GET_SIGNAL(PORTG, 0x01, PINB, 0x08)
    GET_SIGNAL(PORTG, 0x04, PINL, 0x02)
    GET_SIGNAL(PORTG, 0x01, PINL, 0x02)
    GET_SIGNAL(PORTG, 0x04, PINL, 0x08)
    GET_SIGNAL(PORTG, 0x01, PINL, 0x08)
    GET_SIGNAL(PORTG, 0x04, PINL, 0x20)
    GET_SIGNAL(PORTG, 0x01, PINL, 0x20)
    GET_SIGNAL(PORTG, 0x04, PINL, 0x80)
    GET_SIGNAL(PORTG, 0x01, PINL, 0x80)
    GET_SIGNAL(PORTG, 0x04, PING, 0x02)
    GET_SIGNAL(PORTG, 0x01, PING, 0x02)
    GET_SIGNAL(PORTG, 0x04, PIND, 0x80)
    GET_SIGNAL(PORTG, 0x01, PIND, 0x80)
    GET_SIGNAL(PORTC, 0x04, PINC, 0x02)
    GET_SIGNAL(PORTC, 0x01, PINC, 0x02)
    GET_SIGNAL(PORTC, 0x04, PINC, 0x08)
    GET_SIGNAL(PORTC, 0x01, PINC, 0x08)
    GET_SIGNAL(PORTC, 0x04, PINC, 0x20)
    GET_SIGNAL(PORTC, 0x01, PINC, 0x20)
    GET_SIGNAL(PORTC, 0x04, PINC, 0x80)
    GET_SIGNAL(PORTC, 0x01, PINC, 0x80)
    GET_SIGNAL(PORTC, 0x04, PINA, 0x40)
    GET_SIGNAL(PORTC, 0x01, PINA, 0x40)
    GET_SIGNAL(PORTC, 0x04, PINA, 0x10)
    GET_SIGNAL(PORTC, 0x01, PINA, 0x10)
    GET_SIGNAL(PORTC, 0x04, PINA, 0x04)
    GET_SIGNAL(PORTC, 0x01, PINA, 0x04)
    GET_SIGNAL(PORTC, 0x04, PINA, 0x01)
    GET_SIGNAL(PORTC, 0x01, PINA, 0x01)
    GET_SIGNAL(PORTC, 0x40, PINC, 0x02)
    GET_SIGNAL(PORTC, 0x10, PINC, 0x02)
    GET_SIGNAL(PORTC, 0x40, PINC, 0x08)
    GET_SIGNAL(PORTC, 0x10, PINC, 0x08)
    GET_SIGNAL(PORTC, 0x40, PINC, 0x20)
    GET_SIGNAL(PORTC, 0x10, PINC, 0x20)
    GET_SIGNAL(PORTC, 0x40, PINC, 0x80)
    GET_SIGNAL(PORTC, 0x10, PINC, 0x80)
    GET_SIGNAL(PORTC, 0x40, PINA, 0x40)
    GET_SIGNAL(PORTC, 0x10, PINA, 0x40)
    GET_SIGNAL(PORTC, 0x40, PINA, 0x10)
    GET_SIGNAL(PORTC, 0x10, PINA, 0x10)
    GET_SIGNAL(PORTC, 0x40, PINA, 0x04)
    GET_SIGNAL(PORTC, 0x10, PINA, 0x04)
    GET_SIGNAL(PORTC, 0x40, PINA, 0x01)
    GET_SIGNAL(PORTC, 0x10, PINA, 0x01)
    GET_SIGNAL(PORTA, 0x20, PINC, 0x02)
    GET_SIGNAL(PORTA, 0x80, PINC, 0x02)
    GET_SIGNAL(PORTA, 0x20, PINC, 0x08)
    GET_SIGNAL(PORTA, 0x80, PINC, 0x08)
    GET_SIGNAL(PORTA, 0x20, PINC, 0x20)
    GET_SIGNAL(PORTA, 0x80, PINC, 0x20)
    GET_SIGNAL(PORTA, 0x20, PINC, 0x80)
    GET_SIGNAL(PORTA, 0x80, PINC, 0x80)
    GET_SIGNAL(PORTA, 0x20, PINA, 0x40)
    GET_SIGNAL(PORTA, 0x80, PINA, 0x40)
    GET_SIGNAL(PORTA, 0x20, PINA, 0x10)
    GET_SIGNAL(PORTA, 0x80, PINA, 0x10)
    GET_SIGNAL(PORTA, 0x20, PINA, 0x04)
    GET_SIGNAL(PORTA, 0x80, PINA, 0x04)
    GET_SIGNAL(PORTA, 0x20, PINA, 0x01)
    GET_SIGNAL(PORTA, 0x80, PINA, 0x01)
    GET_SIGNAL(PORTA, 0x02, PINC, 0x02)
    GET_SIGNAL(PORTA, 0x08, PINC, 0x02)
    GET_SIGNAL(PORTA, 0x02, PINC, 0x08)
    GET_SIGNAL(PORTA, 0x08, PINC, 0x08)
    GET_SIGNAL(PORTA, 0x02, PINC, 0x20)
    GET_SIGNAL(PORTA, 0x08, PINC, 0x20)
    GET_SIGNAL(PORTA, 0x02, PINC, 0x80)
    GET_SIGNAL(PORTA, 0x08, PINC, 0x80)
    GET_SIGNAL(PORTA, 0x02, PINA, 0x40)
    GET_SIGNAL(PORTA, 0x08, PINA, 0x40)

    /*
    for (byte key = 0; key < KEYS_NUMBER; key++)
    {
        byte section_index = key >> 3;
        byte board_index = key >> 5;
        for (byte state_index = 0; state_index < 2; state_index++)
        {
            byte output_pin = state_index ? LAST_PIN - (section_index * 4) : (LAST_PIN - 2) - (section_index * 4);
            byte input_pin = (LAST_PIN - (board_index * 16) - 1) - ((key & 0x7) * 2);
#if 1
            uint8_t output_bit = digitalPinToBitMask(output_pin);
            uint8_t output_port = digitalPinToPort(output_pin);
            volatile uint8_t *out = portOutputRegister(output_port);
            uint8_t input_bit = digitalPinToBitMask(input_pin);
            uint8_t input_port = digitalPinToPort(input_pin);
            volatile uint8_t *in = portInputRegister(input_port);
            if (first)
            {
                char b[64];
                char *t[] = {"+", "A", "B", "C", "D", "E", "F", "G", "H", "+",  "J", "K", "L"};
                sprintf(b, "GET_SIGNAL(PORT%s, 0x%02x, PIN%s, 0x%02x)", t[output_port], output_bit, t[input_port], input_bit);
                Serial.println(b);
            }
            *out &= ~output_bit;
            *(s++) = (*in & input_bit) ? false : true;
            *out |= output_bit;
#else
            digitalWrite(output_pin, LOW);
            *(s++) = !digitalRead(input_pin);
            digitalWrite(output_pin, HIGH);
#endif
        }
    }
    first = false;
    */
    
    byte          *state  = keys_state;
    unsigned long *ktime  = keys_time;
    boolean       *signal = signals;
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
