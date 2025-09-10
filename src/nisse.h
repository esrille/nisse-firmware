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

#ifndef ESRILLE_NISSE_H
#define ESRILLE_NISSE_H

#define FIRMWARE_VERSION_MAJOR          2
#define FIRMWARE_VERSION_MINOR          1
#define FIRMWARE_VERSION_REVISION       2

#define PNP_VID             0x04D8
#define PNP_PID             0xF550      // Esrille New Keyboard
#define PNP_VERSION         ((FIRMWARE_VERSION_MAJOR << 8) | (FIRMWARE_VERSION_MINOR << 4) | FIRMWARE_VERSION_REVISION)

#define MATRIX_COLS         12
#define MATRIX_ROWS         8

#define XMIT_NONE           0
#define XMIT_NORMAL         1
#define XMIT_BRK            2
#define XMIT_IN_ORDER       3
#define XMIT_MACRO          4

#define EEPROM_BASE         0
#define EEPROM_KANA         1
#define EEPROM_OS           2
#define EEPROM_DELAY        3
#define EEPROM_MOD          4
#define EEPROM_INDICATOR    5
#define EEPROM_IME          6
#define EEPROM_MOUSE        7
#define EEPROM_PREFIX       8

#define BASE_QWERTY         0
#define BASE_DVORAK         1
#define BASE_JIS            2
#define BASE_NICOLA_F       3
#define BASE_COLEMAK        4
#define BASE_COLEMAK_DHM    5
#define BASE_MAX            5

static inline bool KEYBOARD_IsJapanese(uint8_t layout) {
    return layout == BASE_JIS || layout == BASE_NICOLA_F;
}

#define KANA_ROMAJI         0
#define KANA_NICOLA         1
#define KANA_TRON           2
#define KANA_STICKNEY       3
#define KANA_X6004          4
#define KANA_MTYPE          5
#define KANA_NEW_STICKNEY   6
#define KANA_MAX            6

#define OS_PC               0   // No language keys
#define OS_MAC              1   // Kana / Eisuu
#define OS_104A             2   // Shift-Ctrl-Space / Shift-Ctlr-Backspace
#define OS_104B             3   // Alt-`
#define OS_109              4   // Henkan / Muhenkan
#define OS_109A             5   // Shift-Ctrl-Henkan / Shift-Ctlr-Muhenkan
#define OS_109B             6   // `
#define OS_ALT_SP           7   // Alt-Space
#define OS_SHIFT_SP         8   // Shift-Space
#define OS_CTRL_SP          9   // Ctrl-Space
#define OS_CAPS             10  // Caps Lock
#define OS_MAX              10

#define DELAY_0             0
#define DELAY_12            1
#define DELAY_24            2
#define DELAY_36            3
#define DELAY_48            4
#define DELAY_MAX           4
#define DELAY_DEFAULT       DELAY_0

#define MOD_XC              0
#define MOD_XS              1
#define MOD_C               2
#define MOD_S               3
#define MOD_XCJ             4
#define MOD_XSJ             5
#define MOD_CJ              6
#define MOD_SJ              7
#define MOD_XCJA            8   // Apple
#define MOD_XSJA            9   // Apple
#define MOD_CJA             10  // Apple
#define MOD_SJA             11  // Apple
#define MOD_MAX             11
#define MOD_DEFAULT         MOD_XC

static inline bool KEYBOARD_IsJapaneseMod(uint8_t mod) {
    return MOD_XCJ <= mod;
}

static inline bool KEYBOARD_IsMacMod(uint8_t mod) {
    return MOD_XCJA <= mod;
}

#define INDICATOR_LEFT              0   // kana, caps, scroll + tsap + prefix
#define INDICATOR_CENTER            1   // num, kana, caps + tsap + prefix
#define INDICATOR_RIGHT             2   // num, caps, kana + tsap + prefix
#define INDICATOR_LEFT_NUM          3   // num + kana, caps, scroll + tsap + prefix
#define INDICATOR_CENTER_CAPS       4   // num, caps + kana, scroll + tsap + prefix
#define INDICATOR_RIGHT_SCROLL      5   // num, caps, scroll + kana + tsap + prefix
#define INDICATOR_OFF               6   // num, caps, scroll + tsap + prefix
#define INDICATOR_MAX               6
#define INDICATOR_DEFAULT           INDICATOR_OFF

#define IME_MS              0
#define IME_ATOK            1
#define IME_GOOGLE          2
#define IME_APPLE           3
#define IME_MAX             3

#define IME_MS              0
#define IME_ATOK            1
#define IME_GOOGLE          2
#define IME_APPLE           3
#define IME_MAX             3

#define PAD_SENSE_1         0
#define PAD_SENSE_2         1
#define PAD_SENSE_3         2
#define PAD_SENSE_4         3
#define PAD_SENSE_MAX       3

#define PREFIXSHIFT_OFF     0
#define PREFIXSHIFT_ON      1
#define PREFIXSHIFT_LED     2
#define PREFIXSHIFT_MAX     2

//
// SAM D21 User Row Addresses
//
#define USER_BOARD_REVISION     (USER_PAGE_ADDR + 8)

#define DEFAULT_BOARD_REVISION  8   // initiial SAMD21 NISSE controller board

#endif  // ESRILLE_NISSE_H
