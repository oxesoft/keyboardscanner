/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

This code is originally hosted at https://github.com/oxesoft/keyboardscanner

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "globals.h"
#include "ui.h"

#ifdef ENABLE_UI

void uiDisplayBegin();
void uiDisplayRenderMenu(const char *title, const char *const *items, int count, int selected, int topIndex);
void uiDisplayRenderEditInt(const char *title, const char *label, int value, int minV, int maxV);
void uiDisplayRenderEditChoice(const char *title, const char *label, const char *choice);
void uiDisplayRenderStatus(const char *title, const char *msg);

struct DebouncedPin {
    uint8_t pin;
    bool lastStable;
    bool lastRead;
    unsigned long lastChangeMs;

    DebouncedPin(uint8_t p)
        : pin(p), lastStable(HIGH), lastRead(HIGH), lastChangeMs(0)
    {
    }
};

static bool pressedEdge(DebouncedPin &b, unsigned long nowMs,
                        unsigned long debounceMs = 25)
{
    bool r = digitalRead(b.pin);
    if (r != b.lastRead) {
        b.lastRead = r;
        b.lastChangeMs = nowMs;
    }
    if ((nowMs - b.lastChangeMs) >= debounceMs && b.lastStable != r) {
        b.lastStable = r;
        return (b.lastStable == LOW);
    }
    return false;
}

// Encoder ISR
static volatile int s_encDelta = 0;
static volatile uint8_t s_lastAB = 0;

static int8_t quadStep(uint8_t prev, uint8_t curr)
{
    if (prev == 0b00 && curr == 0b01)
        return +1;
    if (prev == 0b01 && curr == 0b11)
        return +1;
    if (prev == 0b11 && curr == 0b10)
        return +1;
    if (prev == 0b10 && curr == 0b00)
        return +1;

    if (prev == 0b00 && curr == 0b10)
        return -1;
    if (prev == 0b10 && curr == 0b11)
        return -1;
    if (prev == 0b11 && curr == 0b01)
        return -1;
    if (prev == 0b01 && curr == 0b00)
        return -1;

    return 0;
}

static void isrEncoder()
{
    uint8_t a = digitalRead(UI_PIN_ENC_CLK);
    uint8_t b = digitalRead(UI_PIN_ENC_DT);
    uint8_t ab = (a << 1) | b;

    int8_t step = quadStep(s_lastAB, ab);
    s_lastAB = ab;

    s_encDelta += step;
}

static int consumeClicks()
{
    int delta;
    noInterrupts();
    delta = s_encDelta;
    s_encDelta = 0;
    interrupts();

    static int accum = 0;
    accum += delta;

    const int STEPS_PER_CLICK = 4;
    int clicks = 0;
    while (accum >= STEPS_PER_CLICK) {
        clicks++;
        accum -= STEPS_PER_CLICK;
    }
    while (accum <= -STEPS_PER_CLICK) {
        clicks--;
        accum += STEPS_PER_CLICK;
    }

    return -clicks;
}

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
static constexpr bool HAS_BUTTONS = true;
#else
static constexpr bool HAS_BUTTONS = false;
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
static constexpr bool HAS_POTS = true;
#else
static constexpr bool HAS_POTS = false;
#endif

// Screens
enum Screen : uint8_t {
    SCREEN_MAIN_MENU,
    SCREEN_MIDI_MENU,

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    SCREEN_BUTTONS_MENU,
    SCREEN_BTN_EDIT_CH,
    SCREEN_BTN_EDIT_CC,
    SCREEN_BTN_EDIT_MODE,
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    SCREEN_POTS_MENU,
    SCREEN_POT_EDIT_CH,
    SCREEN_POT_EDIT_CC,
    SCREEN_POT_EDIT_RANGE,
#endif
};

static Screen s_screen = SCREEN_MAIN_MENU;
static int s_cursor = 0;
static int s_top = 0;

static uint8_t s_editIdx = 0;
static int s_editValue = 0;

static DebouncedPin s_back(UI_PIN_BACK_BTN);
static DebouncedPin s_encSw(UI_PIN_ENC_SW);

// Menu strings

// Main menu: Return MUST NOT exist here.
static const char *MAIN_ITEMS[] = {"1 - MIDI Mapping"};
static constexpr int MAIN_COUNT = 1;

// MIDI menu: built dynamically + Return as last (not numbered)
enum MidiMenuAction : uint8_t { MMA_BUTTONS, MMA_POTS, MMA_RETURN };

static const char *s_midiMenuText[4];
static MidiMenuAction s_midiMenuAction[4];
static int s_midiMenuCount = 0;

static void buildMidiMenu()
{
    s_midiMenuCount = 0;

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    s_midiMenuText[s_midiMenuCount] = "1.1 - BUTTONS";
    s_midiMenuAction[s_midiMenuCount] = MMA_BUTTONS;
    s_midiMenuCount++;
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    s_midiMenuText[s_midiMenuCount] = "1.2 - POTENTIOMETERS";
    s_midiMenuAction[s_midiMenuCount] = MMA_POTS;
    s_midiMenuCount++;
#endif

    // Return always last here
    s_midiMenuText[s_midiMenuCount] = "Return";
    s_midiMenuAction[s_midiMenuCount] = MMA_RETURN;
    s_midiMenuCount++;
}

// Buttons/Pots lists: include Return as last item (not numbered)
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
static char s_btnLabels[BUTTONS_ASSIGN_NUMBER][18];
static const char *s_btnMenuItems[BUTTONS_ASSIGN_NUMBER + 1];
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
static char s_potLabels[POTS_ASSIGN_NUMBER][20];
static const char *s_potMenuItems[POTS_ASSIGN_NUMBER + 1];
#endif

static void buildLabels()
{
#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    for (int i = 0; i < BUTTONS_ASSIGN_NUMBER; i++) {
        snprintf(s_btnLabels[i], sizeof(s_btnLabels[i]), "%d - Button %d",
                 i + 1, i + 1);
        s_btnMenuItems[i] = s_btnLabels[i];
    }
    s_btnMenuItems[BUTTONS_ASSIGN_NUMBER] = "Return";
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    for (int i = 0; i < POTS_ASSIGN_NUMBER; i++) {
        snprintf(s_potLabels[i], sizeof(s_potLabels[i]), "%d - Pot %d", i + 1,
                 i + 1);
        s_potMenuItems[i] = s_potLabels[i];
    }
    s_potMenuItems[POTS_ASSIGN_NUMBER] = "Return";
#endif
}

static void ensureScroll(int count)
{
    if (count <= 0) {
        s_cursor = 0;
        s_top = 0;
        return;
    }

    if (s_cursor < 0)
        s_cursor = 0;
    if (s_cursor > count - 1)
        s_cursor = count - 1;

    if (s_cursor < s_top)
        s_top = s_cursor;
    if (s_cursor > s_top + 3)
        s_top = s_cursor - 3;
    if (s_top < 0)
        s_top = 0;

    int maxTop = (count > 4) ? (count - 4) : 0;
    if (s_top > maxTop)
        s_top = maxTop;
}

static void render()
{
    switch (s_screen) {
    case SCREEN_MAIN_MENU: {
        ensureScroll(MAIN_COUNT);
        uiDisplayRenderMenu("Keyboard Scanner", MAIN_ITEMS, MAIN_COUNT, s_cursor, s_top);
        break;
    }

    case SCREEN_MIDI_MENU: {
        ensureScroll(s_midiMenuCount);
        uiDisplayRenderMenu("MIDI Mapping", s_midiMenuText, s_midiMenuCount, s_cursor, s_top);
        break;
    }

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    case SCREEN_BUTTONS_MENU: {
        const int count = BUTTONS_ASSIGN_NUMBER + 1; // + Return
        ensureScroll(count);
        uiDisplayRenderMenu("Buttons", s_btnMenuItems, count, s_cursor, s_top);
        break;
    }

    case SCREEN_BTN_EDIT_CH:
        uiDisplayRenderEditInt("Button", "1 - MIDI Channel", s_editValue, 1,
                               16);
        break;

    case SCREEN_BTN_EDIT_CC:
        uiDisplayRenderEditInt("Button", "2 - MIDI CC", s_editValue, 0, 127);
        break;

    case SCREEN_BTN_EDIT_MODE:
        uiDisplayRenderEditChoice("Button", "3 - Mode", (s_editValue == 0) ? "Toggle" : "Push");
        break;
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    case SCREEN_POTS_MENU: {
        const int count = POTS_ASSIGN_NUMBER + 1; // + Return
        ensureScroll(count);
        uiDisplayRenderMenu("Potentiometers", s_potMenuItems, count, s_cursor, s_top);
        break;
    }

    case SCREEN_POT_EDIT_CH:
        uiDisplayRenderEditInt("Pot", "1 - MIDI Channel", s_editValue, 1, 16);
        break;

    case SCREEN_POT_EDIT_CC:
        uiDisplayRenderEditInt("Pot", "2 - MIDI CC", s_editValue, 0, 127);
        break;

    case SCREEN_POT_EDIT_RANGE:
        uiDisplayRenderEditInt("Pot", "3 - Range (0..X)", s_editValue, 0, 127);
        break;
#endif
    }
}

static void goTo(Screen s, int cursor = 0)
{
    s_screen = s;
    s_cursor = cursor;
    s_top = 0;
    render();
}

static void doBack()
{
    switch (s_screen) {
    case SCREEN_MAIN_MENU:
        // Back on main menu: just ignore (no Return here)
        uiDisplayRenderStatus("Keyboard Scanner", "Top");
        delay(250);
        render();
        break;

    case SCREEN_MIDI_MENU:
        goTo(SCREEN_MAIN_MENU, 0);
        break;

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    case SCREEN_BUTTONS_MENU:
        goTo(SCREEN_MIDI_MENU, 0);
        break;

    case SCREEN_BTN_EDIT_CH:
        goTo(SCREEN_BUTTONS_MENU, (int)s_editIdx);
        break;

    case SCREEN_BTN_EDIT_CC:
        goTo(SCREEN_BTN_EDIT_CH, 0);
        break;

    case SCREEN_BTN_EDIT_MODE:
        goTo(SCREEN_BTN_EDIT_CC, 0);
        break;
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    case SCREEN_POTS_MENU:
        goTo(SCREEN_MIDI_MENU, 0);
        break;

    case SCREEN_POT_EDIT_CH:
        goTo(SCREEN_POTS_MENU, (int)s_editIdx);
        break;

    case SCREEN_POT_EDIT_CC:
        goTo(SCREEN_POT_EDIT_CH, 0);
        break;

    case SCREEN_POT_EDIT_RANGE:
        goTo(SCREEN_POT_EDIT_CC, 0);
        break;
#endif
    }
}

static void doEnter()
{
    switch (s_screen) {
    case SCREEN_MAIN_MENU:
        // Only item: MIDI Mapping
        goTo(SCREEN_MIDI_MENU, 0);
        break;

    case SCREEN_MIDI_MENU: {
        MidiMenuAction act = s_midiMenuAction[s_cursor];
        if (act == MMA_RETURN) {
            goTo(SCREEN_MAIN_MENU, 0);
            break;
        }

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
        if (act == MMA_BUTTONS) {
            goTo(SCREEN_BUTTONS_MENU, 0);
            break;
        }
#endif
#ifdef ENABLE_MIDI_ASSIGN_POTS
        if (act == MMA_POTS) {
            goTo(SCREEN_POTS_MENU, 0);
            break;
        }
#endif
        goTo(SCREEN_MAIN_MENU, 0);
        break;
    }

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    case SCREEN_BUTTONS_MENU: {
        if (s_cursor == BUTTONS_ASSIGN_NUMBER) { // Return
            goTo(SCREEN_MIDI_MENU, 0);
            break;
        }

        s_editIdx = (uint8_t)s_cursor;
        MidiButtonMapping m = getMidiAssignButtonMapping(s_editIdx);
        s_editValue = m.channel;
        goTo(SCREEN_BTN_EDIT_CH, 0);
        break;
    }

    case SCREEN_BTN_EDIT_CH: {
        MidiButtonMapping m = getMidiAssignButtonMapping(s_editIdx);
        m.channel = (uint8_t)s_editValue;
        setMidiAssignButtonMapping(s_editIdx, m);

        m = getMidiAssignButtonMapping(s_editIdx);
        s_editValue = m.cc;
        goTo(SCREEN_BTN_EDIT_CC, 0);
        break;
    }

    case SCREEN_BTN_EDIT_CC: {
        MidiButtonMapping m = getMidiAssignButtonMapping(s_editIdx);
        m.cc = (uint8_t)s_editValue;
        setMidiAssignButtonMapping(s_editIdx, m);

        m = getMidiAssignButtonMapping(s_editIdx);
        s_editValue = (m.mode == MIDI_BTN_MODE_TOGGLE) ? 0 : 1;
        goTo(SCREEN_BTN_EDIT_MODE, 0);
        break;
    }

    case SCREEN_BTN_EDIT_MODE: {
        MidiButtonMapping m = getMidiAssignButtonMapping(s_editIdx);
        m.mode = (uint8_t)((s_editValue == 0) ? MIDI_BTN_MODE_TOGGLE
                                              : MIDI_BTN_MODE_PUSH);
        setMidiAssignButtonMapping(s_editIdx, m);

        uiDisplayRenderStatus("Saved", "OK");
        delay(300);
        goTo(SCREEN_BUTTONS_MENU, (int)s_editIdx);
        break;
    }
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    case SCREEN_POTS_MENU: {
        if (s_cursor == POTS_ASSIGN_NUMBER) { // Return
            goTo(SCREEN_MIDI_MENU, 0);
            break;
        }

        s_editIdx = (uint8_t)s_cursor;
        MidiPotMapping p = getMidiAssignPotMapping(s_editIdx);
        s_editValue = p.channel;
        goTo(SCREEN_POT_EDIT_CH, 0);
        break;
    }

    case SCREEN_POT_EDIT_CH: {
        MidiPotMapping p = getMidiAssignPotMapping(s_editIdx);
        p.channel = (uint8_t)s_editValue;
        setMidiAssignPotMapping(s_editIdx, p);

        p = getMidiAssignPotMapping(s_editIdx);
        s_editValue = p.cc;
        goTo(SCREEN_POT_EDIT_CC, 0);
        break;
    }

    case SCREEN_POT_EDIT_CC: {
        MidiPotMapping p = getMidiAssignPotMapping(s_editIdx);
        p.cc = (uint8_t)s_editValue;
        setMidiAssignPotMapping(s_editIdx, p);

        p = getMidiAssignPotMapping(s_editIdx);
        s_editValue = p.max;
        goTo(SCREEN_POT_EDIT_RANGE, 0);
        break;
    }

    case SCREEN_POT_EDIT_RANGE: {
        MidiPotMapping p = getMidiAssignPotMapping(s_editIdx);
        p.min = 0;
        p.max = (uint8_t)s_editValue;
        setMidiAssignPotMapping(s_editIdx, p);

        uiDisplayRenderStatus("Saved", "OK");
        delay(300);
        goTo(SCREEN_POTS_MENU, (int)s_editIdx);
        break;
    }
#endif
    }
}

static void applyClicks(int clicks)
{
    if (clicks == 0)
        return;

    switch (s_screen) {
    case SCREEN_MAIN_MENU:
        // only 1 item
        s_cursor = 0;
        render();
        break;

    case SCREEN_MIDI_MENU:
        s_cursor += clicks;
        if (s_cursor < 0)
            s_cursor = s_midiMenuCount - 1;
        if (s_cursor >= s_midiMenuCount)
            s_cursor = 0;
        render();
        break;

#ifdef ENABLE_MIDI_ASSIGN_BUTTONS
    case SCREEN_BUTTONS_MENU: {
        const int count = BUTTONS_ASSIGN_NUMBER + 1;
        s_cursor += clicks;
        if (s_cursor < 0)
            s_cursor = count - 1;
        if (s_cursor >= count)
            s_cursor = 0;
        render();
        break;
    }

    case SCREEN_BTN_EDIT_CH:
        s_editValue += clicks;
        if (s_editValue < 1)
            s_editValue = 1;
        if (s_editValue > 16)
            s_editValue = 16;
        render();
        break;

    case SCREEN_BTN_EDIT_CC:
        s_editValue += clicks;
        if (s_editValue < 0)
            s_editValue = 0;
        if (s_editValue > 127)
            s_editValue = 127;
        render();
        break;

    case SCREEN_BTN_EDIT_MODE:
        s_editValue += clicks;
        if (s_editValue < 0)
            s_editValue = 0;
        if (s_editValue > 1)
            s_editValue = 1;
        render();
        break;
#endif

#ifdef ENABLE_MIDI_ASSIGN_POTS
    case SCREEN_POTS_MENU: {
        const int count = POTS_ASSIGN_NUMBER + 1;
        s_cursor += clicks;
        if (s_cursor < 0)
            s_cursor = count - 1;
        if (s_cursor >= count)
            s_cursor = 0;
        render();
        break;
    }

    case SCREEN_POT_EDIT_CH:
        s_editValue += clicks;
        if (s_editValue < 1)
            s_editValue = 1;
        if (s_editValue > 16)
            s_editValue = 16;
        render();
        break;

    case SCREEN_POT_EDIT_CC:
    case SCREEN_POT_EDIT_RANGE:
        s_editValue += clicks;
        if (s_editValue < 0)
            s_editValue = 0;
        if (s_editValue > 127)
            s_editValue = 127;
        render();
        break;
#endif
    }
}

void initUi()
{
    pinMode(UI_PIN_BACK_BTN, INPUT_PULLUP);
    pinMode(UI_PIN_ENC_CLK, INPUT_PULLUP);
    pinMode(UI_PIN_ENC_DT, INPUT_PULLUP);
    pinMode(UI_PIN_ENC_SW, INPUT_PULLUP);

    s_lastAB =
        ((digitalRead(UI_PIN_ENC_CLK) << 1) | digitalRead(UI_PIN_ENC_DT));
    attachInterrupt(digitalPinToInterrupt(UI_PIN_ENC_CLK), isrEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(UI_PIN_ENC_DT), isrEncoder, CHANGE);

    buildMidiMenu();
    buildLabels();

    uiDisplayBegin();
    goTo(SCREEN_MAIN_MENU, 0);
}

void updateUi()
{
    unsigned long now = millis();

    int clicks = consumeClicks();
    if (clicks != 0)
        applyClicks(clicks);

    if (pressedEdge(s_encSw, now))
        doEnter();
    if (pressedEdge(s_back, now))
        doBack();
}

#endif
