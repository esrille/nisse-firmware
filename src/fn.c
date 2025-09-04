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

#define MAX_OS_KEY_NAME     6

static uint8_t const osKeys[OS_MAX + 1][MAX_OS_KEY_NAME] = {
    {KEY_P, KEY_C, KEY_ENTER, 0},
    {KEY_M, KEY_A, KEY_C, KEY_ENTER, 0},
    {KEY_1, KEY_0, KEY_4, KEY_A, KEY_ENTER, 0},
    {KEY_1, KEY_0, KEY_4, KEY_B, KEY_ENTER, 0},
    {KEY_1, KEY_0, KEY_9, KEY_ENTER, 0},
    {KEY_1, KEY_0, KEY_9, KEY_A, KEY_ENTER, 0},
    {KEY_1, KEY_0, KEY_9, KEY_B, KEY_ENTER, 0},
    {KEY_A, KEY_MINUS, KEY_S, KEY_P, KEY_ENTER, 0},
    {KEY_S, KEY_MINUS, KEY_S, KEY_P, KEY_ENTER, 0},
    {KEY_C, KEY_MINUS, KEY_S, KEY_P, KEY_ENTER, 0},
    {KEY_C, KEY_A, KEY_P, KEY_S, KEY_ENTER, 0},
};

#define MAX_BASE_KEY_NAME   6

static uint8_t const baseKeys[BASE_MAX + 1][MAX_BASE_KEY_NAME] =
{
    {KEY_U, KEY_S, KEY_ENTER, 0},
    {KEY_U, KEY_S, KEY_MINUS, KEY_D, KEY_ENTER, 0},
    {KEY_J, KEY_P, KEY_ENTER, 0},
    {KEY_J, KEY_P, KEY_MINUS, KEY_N, KEY_ENTER, 0},
    {KEY_U, KEY_S, KEY_MINUS, KEY_C, KEY_ENTER, 0},
    {KEY_U, KEY_S, KEY_MINUS, KEY_M, KEY_ENTER, 0},
};

#define MAX_KANA_KEY_NAME   7

static uint8_t const kanaKeys[KANA_MAX + 1][MAX_KANA_KEY_NAME] =
{
    {KEY_R, KEY_O, KEY_M, KEY_A, KEY_ENTER, 0},
    {KEY_N, KEY_I, KEY_C, KEY_O, KEY_ENTER, 0},
    {KEY_T, KEY_R, KEY_O, KEY_N, KEY_ENTER, 0},
    {KEY_S, KEY_T, KEY_I, KEY_C, KEY_K, KEY_ENTER, 0},
    {KEY_X, KEY_6, KEY_0, KEY_0, KEY_4, KEY_ENTER, 0},
    {KEY_M, KEY_T, KEY_Y, KEY_P, KEY_E, KEY_ENTER, 0},
    {KEY_N, KEY_E, KEY_W, KEY_ENTER, 0},
};

#define MAX_DELAY_KEY_NAME  5

static uint8_t const delayKeys[DELAY_MAX + 1][MAX_DELAY_KEY_NAME] =
{
    {KEY_D, KEY_0, KEY_ENTER, 0},
    {KEY_D, KEY_1, KEY_2, KEY_ENTER, 0},
    {KEY_D, KEY_2, KEY_4, KEY_ENTER, 0},
    {KEY_D, KEY_3, KEY_6, KEY_ENTER, 0},
    {KEY_D, KEY_4, KEY_8, KEY_ENTER, 0},
};

#define MAX_MOD_KEY_NAME    6

static uint8_t const modKeys[MOD_MAX + 1][MAX_MOD_KEY_NAME] =
{
    {KEY_X, KEY_C, KEY_ENTER, 0},
    {KEY_X, KEY_S, KEY_ENTER, 0},
    {KEY_C, KEY_ENTER, 0},
    {KEY_S, KEY_ENTER, 0},
    {KEY_X, KEY_C, KEY_J, KEY_ENTER, 0},
    {KEY_X, KEY_S, KEY_J, KEY_ENTER, 0},
    {KEY_C, KEY_J, KEY_ENTER, 0},
    {KEY_S, KEY_J, KEY_ENTER, 0},
    {KEY_X, KEY_C, KEY_J, KEY_A, KEY_ENTER, 0},
    {KEY_X, KEY_S, KEY_J, KEY_A, KEY_ENTER, 0},
    {KEY_C, KEY_J, KEY_A, KEY_ENTER, 0},
    {KEY_S, KEY_J, KEY_A, KEY_ENTER, 0},
};

#define MAX_IME_KEY_NAME    6

static uint8_t const imeKeys[IME_MAX + 1][MAX_IME_KEY_NAME] =
{
    {KEY_M, KEY_S, KEY_ENTER, 0},
    {KEY_A, KEY_T, KEY_O, KEY_K, KEY_ENTER, 0},
    {KEY_G, KEY_O, KEY_O, KEY_G, KEY_ENTER, 0},
    {KEY_A, KEY_P, KEY_P, KEY_L, KEY_ENTER, 0},
};

#define MAX_INDICATOR_KEY_NAME  5

static uint8_t const indicatorKeys[INDICATOR_MAX + 1][MAX_INDICATOR_KEY_NAME] =
{
    {KEY_L, KEY_ENTER, 0},
    {KEY_C, KEY_ENTER, 0},
    {KEY_R, KEY_ENTER, 0},
    {KEY_L, KEY_MINUS, KEY_N, KEY_ENTER, 0},
    {KEY_C, KEY_MINUS, KEY_C, KEY_ENTER, 0},
    {KEY_R, KEY_MINUS, KEY_S, KEY_ENTER, 0},
    {KEY_O, KEY_F, KEY_F, KEY_ENTER, 0},
};

#define MAX_PREFIX_KEY_NAME 5

static uint8_t const prefixKeys[PREFIXSHIFT_MAX + 1][MAX_PREFIX_KEY_NAME] =
{
    {KEY_O, KEY_F, KEY_F, KEY_ENTER, 0},
    {KEY_O, KEY_N, KEY_ENTER, 0},
    {KEY_L, KEY_E, KEY_D, KEY_ENTER, 0},
};

static const uint8_t aboutTitle[] = {
    KEY_E, KEY_S, KEY_R, KEY_I, KEY_L, KEY_L, KEY_E, KEY_SPACE,
    KEY_N, KEY_I, KEY_S, KEY_S, KEY_E,
    KEY_ENTER, 0
};

static const uint8_t aboutCPU[] = {
    KEY_SPACE, KEY_MINUS, KEY_SPACE, KEY_S, KEY_A, KEY_M, KEY_D, KEY_2, KEY_1, 0
};

static const uint8_t aboutRevision[] = {
    KEY_R, KEY_E, KEY_V, KEY_PERIOD, KEY_SPACE, 0
};

static const uint8_t aboutVersion[] = {
    KEY_V, KEY_E, KEY_R, KEY_PERIOD, KEY_SPACE, 0
};

static const uint8_t aboutCopyright[] = {
    KEY_C, KEY_O, KEY_P, KEY_Y, KEY_R, KEY_I, KEY_G, KEY_H, KEY_T, KEY_SPACE, KEY_2, KEY_0, KEY_1, KEY_3, KEY_MINUS, KEY_2, KEY_0, KEY_2, KEY_5, KEY_SPACE,
    KEY_E, KEY_S, KEY_R, KEY_I, KEY_L, KEY_L, KEY_E, KEY_SPACE, KEY_I, KEY_N, KEY_C, KEY_PERIOD, KEY_ENTER, 0
};

static const uint8_t aboutF2[] = {
    KEY_F, KEY_2, KEY_SPACE, 0
};

static const uint8_t aboutF3[] = {
    KEY_F, KEY_3, KEY_SPACE, 0
};

static const uint8_t aboutF4[] = {
    KEY_F, KEY_4, KEY_SPACE, 0
};

static const uint8_t aboutF5[] = {
    KEY_F, KEY_5, KEY_SPACE, 0
};

static const uint8_t aboutF6[] = {
    KEY_F, KEY_6, KEY_SPACE, 0
};

static const uint8_t aboutF7[] = {
    KEY_F, KEY_7, KEY_SPACE, 0
};

static const uint8_t aboutF8[] = {
    KEY_F, KEY_8, KEY_SPACE, 0
};

static const uint8_t aboutF9[] = {
    KEY_F, KEY_9, KEY_SPACE, 0
};

static const uint8_t aboutTSAP[] = {
    KEY_T, KEY_S, KEY_A, KEY_P, KEY_SPACE, 0
};

static const uint8_t aboutBLE[] = {
    KEY_B, KEY_L, KEY_E, KEY_SPACE, KEY_M, KEY_O, KEY_D, KEY_U, KEY_L, KEY_E,
    KEY_ENTER, 0
};

static const uint8_t aboutKVM[] = {
    KEY_K, KEY_V, KEY_M, KEY_SPACE, 0
};

static const uint8_t aboutLESC[] = {
    KEY_L, KEY_E, KEY_S, KEY_C, KEY_SPACE, 0
};

static const uint8_t aboutVBus[] = {
    KEY_V, KEY_B, KEY_U, KEY_S, KEY_SPACE, 0
};

static void DoAbout(void) {
    bool usb_mode = !HOS_IsModuleInstalled();

    MACRO_Puts(aboutTitle);

    // REV.
    MACRO_Puts(aboutRevision);
    MACRO_PutNumber(KEYBOARD_GetBoardRevision());
    MACRO_Puts(aboutCPU);
    MACRO_Put(KEY_ENTER);

    // VER.
    MACRO_Puts(aboutVersion);
    MACRO_PutNumber(FIRMWARE_VERSION_MAJOR);
    MACRO_Put(KEY_PERIOD);
    MACRO_PutNumber(FIRMWARE_VERSION_MINOR);
    MACRO_Put(KEY_PERIOD);
    MACRO_PutNumber(FIRMWARE_VERSION_REVISION);
    MACRO_Put(KEY_ENTER);

    if (usb_mode) {
        MACRO_Puts(aboutCopyright);
    } else {
        MACRO_Puts(aboutBLE);
        MACRO_Puts(aboutRevision);
        MACRO_PutNumber(HOS_GetRevision() & 0xf);
        MACRO_Put(KEY_ENTER);

        MACRO_Puts(aboutVersion);
        MACRO_PutNumber((HOS_GetVersion() >> 8) & 0xf);
        MACRO_Put(KEY_PERIOD);
        MACRO_PutNumber((HOS_GetVersion() >> 4) & 0xf);
        MACRO_PutNumber(HOS_GetVersion() & 0xf);
        MACRO_Put(KEY_ENTER);

        MACRO_Puts(aboutCopyright);

        if (!PROFILE_IsUSBMode()) {
            MACRO_Puts(aboutLESC);
            MACRO_PutNumber(HOS_GetLESC());
            MACRO_Put(KEY_ENTER);
        }

        MACRO_Puts(aboutKVM);
        MACRO_PutNumber(PROFILE_GetCurrent());
        MACRO_Put(KEY_ENTER);

        // Battery
        uint16_t voltage = HOS_GetBatteryVoltage();
        uint8_t level = HOS_GetBatteryLevel();
        MACRO_PutNumber(voltage / 100);
        MACRO_Put(KEY_PERIOD);
        voltage %= 100;
        if (voltage < 10) {
            MACRO_Put(KEY_0);
        }
        MACRO_PutNumber(voltage % 100);
        MACRO_Put(KEY_V);
        MACRO_Put(KEY_SPACE);
        MACRO_PutNumber(level);
        MACRO_Put(KEY_LEFT_SHIFT);
        MACRO_Put(KEY_5);
        MACRO_Put(KEY_ENTER);

        // VBUS
        MACRO_Puts(aboutVBus);
        MACRO_PutNumber(USB_VBUS_SENSE_Get());
        MACRO_Put(KEY_ENTER);
    }

    // F2 OS
    MACRO_Puts(aboutF2);
    MACRO_Puts(osKeys[PROFILE_Read(EEPROM_OS)]);

    // F3 Layout
    MACRO_Puts(aboutF3);
    MACRO_Puts(baseKeys[PROFILE_Read(EEPROM_BASE)]);

    // F4 Kana Layout
    MACRO_Puts(aboutF4);
    MACRO_Puts(kanaKeys[PROFILE_Read(EEPROM_KANA)]);

    // F5 Delay
    MACRO_Puts(aboutF5);
    MACRO_Puts(delayKeys[PROFILE_Read(EEPROM_DELAY)]);

    // F6 Modifiers
    MACRO_Puts(aboutF6);
    MACRO_Puts(modKeys[PROFILE_Read(EEPROM_MOD)]);

    // F7 IME
    MACRO_Puts(aboutF7);
    MACRO_Puts(imeKeys[PROFILE_Read(EEPROM_IME)]);

    // F8 LED
    MACRO_Puts(aboutF8);
    MACRO_Puts(indicatorKeys[PROFILE_Read(EEPROM_INDICATOR)]);

    // F9 Prefix Shift
    MACRO_Puts(aboutF9);
    MACRO_Puts(prefixKeys[PROFILE_Read(EEPROM_PREFIX)]);

    if (APP_HasMouseInterface()) {
        MACRO_Puts(aboutTSAP);
        MACRO_PutNumber(1 + PROFILE_Read(EEPROM_MOUSE));
        MACRO_Put(KEY_ENTER);
    }
}

static uint8_t IncrementProfileSetting(uint8_t offset, uint8_t max) {
    uint8_t value = PROFILE_Read(offset);
    ++value;
    if (max < value) {
        value = 0;
    }
    PROFILE_Write(offset, value);
    return value;
}

int8_t KEYBOARD_GetFnReport(uint8_t keycode)
{
    uint8_t value;

    switch (keycode) {
    case KEY_F1:
        DoAbout();
        break;
    case KEY_F2:
        value = IncrementProfileSetting(EEPROM_OS, OS_MAX);
        MACRO_Puts(osKeys[value]);
        break;
    case KEY_F3:
        value = IncrementProfileSetting(EEPROM_BASE, BASE_MAX);
        MACRO_Puts(baseKeys[value]);
        break;
    case KEY_F4:
        value = IncrementProfileSetting(EEPROM_KANA, KANA_MAX);
        MACRO_Puts(kanaKeys[value]);
        break;
    case KEY_F5:
        value = IncrementProfileSetting(EEPROM_DELAY, DELAY_MAX);
        MACRO_Puts(delayKeys[value]);
        break;
    case KEY_F6:
        value = IncrementProfileSetting(EEPROM_MOD, MOD_MAX);
        MACRO_Puts(modKeys[value]);
        break;
    case KEY_F7:
        value = IncrementProfileSetting(EEPROM_IME, IME_MAX);
        MACRO_Puts(imeKeys[value]);
        break;
    case KEY_F8:
        value = IncrementProfileSetting(EEPROM_INDICATOR, INDICATOR_MAX);
        MACRO_Puts(indicatorKeys[value]);
        break;
    case KEY_F9:
        value = IncrementProfileSetting(EEPROM_PREFIX, PREFIXSHIFT_MAX);
        MACRO_Puts(prefixKeys[value]);
        break;
    default:
        return XMIT_NONE;
    }
    return XMIT_MACRO;
}
