/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI
controller Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

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

#ifdef ENABLE_UI

#if defined(UI_USE_OLED_128x64)
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
static Adafruit_SSD1306 g_display(128, 64, &Wire, UI_OLED_RESET_PIN);
#endif

static bool g_displayOk = false;

static void oledHeader(const char *title)
{
#if defined(UI_USE_OLED_128x64)
    g_display.setCursor(0, 0);
    g_display.setTextSize(1);
    g_display.print(title);
    g_display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
#endif
}

void uiDisplayBegin()
{
#if defined(UI_USE_OLED_128x64)
    Wire.begin();
    g_displayOk = g_display.begin(SSD1306_SWITCHCAPVCC, UI_OLED_ADDR);
    if (!g_displayOk)
        return;

    g_display.clearDisplay();
    g_display.setTextColor(SSD1306_WHITE);
    g_display.setCursor(0, 0);
    g_display.print("UI OK");
    g_display.display();
#endif
}

void uiDisplayClear()
{
#if defined(UI_USE_OLED_128x64)
    if (!g_displayOk)
        return;
    g_display.clearDisplay();
#endif
}

// ARROW BLOCK
// static void oledLine(int row, bool selected, const char* text) {
// #if defined(UI_USE_OLED_128x64)
//   int y = 14 + row * 12;
//   g_display.setCursor(0, y);
//   g_display.print(selected ? ">" : " ");
//   g_display.print(text);
// #endif
// }

// CONSTRAST BLOCK
static void oledLine(int row, bool selected, const char *text)
{
#if defined(UI_USE_OLED_128x64)
    int y = 14 + row * 12;

    if (selected) {
        // bloco de contraste
        g_display.fillRect(0, y - 1, 128, 11, SSD1306_WHITE);
        g_display.setTextColor(SSD1306_BLACK);
    } else {
        g_display.setTextColor(SSD1306_WHITE);
    }

    g_display.setCursor(2, y);
    g_display.print(text);

    g_display.setTextColor(SSD1306_WHITE);
#endif
}

void uiDisplayRenderMenu(const char *title, const char *const *items, int count,
                         int selected, int topIndex)
{
#if defined(UI_USE_OLED_128x64)
    if (!g_displayOk)
        return;

    g_display.clearDisplay();
    oledHeader(title);

    for (int row = 0; row < 4; row++) {
        int idx = topIndex + row;
        if (idx >= count)
            break;
        oledLine(row, idx == selected, items[idx]);
    }

    g_display.display();
#else
    (void)title;
    (void)items;
    (void)count;
    (void)selected;
    (void)topIndex;
#endif
}

void uiDisplayRenderEditInt(const char *title, const char *label, int value,
                            int minV, int maxV)
{
#if defined(UI_USE_OLED_128x64)
    if (!g_displayOk)
        return;

    g_display.clearDisplay();
    oledHeader(title);

    g_display.setCursor(0, 18);
    g_display.print(label);

    g_display.setCursor(0, 34);
    g_display.setTextSize(2);
    g_display.print(value);
    g_display.setTextSize(1);

    g_display.setCursor(0, 56);
    g_display.print(minV);
    g_display.print("..");
    g_display.print(maxV);

    g_display.display();
#else
    (void)title;
    (void)label;
    (void)value;
    (void)minV;
    (void)maxV;
#endif
}

void uiDisplayRenderEditChoice(const char *title, const char *label,
                               const char *choice)
{
#if defined(UI_USE_OLED_128x64)
    if (!g_displayOk)
        return;

    g_display.clearDisplay();
    oledHeader(title);

    g_display.setCursor(0, 18);
    g_display.print(label);

    g_display.setCursor(0, 34);
    g_display.setTextSize(2);
    g_display.print(choice);
    g_display.setTextSize(1);

    g_display.display();
#else
    (void)title;
    (void)label;
    (void)choice;
#endif
}

void uiDisplayRenderStatus(const char *title, const char *msg)
{
#if defined(UI_USE_OLED_128x64)
    if (!g_displayOk)
        return;

    g_display.clearDisplay();
    oledHeader(title);
    g_display.setCursor(0, 24);
    g_display.print(msg);
    g_display.display();
#else
    (void)title;
    (void)msg;
#endif
}

#endif
