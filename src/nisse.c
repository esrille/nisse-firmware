/*
 * Copyright 2013-2025 Esrille Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "app.h"

typedef struct {
    uint8_t modifiers;
    uint8_t keycode;
} FN_KEY;

typedef struct {
    uint8_t row;
    uint8_t column;
} KEY_MAPPING;

static const uint8_t colPins[MATRIX_COLS] = {
    PORT_PIN_PA03, PORT_PIN_PA02, PORT_PIN_PA01, PORT_PIN_PA00, PORT_PIN_PA28, PORT_PIN_PA27,
    PORT_PIN_PA12, PORT_PIN_PA13, PORT_PIN_PA14, PORT_PIN_PA15, PORT_PIN_PA16, PORT_PIN_PA17
};
static const uint8_t rowPins[MATRIX_ROWS] = {
    PORT_PIN_PB10, PORT_PIN_PA10, PORT_PIN_PA09, PORT_PIN_PA08, PORT_PIN_PA07, PORT_PIN_PA06, PORT_PIN_PA05, PORT_PIN_PA04
};
static const uint8_t ledPins[] = {PORT_PIN_PA18, PORT_PIN_PA19, PORT_PIN_PA20};

static const uint8_t initialProfileData[PROFILE_DATA_SIZE] = {
    BASE_QWERTY,
    KANA_ROMAJI,
    OS_PC,
    DELAY_DEFAULT,
    MOD_DEFAULT,
    INDICATOR_DEFAULT,
    IME_MS,
    PAD_SENSE_1,
    PREFIXSHIFT_OFF
};

static const uint8_t marixNumLock[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEYPAD_DOT},
    {0, 0, 0, 0, 0, 0, KEY_CALC, KEY_TAB, KEYPAD_DIVIDE, KEYPAD_MULTIPLY, KEY_BACKSPACE, 0},
    {0, 0, 0, 0, 0, 0, 0, KEYPAD_7, KEYPAD_8, KEYPAD_9, KEYPAD_SUBTRACT, KEYPAD_0},
    {0, 0, 0, 0, 0, 0, 0, KEYPAD_4, KEYPAD_5, KEYPAD_6, KEYPAD_ADD, 0},
    {0, 0, 0, 0, 0, 0, 0, KEYPAD_1, KEYPAD_2, KEYPAD_3, KEYPAD_ENTER, 0},
};

// An unused key position in matrixAnsi
#define ROW_UNUSED      1
#define COLUMN_UNUSED   2

static const uint8_t matrixAnsi[MATRIX_ROWS][MATRIX_COLS] = {
    {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
    {KEY_CAPS_LOCK, KEY_LEFT_BRACKET, 0, 0, 0, 0, 0, 0, 0, 0, KEY_EQUAL, KEY_APOSTROPHE},
    {KEY_LEFT_CONTROL, KEY_RIGHT_BRACKET, 0, 0, 0, KEY_ESCAPE, KEY_APPLICATION, 0, 0, 0, KEY_MINUS, KEY_RIGHT_CONTROL},
    {KEY_LEFT_GUI, KEY_GRAVE, 0, 0, 0, KEY_TAB, KEY_ENTER, 0, 0, 0, KEY_BACKSLASH, KEY_RIGHT_GUI},
    {KEY_LEFT_FN, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_RIGHT_FN},
    {KEY_BACKSPACE, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_SPACE},
    {KEY_LEFT_SHIFT, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_RIGHT_SHIFT},
    {KEY_LEFT_ALT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_RIGHT_ALT},
};

static const uint8_t matrixDvorak[MATRIX_ROWS][MATRIX_COLS] = {
    {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
    {KEY_CAPS_LOCK, KEY_LEFT_BRACKET, 0, 0, 0, 0, 0, 0, 0, 0, KEY_BACKSLASH, KEY_MINUS},
    {KEY_LEFT_CONTROL, KEY_RIGHT_BRACKET, 0, 0, 0, KEY_ESCAPE, KEY_APPLICATION, 0, 0, 0, KEY_SLASH, KEY_RIGHT_CONTROL},
    {KEY_LEFT_GUI, KEY_GRAVE, 0, 0, 0, KEY_TAB, KEY_ENTER, 0, 0, 0, KEY_EQUAL, KEY_RIGHT_GUI},
    {KEY_LEFT_FN, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_RIGHT_FN},
    {KEY_BACKSPACE, KEY_APOSTROPHE, KEY_COMMA, KEY_PERIOD, KEY_P, KEY_Y, KEY_F, KEY_G, KEY_C, KEY_R, KEY_L, KEY_SPACE},
    {KEY_LEFT_SHIFT, KEY_A, KEY_O, KEY_E, KEY_U, KEY_I, KEY_D, KEY_H, KEY_T, KEY_N, KEY_S, KEY_RIGHT_SHIFT},
    {KEY_LEFT_ALT, KEY_SEMICOLON, KEY_Q, KEY_J, KEY_K, KEY_X, KEY_B, KEY_M, KEY_W, KEY_V, KEY_Z, KEY_RIGHT_ALT},
};

static const uint8_t matrixColemak[MATRIX_ROWS][MATRIX_COLS] = {
    {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
    {KEY_CAPS_LOCK, KEY_LEFT_BRACKET, 0, 0, 0, 0, 0, 0, 0, 0, KEY_EQUAL, KEY_APOSTROPHE},
    {KEY_LEFT_CONTROL, KEY_RIGHT_BRACKET, 0, 0, 0, KEY_ESCAPE, KEY_APPLICATION, 0, 0, 0, KEY_MINUS, KEY_RIGHT_CONTROL},
    {KEY_LEFT_GUI, KEY_GRAVE, 0, 0, 0, KEY_TAB, KEY_ENTER, 0, 0, 0, KEY_BACKSLASH, KEY_RIGHT_GUI},
    {KEY_LEFT_FN, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_RIGHT_FN},
    {KEY_BACKSPACE, KEY_Q, KEY_W, KEY_F, KEY_P, KEY_G, KEY_J, KEY_L, KEY_U, KEY_Y, KEY_SEMICOLON, KEY_SPACE},
    {KEY_LEFT_SHIFT, KEY_A, KEY_R, KEY_S, KEY_T, KEY_D, KEY_H, KEY_N, KEY_E, KEY_I, KEY_O, KEY_RIGHT_SHIFT},
    {KEY_LEFT_ALT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_K, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_RIGHT_ALT},
};

static const uint8_t matrixColemakDHM[MATRIX_ROWS][MATRIX_COLS] = {
    {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
    {KEY_CAPS_LOCK, KEY_LEFT_BRACKET, 0, 0, 0, 0, 0, 0, 0, 0, KEY_EQUAL, KEY_APOSTROPHE},
    {KEY_LEFT_CONTROL, KEY_RIGHT_BRACKET, 0, 0, 0, KEY_ESCAPE, KEY_APPLICATION, 0, 0, 0, KEY_MINUS, KEY_RIGHT_CONTROL},
    {KEY_LEFT_GUI, KEY_GRAVE, 0, 0, 0, KEY_TAB, KEY_ENTER, 0, 0, 0, KEY_BACKSLASH, KEY_RIGHT_GUI},
    {KEY_LEFT_FN, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_RIGHT_FN},
    {KEY_BACKSPACE, KEY_Q, KEY_W, KEY_F, KEY_P, KEY_B, KEY_J, KEY_L, KEY_U, KEY_Y, KEY_SEMICOLON, KEY_SPACE},
    {KEY_LEFT_SHIFT, KEY_A, KEY_R, KEY_S, KEY_T, KEY_G, KEY_M, KEY_N, KEY_E, KEY_I, KEY_O, KEY_RIGHT_SHIFT},
    {KEY_LEFT_ALT, KEY_Z, KEY_X, KEY_C, KEY_D, KEY_V, KEY_K, KEY_H, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_RIGHT_ALT},
};

//
// Japanese layouts
//
// [{   KEY_RIGHT_BRACKET
// ]}   KEY_NON_US_HASH (NON_US_HASH)
// \|   KEY_INTERNATIONAL3 (INTERNATIONAL3)
// @`   KEY_LEFT_BRACKET
// -=   KEY_MINUS
// :*   KEY_APOSTROPHE (QUOTE)
// ^~   KEY_EQUAL
//  _   KEY_INTERNATIONAL1 (INTERNATIONAL1)
// no-convert   KEY_INTERNATIONAL5 (INTERNATIONAL5)
// convert      KEY_INTERNATIONAL4 (INTERNATIONAL4)
// hiragana     KEY_INTERNATIONAL2 (INTERNATIONAL2)
// zenkaku      KEY_GRAVE
//

static const uint8_t matrixJIS[MATRIX_ROWS][MATRIX_COLS] = {
    {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
    {KEY_CAPS_LOCK, KEY_RIGHT_BRACKET, 0, 0, 0, 0, 0, 0, 0, 0, KEY_EQUAL, KEY_APOSTROPHE},
    {KEY_LEFT_CONTROL, KEY_NON_US_HASH, 0, 0, 0, KEY_ESCAPE, KEY_APPLICATION, 0, 0, 0, KEY_MINUS, KEY_RIGHT_CONTROL},
    {KEY_LEFT_GUI, KEY_INTERNATIONAL3, 0, 0, 0, KEY_TAB, KEY_ENTER, 0, 0, 0, KEY_LEFT_BRACKET, KEY_RIGHT_GUI},
    {KEY_LEFT_FN, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_RIGHT_FN},
    {KEY_BACKSPACE, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_SPACE},
    {KEY_LEFT_SHIFT, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_RIGHT_SHIFT},
    {KEY_LEFT_ALT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_RIGHT_ALT},
};

static const uint8_t matrixNicolaF[MATRIX_ROWS][MATRIX_COLS] = {
    {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
    {KEY_EQUAL, KEY_RIGHT_BRACKET, 0, 0, 0, 0, 0, 0, 0, 0, KEY_MINUS, KEY_BACKSPACE},
    {KEY_LEFT_CONTROL, KEY_NON_US_HASH, 0, 0, 0, KEY_ESCAPE, KEY_APPLICATION, 0, 0, 0, KEY_APOSTROPHE, KEY_RIGHT_CONTROL},
    {KEY_LEFT_GUI, KEY_INTERNATIONAL3, 0, 0, 0, KEY_TAB, KEY_ENTER, 0, 0, 0, KEY_LEFT_BRACKET, KEY_RIGHT_GUI},
    {KEY_LEFT_FN, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_RIGHT_FN},
    {KEYPAD_ENTER, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_SPACE},
    {KEY_LEFT_SHIFT, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_RIGHT_SHIFT},
    {KEY_LEFT_ALT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_RIGHT_ALT},
};

static const uint8_t (*const matrixes[BASE_MAX + 1])[MATRIX_COLS] = {
    matrixAnsi,
    matrixDvorak,
    matrixJIS,
    matrixNicolaF,
    matrixColemak,
    matrixColemakDHM,
};

static const FN_KEY matrixFN[MATRIX_ROWS][MATRIX_COLS] = {
    {{}, {}, {}, {}, {}, {},
     {}, {}, {}, {}, {}, {}},
    {{/*caps*/ 0, KEY_DELETE}, {}, {}, {}, {}, {},
     {}, {}, {}, {}, {}, {/*'*/ 0, KEYPAD_NUM_LOCK}},
    {{}, {/*]*/ MOD_CONTROL_LEFT | MOD_SHIFT_LEFT, KEY_Z}, {}, {}, {}, {},
     {/*app*/ 0, KEY_CAPS_LOCK}, {}, {}, {}, {/*-*/ 0, KEY_PRINT_SCREEN}, {}},
    {{}, {/*`*/ 0, KEY_INSERT}, {}, {}, {}, {},
     {}, {}, {}, {}, {/*\*/ 0, KEY_SCROLL_LOCK}, {}},
    {{}, {}, {}, {}, {}, {},
     {}, {}, {}, {}, {}, {}},
    {{}, {}, {}, {/*e*/ 0, KEY_PAGE_UP}, {}, {},
     {/*y*/ MOD_CONTROL_LEFT, KEY_HOME}, {/*u*/ MOD_CONTROL_LEFT, KEY_LEFT_ARROW}, {/*i*/ 0, KEY_UP_ARROW}, {/*o*/ MOD_CONTROL_LEFT, KEY_RIGHT_ARROW}, {/*p*/ MOD_CONTROL_LEFT, KEY_END}, {}},
    {{}, {}, {}, {/*d*/ 0, KEY_PAGE_DOWN}, {}, {},
     {/*h*/ 0, KEY_HOME}, {/*j*/ 0, KEY_LEFT_ARROW}, {/*k*/ 0, KEY_DOWN_ARROW}, {/*l*/ 0, KEY_RIGHT_ARROW}, {/*;*/ 0, KEY_END}, {}},
    {{}, {}, {}, {}, {}, {},
     {}, {/*m*/ MOD_SHIFT_LEFT, KEY_LEFT_ARROW}, {/*,*/ MOD_SHIFT_LEFT, KEY_DOWN_ARROW}, {/*.*/ MOD_SHIFT_LEFT, KEY_RIGHT_ARROW}, {/*/*/ MOD_SHIFT_LEFT, KEY_END}, {}}
};

static const uint16_t ccMap[12] = {
    // F1
    0,
    0,
    0,
    0,

    // F5
    0,
    0,
    0,
    0,

    // F9
    0,
    USB_HID_CONSUMER_MUTE,
    USB_HID_CONSUMER_VOLUME_DECREMENT,
    USB_HID_CONSUMER_VOLUME_INCREMENT
};

/* modifiers configurations */
static uint8_t const modMap[4][12] =
{
    // 0: -
    // 1: -
    // 2: CONTROL
    // 3: GUI
    // 4: FN
    // 5: BACKSPACE, SPACE
    // 6: SHIFT
    // 7: ALT
    {0, 1, 2, 3, 4, 5, 6, 7},   // XC
    {0, 1, 6, 3, 4, 5, 2, 7},   // XS
    {0, 1, 2, 3, 4, 6, 5, 7},   // C
    {0, 1, 6, 3, 4, 2, 5, 7},   // S
};

/* dual role FN keys */
static const FN_KEY imeKeys[OS_MAX + 1][2] = {
    {{0, 0} , {0, 0}},   // OS_PC
    {{0, KEY_LANG2}, {0, KEY_LANG1}},    // OS_MAC
    {{MOD_CONTROL_LEFT | MOD_SHIFT_LEFT, KEY_BACKSPACE}, {MOD_CONTROL_LEFT | MOD_SHIFT_LEFT, KEY_SPACE}}, // OS_104A
    {{MOD_ALT_LEFT, KEY_GRAVE}, {MOD_ALT_LEFT, KEY_GRAVE}}, // OS_104B
    {{0, KEY_INTERNATIONAL5}, {0, KEY_INTERNATIONAL4}}, // OS_109
    {{MOD_CONTROL_LEFT | MOD_SHIFT_LEFT, KEY_INTERNATIONAL5}, {MOD_CONTROL_LEFT | MOD_SHIFT_LEFT, KEY_INTERNATIONAL4}}, // OS_109A
    {{0, KEY_GRAVE}, {0, KEY_GRAVE}}, // OS_109B
    {{MOD_ALT_LEFT, KEY_SPACE}, {MOD_ALT_LEFT, KEY_SPACE}}, // OS_ALT_SP
    {{MOD_SHIFT_LEFT, KEY_SPACE}, {MOD_SHIFT_LEFT, KEY_SPACE}}, // OS_SHIFT_SP
    {{MOD_CONTROL_LEFT, KEY_SPACE}, {MOD_CONTROL_LEFT, KEY_SPACE}}, // OS_CTRL_SP
    {{0, KEY_CAPS_LOCK}, {0, KEY_CAPS_LOCK}}, // OS_CAPS
};

#define DELAY_SLOTS (DELAY_MAX +  2)

typedef struct {
    KEY_MAPPING keyMapping[256];

    int currentMap;
    uint16_t matrixBitmap[DELAY_SLOTS][MATRIX_ROWS];
    uint16_t matrixCurrent[MATRIX_ROWS];
    uint16_t matrixPrev[MATRIX_ROWS];
    uint8_t countRow[MATRIX_ROWS];
    uint8_t countCol[MATRIX_COLS];

    // keyboard
    int8_t xmit;
    uint8_t leds;
    uint8_t report[KEYBOARD_REPORT_LEN];
    uint8_t reportPrev[KEYBOARD_REPORT_LEN];
    bool enableLEDs;

    // cc
    uint16_t cc;
    uint16_t ccPrev;

    // language support (Japanese)
    bool kana;
    uint8_t prefix;
    uint8_t dualRoleFN;

    // profile
    uint8_t profile;
} NISSE_CONTROL;

static NISSE_CONTROL controller;

static void LED_Initialize(void)
{
    KEYBOARD_EnableLED(true);
    // Turn on all LEDs after power-up
    KEYBOARD_SetLEDs(LED_NUM_LOCK_BIT | LED_CAPS_LOCK_BIT | LED_SCROLL_LOCK_BIT);
}

void KEYBOARD_UpdateLEDs(void)
{
    uint8_t bits;

    if (!controller.enableLEDs) {
        bits = 0;
    } else {
        bits = controller.leds;
        switch (PROFILE_Read(EEPROM_INDICATOR)) {
        case INDICATOR_LEFT:
            bits &= ~LED_NUM_LOCK_BIT;
            if (controller.kana) {
                bits |= LED_NUM_LOCK_BIT;
            }
            break;
        case INDICATOR_CENTER:
            bits &= ~LED_CAPS_LOCK_BIT;
            if (controller.kana) {
                bits |= LED_CAPS_LOCK_BIT;
            }
            break;
        case INDICATOR_RIGHT:
            bits &= ~LED_SCROLL_LOCK_BIT;
            if (controller.kana) {
                bits |= LED_SCROLL_LOCK_BIT;
            }
            break;
        case INDICATOR_LEFT_NUM:
            if (controller.kana) {
                bits |= LED_NUM_LOCK_BIT;
            }
            break;
        case INDICATOR_CENTER_CAPS:
            if (controller.kana) {
                bits |= LED_CAPS_LOCK_BIT;
            }
            break;
        case INDICATOR_RIGHT_SCROLL:
            if (controller.kana) {
                bits |= LED_SCROLL_LOCK_BIT;
            }
            break;
        default:
            break;
        }
        if (TSAP_IsTouched()) {
            bits |= LED_SCROLL_LOCK_BIT;
        }
        if (controller.kana && controller.prefix && PROFILE_Read(EEPROM_PREFIX) == PREFIXSHIFT_LED) {
            bits |= LED_SCROLL_LOCK_BIT;
        }
    }
    for (unsigned int i = 0; i < sizeof ledPins; ++i) {
        if (bits & (1u << i)) {
            PORT_PinClear(ledPins[i]);
        } else {
            PORT_PinSet(ledPins[i]);
        }
    }
}

void KEYBOARD_EnableLED(bool enable)
{
    controller.enableLEDs = enable;
}

void KEYBOARD_SetLED(uint8_t led, bool on)
{
    uint8_t bit = 1u << (led - 1);
    if (on) {
        controller.leds |= bit;
    } else {
        controller.leds &= ~bit;
    }
}

void KEYBOARD_SetLEDs(uint8_t bits)
{
    controller.leds &= ~0x3f;
    controller.leds |= (bits & 0x3f);
}

bool KEYBOARD_ScanMatrix(void)
{
    bool wakeup = false;

    ++controller.currentMap;
    if (DELAY_SLOTS <= controller.currentMap) {
        controller.currentMap = 0;
    }
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        unsigned int rowPin = rowPins[i];
        PORT_PinOutputEnable(rowPin);   // Set to low
        controller.matrixBitmap[controller.currentMap][i] = 0;
        for (int j = 0; j < MATRIX_COLS; ++j) {
            uint8_t colPin = colPins[j];
            bool pressed = !PORT_PinRead(colPin);
            if (pressed) {
                controller.matrixBitmap[controller.currentMap][i] |= (1u << j);
                ++controller.countCol[j];
                ++controller.countRow[i];
                wakeup = true;
            }
        }
        PORT_PinInputEnable(rowPin);    // Set Hi-Z
    }
    return wakeup;
}

bool KEYBOARD_IsRawKeyPressed(uint8_t key)
{
    uint8_t row = controller.keyMapping[key].row;
    uint8_t col = controller.keyMapping[key].column;
    return controller.matrixBitmap[controller.currentMap][row] & (1u << col);
}

static bool DetectGhost(void)
{
    uint8_t rx = 0;
    uint8_t cx = 0;

    for (int i = 0; i < MATRIX_ROWS; ++i) {
        if (2 <= controller.countRow[i]) {
            ++rx;
        }
    }
    for (int i = 0; i < MATRIX_COLS; ++i) {
        if (2 <= controller.countCol[i]) {
            ++cx;
        }
    }
    memset(controller.countRow, 0, MATRIX_ROWS);
    memset(controller.countCol, 0, MATRIX_COLS);
    if (2 <= rx && 2 <= cx) {
        --controller.currentMap;
        if (controller.currentMap < 0) {
            controller.currentMap = DELAY_SLOTS - 1;
        }
        return true;
    }
    return false;
}

static bool IsShiftSwitch(int row, int column)
{
    if (column != 0 && column != 11) {
        return false;
    }
    const uint8_t mod = PROFILE_Read(EEPROM_MOD) % 4;
    switch (mod) {
    case MOD_S:
        return row == 2;
    case MOD_C:
        return row == 5;
    case MOD_XS:
        return row == 2;
    case MOD_XC:
    default:
        return row == 6;
    }
}

bool KEYBOARD_ProcessMatrix(void) {
    if (DetectGhost()) {
        return false;
    }
    int current = controller.currentMap;
    int prev = (current <= 0) ? (DELAY_SLOTS - 1) : (current - 1);
    int delayedCurrent = current - PROFILE_Read(EEPROM_DELAY);
    if (delayedCurrent < 0) {
        delayedCurrent += DELAY_SLOTS;
    }
    int delayedPrev = (delayedCurrent <= 0) ? (DELAY_SLOTS - 1) : (delayedCurrent - 1);
    for (int row = 0; row < MATRIX_ROWS; ++row) {
        controller.matrixPrev[row] = controller.matrixCurrent[row];
        for (int col = 0; col < MATRIX_COLS; ++col) {
            uint16_t bit = 1u << col;
            if (IsShiftSwitch(row, col)) {
                if ((controller.matrixBitmap[current][row] & bit) && (controller.matrixBitmap[prev][row] & bit)) {
                    controller.matrixCurrent[row] |= bit;
                } else if (!(controller.matrixBitmap[current][row] & bit) && !(controller.matrixBitmap[prev][row] & bit)) {
                    controller.matrixCurrent[row] &= ~bit;
                }
            } else {
                if ((controller.matrixBitmap[delayedCurrent][row] & bit) && (controller.matrixBitmap[delayedPrev][row] & bit)) {
                    controller.matrixCurrent[row] |= bit;
                } else if (!(controller.matrixBitmap[delayedCurrent][row] & bit) && !(controller.matrixBitmap[delayedPrev][row] & bit)) {
                    controller.matrixCurrent[row] &= ~bit;
                }
            }
        }
    }
    return true;
}

bool KEYBOARD_IsPressed(int row, int col)
{
    return controller.matrixCurrent[row] & (1u << col);
}

bool KEYBOARD_IsKeyPressed(uint8_t key)
{
    return KEYBOARD_IsPressed(controller.keyMapping[key].row, controller.keyMapping[key].column);
}

bool KEYBOARD_IsMake(int row, int col)
{
    const uint16_t bit = (1u << col);
    return (controller.matrixCurrent[row] & bit) && !(controller.matrixPrev[row] & bit);
}

static bool IsFnLayer(void)
{
    return KEYBOARD_IsPressed(4, 0) || KEYBOARD_IsPressed(4, 11);
}

static bool IsFnShiftLayer(void)
{
    return KEYBOARD_IsPressed(2, 0) || KEYBOARD_IsPressed(2, 11) ||
           KEYBOARD_IsPressed(5, 0) || KEYBOARD_IsPressed(5, 11) ||
           KEYBOARD_IsPressed(6, 0) || KEYBOARD_IsPressed(6, 11);
}

int8_t KEYBOARD_Get10KeyKeycode(int row, int col)
{
    uint8_t keycode = marixNumLock[row][col];

    // During Bluetooth passkey entry, LEDs indicate connection status.
    if (HOS_GetIndication() == HOS_BLE_STATE_BONDING) {
        // Accept passkey input via 10-key emulation regardless of Num Lock LED status.
        if (KEYPAD_1 <= keycode && keycode <= KEYPAD_0) {
            keycode -= KEYPAD_1;
            keycode += KEY_1;
            return keycode;
        }
        if (KEYPAD_ENTER == keycode) {
            return KEY_ENTER;
        }
    } else if (controller.leds & LED_NUM_LOCK_BIT) {
        return keycode;
    }
    return 0;
}

int8_t KEYBOARD_GetKeycode(int row, int col)
{
    const uint8_t (*const matrix)[MATRIX_COLS] = matrixes[PROFILE_Read(EEPROM_BASE)];
    uint8_t layout = PROFILE_Read(EEPROM_MOD);
    uint8_t keycode;

    // Check the 10 keys before others
    keycode = KEYBOARD_Get10KeyKeycode(row, col);
    if (keycode) {
        return keycode;
    }

    if (col == 0 || col == MATRIX_COLS - 1) {
        keycode = matrix[modMap[layout % 4][row]][col];
    } else {
        keycode = matrix[row][col];
    }
    if (KEYBOARD_IsMacMod(layout)) {
        switch (keycode) {
        case KEY_RIGHT_ALT:
            keycode = KEY_LANG1;
            break;
        case KEY_LEFT_ALT:
            keycode = KEY_LANG2;
            break;
        case KEY_APPLICATION:
            keycode = KEY_LEFT_ALT;
            break;
        default:
            break;
        }
    } else if (KEYBOARD_IsJapaneseMod(layout)) {
        switch (keycode) {
        case KEY_RIGHT_ALT:
            keycode = KEY_LANG1;
            break;
        case KEY_LEFT_ALT:
            keycode = KEY_LANG2;
            break;
        case KEY_LEFT_GUI:
            keycode = KEY_LEFT_ALT;
            break;
        default:
            break;
        }
    }
    return keycode;
}

static uint8_t GetModifiers(void)
{
    uint8_t mod = 0;

    for (int row = 0; row < MATRIX_ROWS; ++row) {
        for (int col = 0; col < MATRIX_COLS; col += MATRIX_COLS - 1) {
            if (KEYBOARD_IsPressed(row, col)) {
                uint8_t keycode = KEYBOARD_GetKeycode(row, col);
                if (KEYBOARD_IsModifier(keycode)) {
                    uint8_t bit = 1u << (keycode - KEY_LEFT_CONTROL);
                    mod |= bit;
                    if ((bit & MOD_SHIFT) && KEYBOARD_IsMake(row, col)) {
                        controller.prefix ^= bit;
                    }
                }
            }
        }
    }
    return mod;
}

static int8_t GetReport(uint8_t *buf, size_t bufLen, uint16_t *cc) {
    size_t currentReportByte = 2;
    const uint8_t (*matrix)[MATRIX_COLS] = matrixes[PROFILE_Read(EEPROM_BASE)];
    uint8_t mod = GetModifiers();

    memset(buf, 0, bufLen);
    *cc = 0;
    if (IsFnLayer()) {

        // dual role FN keys
        if (KEYBOARD_IsMake(4, 0)) {
            controller.dualRoleFN |= 1;
        }
        if (KEYBOARD_IsMake(4, 11)) {
            controller.dualRoleFN |= 2;
        }

        for (int i = 0; i < MATRIX_ROWS; i++) {
            for (int j = 0; j < MATRIX_COLS; j++) {
                if (KEYBOARD_IsPressed(i, j)) {
                    uint8_t keycode = matrix[i][j];
                    if (!keycode)
                        continue;
                    if (matrixFN[i][j].keycode) {
                        buf[0] |= matrixFN[i][j].modifiers;
                        if (currentReportByte < bufLen) {
                            buf[currentReportByte++] = matrixFN[i][j].keycode;
                        }
                    } else if (KEY_F1 <= keycode && keycode <= KEY_F12) {
                        if (IsFnShiftLayer()) {
                            if (KEY_F1 <= keycode && keycode <= KEY_F4) {
                                uint8_t profile = 0;
                                if (keycode < KEY_F4) {
                                    profile = (keycode - KEY_F1) + 1;
                                }
                                if (profile != controller.profile) {
                                    // Send a break before resetting the hardware
                                    controller.profile = profile;
                                    KEYBOARD_EnableLED(true);
                                    memset(buf, 0, KEYBOARD_REPORT_LEN);
                                    controller.dualRoleFN = 0;
                                    return XMIT_NORMAL;
                                }
                            }
                        } else if (ccMap[keycode - KEY_F1]) {
                            *cc = ccMap[keycode - KEY_F1];
                        } else if (KEYBOARD_IsMake(i, j) && KEYBOARD_GetFnReport(keycode)) {
                            MACRO_Begin(MAX_MACRO_SIZE);
                            controller.dualRoleFN = 0;
                            return XMIT_IN_ORDER;
                        }
                    } else if (keycode == KEY_ESCAPE) {
                        if (IsFnShiftLayer()) {
                            if (!PROFILE_IsUSBMode()) {
                                HOS_SetEvent(HOS_TYPE_INFO, HOS_EVENT_CLEAR_BONDING_DATA);
                                buf[0] &= ~MOD_SHIFT;
                                return XMIT_BRK;
                            }
                        }
                    } else if (keycode != KEY_LEFT_FN && keycode != KEY_RIGHT_FN) {
                        if (KEYBOARD_IsModifier(keycode)) {
                            buf[0] |= 1u << (keycode - KEY_LEFT_CONTROL);
                        } else if (currentReportByte < bufLen) {
                            buf[0] |= MOD_CONTROL_LEFT;
                            buf[currentReportByte++] = keycode;
                        }
                    }
                }
            }
        }
        if (buf[0] || buf[2]) {
            controller.dualRoleFN = 0;
        }
        return XMIT_NORMAL;
    }

    if (!controller.kana || (mod & (MOD_CONTROL | MOD_ALT | MOD_GUI)) || PROFILE_Read(EEPROM_KANA) == KANA_ROMAJI) {
        // Normal layer
        for (int row = 0; row < MATRIX_ROWS; ++row) {
            for (int col = 0; col < MATRIX_COLS; ++col) {
                uint8_t keycode = 0;

                if (!KEYBOARD_IsPressed(row, col)) {
                    continue;
                }
                keycode = KEYBOARD_GetKeycode(row, col);
                if (!keycode) {
                    continue;
                }
                if (KEYBOARD_IsModifier(keycode)) {
                    buf[0] |= 1u << (keycode - KEY_LEFT_CONTROL);
                } else if (currentReportByte < bufLen) {
                    buf[currentReportByte++] = keycode;
                }
            }
        }
    } else {
        if (PROFILE_Read(EEPROM_PREFIX)) {
            mod |= controller.prefix;
        }
        int8_t xmit = KEYBOARD_GetKanaReport(buf, bufLen, mod);
        switch (xmit) {
        case XMIT_MACRO:
            controller.prefix = 0;
            MACRO_Begin(MAX_MACRO_SIZE);
            return XMIT_IN_ORDER;
        case XMIT_NORMAL:
            while (currentReportByte < bufLen && buf[currentReportByte]) {
                ++currentReportByte;
            }
            break;
        default:
            buf[0] &= ~MOD_SHIFT;
            break;
        }
    }

    // Check Shift-0 in JIS keyboard
    uint8_t baseLayer = PROFILE_Read(EEPROM_BASE);
    uint8_t* zero = memchr(buf + 2, KEY_0, 6);
    if ((baseLayer == BASE_JIS || baseLayer == BASE_NICOLA_F) && (buf[0] & MOD_SHIFT) && zero) {
        *zero = KEY_INTERNATIONAL1;
    }

    // Check dual role FN keys for IME on/off
    if (controller.dualRoleFN) {
        if (PROFILE_Read(EEPROM_OS) != OS_PC && !KEYBOARD_IsJapaneseMod(PROFILE_Read(EEPROM_MOD)) && !buf[0] && !buf[2]) {
            buf[2] = (controller.dualRoleFN & 1) ? KEY_LANG2 : KEY_LANG1;
        }
        controller.dualRoleFN = 0;
    }

    if (buf[2]) {
        // Reset prefix shift
        controller.prefix = 0;
    }

    return XMIT_NORMAL;
}

void KEYBOARD_Initialize(void)
{
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t key = matrixAnsi[row][col];

            if (key != 0) {
                controller.keyMapping[key] = (KEY_MAPPING){
                    .row = row,
                    .column = col
                };
            } else {
                controller.keyMapping[key] = (KEY_MAPPING){
                    .row = ROW_UNUSED,
                    .column = COLUMN_UNUSED
                };
            }
        }
    }

    controller.currentMap = 0;
    controller.xmit = XMIT_NORMAL;
    controller.kana = false;
    controller.ccPrev = USB_HID_CONSUMER_CONSUMER_CONTROL;
    controller.dualRoleFN = 0;
    controller.leds = 0;

    PROFILE_Initialize(initialProfileData);
    controller.profile = PROFILE_GetCurrent();

    // Initialize key matrix:
    for (int i = 0; i < MATRIX_COLS; ++i) {
        // Set as inputs with pull-up registers
        uint8_t pin = colPins[i];
        PORT_PinGPIOConfig(pin);
        PORT_PinInputEnable(pin);
        PORT_PinPullEnable(pin);
        PORT_PinWrite(pin, true);   // pull up
    }
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        // Set to Hi-Z
        unsigned int pin = rowPins[i];
        PORT_PinGPIOConfig(pin);
        PORT_PinInputEnable(pin);   // no pull up
        PORT_PinWrite(pin, false);
    }

    LED_Initialize();
    HOS_Initialize();
}

bool KEYBOARD_Task(void)
{
    KEYBOARD_UpdateLEDs();
    if (PROFILE_GetCurrent() != controller.profile) {
        PROFILE_Select(controller.profile);
        NVIC_SystemReset();
        // NOT REACHED HERE
    }
    if (controller.xmit != XMIT_IN_ORDER) {
        KEYBOARD_ScanMatrix();
        if (!KEYBOARD_ProcessMatrix()) {
            return false;
        }
        controller.xmit = GetReport(controller.report, KEYBOARD_REPORT_LEN, &controller.cc);
        if (controller.xmit == XMIT_NORMAL && TSAP_IsTouched()) {
            memset(controller.report + 2, 0, KEYBOARD_REPORT_LEN - 2);
        }
    }
    return true;
}

bool KEYBOARD_GetMacroReport(uint8_t* preport) {
    if (controller.xmit != XMIT_IN_ORDER) {
        return false;
    }
    memmove(controller.reportPrev, controller.report, KEYBOARD_REPORT_LEN);
    uint8_t key = MACRO_Peek();
    uint8_t mod = 0;
    if (KEYBOARD_IsModifier(key)) {
        mod = 1u << (key - KEY_LEFT_CONTROL);
        MACRO_Get();
        key = MACRO_Peek();
    }
    if (controller.reportPrev[2] && controller.report[2] == key) {
        // break the current key press
        controller.report[0] = 0;
        controller.report[2] = 0;
    } else {
        MACRO_Get();
        controller.report[0] = mod;
        controller.report[2] = key;
        if (!key)
            controller.xmit = XMIT_NORMAL;
    }
    memmove(preport, controller.report, KEYBOARD_REPORT_LEN);
    return true;
}

static void ToggleKanaMode(uint8_t* preport)
{
    const uint8_t lang_keys[2] = {
        KEY_LANG2,  // off
        KEY_LANG1   // on
    };
    uint8_t os = PROFILE_Read(EEPROM_OS);

    if (os == OS_PC) {
        return;
    }
    for (int i = 0; i < 2; ++i) {
        uint8_t* lang = memchr(preport + 2, lang_keys[i], 6);
        if (!lang) {
            continue;
        }
        controller.kana = (i == 1);
        const FN_KEY* fn = &imeKeys[os][i];
        if (os != OS_CAPS) {
            *lang = fn->keycode;
            preport[0] |= fn->modifiers;
            continue;
        }
        // OS_CAPS special handling
        bool caps_on = (controller.leds & LED_CAPS_LOCK_BIT) != 0;
        if ((i == 0 && caps_on) || (i == 1 && !caps_on)) {
            *lang = KEY_CAPS_LOCK;
        } else {
            memmove(lang, lang + 1, 7 - (lang - preport));
        }
    }
}

bool KEYBOARD_GetReport(uint8_t* preport)
{
    if (controller.xmit == XMIT_IN_ORDER) {
        uint8_t key = MACRO_Peek();
        uint8_t mod = 0;

        if (KEYBOARD_IsModifier(key)) {
            mod = 1u << (key - KEY_LEFT_CONTROL);
            MACRO_Get();
            key = MACRO_Peek();
        }
        if (controller.reportPrev[2] && controller.report[2] == key) {
            // break the current key press
            controller.report[0] = 0;
            controller.report[2] = 0;
        } else {
            MACRO_Get();
            controller.report[0] = mod;
            controller.report[2] = key;
            if (!key) {
                controller.xmit = XMIT_NORMAL;
            }
        }
    }
    if (memcmp(controller.reportPrev, controller.report, KEYBOARD_REPORT_LEN)) {
        memmove(controller.reportPrev, controller.report, KEYBOARD_REPORT_LEN);
        memmove(preport, controller.report, KEYBOARD_REPORT_LEN);
        ToggleKanaMode(preport);
        return true;
    }
    return false;
}

bool KEYBOARD_GetConsumerReport(uint8_t* preport)
{
    if (controller.cc != controller.ccPrev) {
        // Both USB and SAM use little endian.
        memmove(preport, &controller.cc, CONSUMER_REPORT_LEN);
        controller.ccPrev = controller.cc;
        return true;
    }
    return false;
}

uint8_t KEYBOARD_GetBoardRevision(void)
{
    uint8_t rev = *(uint8_t*) USER_BOARD_REVISION;

    if (rev == 0xff) {
        rev = DEFAULT_BOARD_REVISION;
    }
    return rev;
}
