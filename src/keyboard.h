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

#ifndef ESRILLE_KEYBOARD_H
#define ESRILLE_KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_REPORT_LEN     8
#define CONSUMER_REPORT_LEN     2

typedef struct
{
    uint8_t data[KEYBOARD_REPORT_LEN];
} KEYBOARD_INPUT_REPORT;

typedef struct
{
    uint8_t data[64];
} KEYBOARD_OUTPUT_REPORT;

typedef struct
{
    uint8_t data[CONSUMER_REPORT_LEN];
} CONSUMER_REPORT;

#define KEY_ERRORROLLOVER       0x01
#define KEY_POSTFAIL            0x02
#define KEY_ERRORUNDEFINED      0x03
#define KEY_A                   0x04
#define KEY_B                   0x05
#define KEY_C                   0x06
#define KEY_D                   0x07
#define KEY_E                   0x08
#define KEY_F                   0x09
#define KEY_G                   0x0A
#define KEY_H                   0x0B
#define KEY_I                   0x0C
#define KEY_J                   0x0D
#define KEY_K                   0x0E
#define KEY_L                   0x0F
#define KEY_M                   0x10
#define KEY_N                   0x11
#define KEY_O                   0x12
#define KEY_P                   0x13
#define KEY_Q                   0x14
#define KEY_R                   0x15
#define KEY_S                   0x16
#define KEY_T                   0x17
#define KEY_U                   0x18
#define KEY_V                   0x19
#define KEY_W                   0x1A
#define KEY_X                   0x1B
#define KEY_Y                   0x1C
#define KEY_Z                   0x1D
#define KEY_1                   0x1E
#define KEY_2                   0x1F
#define KEY_3                   0x20
#define KEY_4                   0x21
#define KEY_5                   0x22
#define KEY_6                   0x23
#define KEY_7                   0x24
#define KEY_8                   0x25
#define KEY_9                   0x26
#define KEY_0                   0x27
#define KEY_ENTER               0x28
#define KEY_ESCAPE              0x29
#define KEY_BACKSPACE           0x2A
#define KEY_TAB                 0x2B
#define KEY_SPACE               0x2C
#define KEY_MINUS               0x2D
#define KEY_EQUAL               0x2E
#define KEY_LEFT_BRACKET        0x2F
#define KEY_RIGHT_BRACKET       0x30
#define KEY_BACKSLASH           0x31
#define KEY_NON_US_HASH         0x32
#define KEY_SEMICOLON           0x33
#define KEY_APOSTROPHE          0x34
#define KEY_GRAVE               0x35
#define KEY_COMMA               0x36
#define KEY_PERIOD              0x37
#define KEY_SLASH               0x38
#define KEY_CAPS_LOCK           0x39
#define KEY_F1                  0x3A
#define KEY_F2                  0x3B
#define KEY_F3                  0x3C
#define KEY_F4                  0x3D
#define KEY_F5                  0x3E
#define KEY_F6                  0x3F
#define KEY_F7                  0x40
#define KEY_F8                  0x41
#define KEY_F9                  0x42
#define KEY_F10                 0x43
#define KEY_F11                 0x44
#define KEY_F12                 0x45
#define KEY_PRINT_SCREEN        0x46
#define KEY_SCROLL_LOCK         0x47
#define KEY_PAUSE               0x48
#define KEY_INSERT              0x49
#define KEY_HOME                0x4A
#define KEY_PAGE_UP             0x4B
#define KEY_DELETE              0x4C
#define KEY_END                 0x4D
#define KEY_PAGE_DOWN           0x4E
#define KEY_RIGHT_ARROW         0x4F
#define KEY_LEFT_ARROW          0x50
#define KEY_DOWN_ARROW          0x51
#define KEY_UP_ARROW            0x52
#define KEYPAD_NUM_LOCK         0x53
#define KEYPAD_DIVIDE           0x54
#define KEYPAD_MULTIPLY         0x55
#define KEYPAD_SUBTRACT         0x56
#define KEYPAD_ADD              0x57
#define KEYPAD_ENTER            0x58
#define KEYPAD_1                0x59
#define KEYPAD_2                0x5A
#define KEYPAD_3                0x5B
#define KEYPAD_4                0x5C
#define KEYPAD_5                0x5D
#define KEYPAD_6                0x5E
#define KEYPAD_7                0x5F
#define KEYPAD_8                0x60
#define KEYPAD_9                0x61
#define KEYPAD_0                0x62
#define KEYPAD_DOT              0x63
#define KEY_NON_US_BACKSLASH    0x64
#define KEY_APPLICATION         0x65
#define KEY_POWER               0x66
#define KEYPAD_EQUAL            0x67
#define KEY_F13                 0x68
#define KEY_F14                 0x69
#define KEY_F15                 0x6A
#define KEY_F16                 0x6B
#define KEY_F17                 0x6C
#define KEY_F18                 0x6D
#define KEY_F19                 0x6E
#define KEY_F20                 0x6F
#define KEY_F21                 0x70
#define KEY_F22                 0x71
#define KEY_F23                 0x72
#define KEY_F24                 0x73
#define KEY_EXECUTE             0x74
#define KEY_HELP                0x75
#define KEY_MENU                0x76
#define KEY_SELECT              0x77
#define KEY_STOP                0x78
#define KEY_AGAIN               0x79
#define KEY_UNDO                0x7A
#define KEY_CUT                 0x7B
#define KEY_COPY                0x7C
#define KEY_PASTE               0x7D
#define KEY_FIND                0x7E
#define KEY_MUTE                0x7F
#define KEY_VOLUME_UP           0x80
#define KEY_VOLUME_DOWN         0x81
#define KEY_LOCKING_CAPS_LOCK   0x82
#define KEY_LOCKING_NUM_LOCK    0x83
#define KEY_LOCKING_SCROLL_LOCK 0x84
#define KEYPAD_COMMA            0x85
#define KEYPAD_EQUAL_SIGN       0x86
#define KEY_INTERNATIONAL1      0x87
#define KEY_INTERNATIONAL2      0x88
#define KEY_INTERNATIONAL3      0x89
#define KEY_INTERNATIONAL4      0x8A
#define KEY_INTERNATIONAL5      0x8B
#define KEY_INTERNATIONAL6      0x8C
#define KEY_INTERNATIONAL7      0x8D
#define KEY_INTERNATIONAL8      0x8E
#define KEY_INTERNATIONAL9      0x8F
#define KEY_LANG1               0x90
#define KEY_LANG2               0x91
#define KEY_LANG3               0x92
#define KEY_LANG4               0x93
#define KEY_LANG5               0x94
#define KEY_LANG6               0x95
#define KEY_LANG7               0x96
#define KEY_LANG8               0x97
#define KEY_LANG9               0x98
#define KEY_ALTERNATE_ERASE     0x99
#define KEY_SYSREQ_ATTENTION    0x9A
#define KEY_CANCEL              0x9B
#define KEY_CLEAR               0x9C
#define KEY_PRIOR               0x9D
#define KEY_RETURN              0x9E
#define KEY_SEPARATOR           0x9F
#define KEY_OUT                 0xA0
#define KEY_OPER                0xA1
#define KEY_CLEAR_AGAIN         0xA2
#define KEY_CRSEL_PROPS         0xA3
#define KEY_EXSEL               0xA4
#define KEYPAD_00               0xB0
#define KEYPAD_000              0xB1
#define THOUSANDS_SEPARATOR     0xB2
#define DECIMAL_SEPARATOR       0xB3
#define CURRENCY_UNIT           0xB4
#define CURRENCY_SUB_UNIT       0xB5
#define KEYPAD_LEFT_PAREN       0xB6
#define KEYPAD_RIGHT_PAREN      0xB7
#define KEYPAD_LEFT_BRACE       0xB8
#define KEYPAD_RIGHT_BRACE      0xB9
#define KEYPAD_TAB              0xBA
#define KEYPAD_BACKSPACE        0xBB
#define KEYPAD_A                0xBC
#define KEYPAD_B                0xBD
#define KEYPAD_C                0xBE
#define KEYPAD_D                0xBF
#define KEYPAD_E                0xC0
#define KEYPAD_F                0xC1
#define KEYPAD_XOR              0xC2
#define KEYPAD_HAT              0xC3      // ^
#define KEYPAD_PERCENT          0xC4
#define KEYPAD_LESS             0xC5
#define KEYPAD_MORE             0xC6
#define KEYPAD_AND              0xC7
#define KEYPAD_LOGICAL_AND      0xC8
#define KEYPAD_OR               0xC9
#define KEYPAD_LOGICAL_OR       0xCA
#define KEYPAD_COLON            0xCB
#define KEYPAD_HASH             0xCC    // #
#define KEYPAD_SPACE            0xCD
#define KEYPAD_ATMARK           0xCE
#define KEYPAD_NOT              0xCF    // !
#define KEYPAD_MEMORY_STORE     0xD0
#define KEYPAD_MEMORY_RECALL    0xD1
#define KEYPAD_MEMORY_CLEAR     0xD2
#define KEYPAD_MEMORY_ADD       0xD3
#define KEYPAD_MEMORY_SUBTRACT  0xD4
#define KEYPAD_MEMORY_MULTIPLY  0xD5
#define KEYPAD_MEMORY_DIVIDE    0xD6
#define KEYPAD_SIGN             0xD7    // +/-
#define KEYPAD_CLEAR            0xD8
#define KEYPAD_CLEAR_ENTRY      0xD9
#define KEYPAD_BINARY           0xDA
#define KEYPAD_OCTAL            0xDB
#define KEYPAD_DECIMAL          0xDC
#define KEYPAD_HEXADECIMAL      0xDD
#define KEY_LEFT_CONTROL        0xE0
#define KEY_LEFT_SHIFT          0xE1
#define KEY_LEFT_ALT            0xE2
#define KEY_LEFT_GUI            0xE3
#define KEY_RIGHT_CONTROL       0xE4
#define KEY_RIGHT_SHIFT         0xE5
#define KEY_RIGHT_ALT           0xE6
#define KEY_RIGHT_GUI           0xE7

#define KEY_CALC                0xFB

// HID extensions for NISSE
#define KEYPAD_PERCENT          0xC4
#define KEY_LEFT_FN             0xF0
#define KEY_RIGHT_FN            0xF1
#define KEY_DAKUTEN             0xF3
#define KEY_HANDAKU             0xF4
#define KEY_CALC                0xFB

#define MOD_ALT_LEFT            (1u << (KEY_LEFT_ALT - KEY_LEFT_CONTROL))
#define MOD_ALT_RIGHT           (1u << (KEY_RIGHT_ALT - KEY_LEFT_CONTROL))
#define MOD_ALT                 (MOD_ALT_LEFT | MOD_ALT_RIGHT)

#define MOD_SHIFT_LEFT          (1u << (KEY_LEFT_SHIFT - KEY_LEFT_CONTROL))
#define MOD_SHIFT_RIGHT         (1u << (KEY_RIGHT_SHIFT - KEY_LEFT_CONTROL))
#define MOD_SHIFT               (MOD_SHIFT_LEFT | MOD_SHIFT_RIGHT)

#define MOD_CONTROL_LEFT        (1u << (KEY_LEFT_CONTROL - KEY_LEFT_CONTROL))
#define MOD_CONTROL_RIGHT       (1u << (KEY_RIGHT_CONTROL - KEY_LEFT_CONTROL))
#define MOD_CONTROL             (MOD_CONTROL_LEFT | MOD_CONTROL_RIGHT)

#define MOD_GUI_LEFT            (1u << (KEY_LEFT_GUI - KEY_LEFT_CONTROL))
#define MOD_GUI_RIGHT           (1u << (KEY_RIGHT_GUI - KEY_LEFT_CONTROL))
#define MOD_GUI                 (MOD_GUI_LEFT | MOD_GUI_RIGHT)

//
// LED
//

#define LED_NUM_LOCK            1
#define LED_CAPS_LOCK           2
#define LED_SCROLL_LOCK         3
#define LED_COMPOSE             4
#define LED_KANA                5
#define LED_MAX                 LED_SCROLL_LOCK

#define LED_NUM_LOCK_BIT        0x01
#define LED_CAPS_LOCK_BIT       0x02
#define LED_SCROLL_LOCK_BIT     0x04

//
// Kana
//

#define ROMA_NONE       0

#define ROMA_A          1
#define ROMA_I          2
#define ROMA_U          3
#define ROMA_E          4
#define ROMA_O          5

#define ROMA_K          7
#define ROMA_KA         8
#define ROMA_KI         9
#define ROMA_KU         10
#define ROMA_KE         11
#define ROMA_KO         12
#define ROMA_KY         13

#define ROMA_S          14
#define ROMA_SA         15
#define ROMA_SI         16
#define ROMA_SU         17
#define ROMA_SE         18
#define ROMA_SO         19
#define ROMA_SY         20

#define ROMA_T          21
#define ROMA_TA         22
#define ROMA_TI         23
#define ROMA_TU         24
#define ROMA_TE         25
#define ROMA_TO         26
#define ROMA_TY         27

#define ROMA_N          28
#define ROMA_NA         29
#define ROMA_NI         30
#define ROMA_NU         31
#define ROMA_NE         32
#define ROMA_NO         33
#define ROMA_NY         34

#define ROMA_H          35
#define ROMA_HA         36
#define ROMA_HI         37
#define ROMA_HU         38
#define ROMA_HE         39
#define ROMA_HO         40
#define ROMA_HY         41

#define ROMA_M          42
#define ROMA_MA         43
#define ROMA_MI         44
#define ROMA_MU         45
#define ROMA_ME         46
#define ROMA_MO         47
#define ROMA_MY         48

#define ROMA_Y          49
#define ROMA_YA         50
#define ROMA_YU         52
#define ROMA_YO         54

#define ROMA_R          56
#define ROMA_RA         57
#define ROMA_RI         58
#define ROMA_RU         59
#define ROMA_RE         60
#define ROMA_RO         61
#define ROMA_RY         62

#define ROMA_W          63
#define ROMA_WA         64
#define ROMA_WO         68

#define ROMA_P          70
#define ROMA_PA         71
#define ROMA_PI         72
#define ROMA_PU         73
#define ROMA_PE         74
#define ROMA_PO         75
#define ROMA_PY         76

#define ROMA_G          77
#define ROMA_GA         78
#define ROMA_GI         79
#define ROMA_GU         80
#define ROMA_GE         81
#define ROMA_GO         82
#define ROMA_GY         83

#define ROMA_Z          84
#define ROMA_ZA         85
#define ROMA_ZI         86
#define ROMA_ZU         87
#define ROMA_ZE         88
#define ROMA_ZO         89
#define ROMA_ZY         90

#define ROMA_D          91
#define ROMA_DA         92
#define ROMA_DI         93
#define ROMA_DU         94
#define ROMA_DE         95
#define ROMA_DO         96
#define ROMA_DY         97

#define ROMA_B          98
#define ROMA_BA         99
#define ROMA_BI         100
#define ROMA_BU         101
#define ROMA_BE         102
#define ROMA_BO         103
#define ROMA_BY         104

#define ROMA_X          105
#define ROMA_XA         106
#define ROMA_XI         107
#define ROMA_XU         108
#define ROMA_XE         109
#define ROMA_XO         110

#define ROMA_XK         112
#define ROMA_XKA        113
#define ROMA_XKE        116

#define ROMA_XT         119
#define ROMA_XTU        122

#define ROMA_XY         126
#define ROMA_XYA        127
#define ROMA_XYU        129
#define ROMA_XYO        131

#define ROMA_XW         133
#define ROMA_XWA        134

#define ROMA_WY         140
#define ROMA_WYI        142
#define ROMA_WYE        144

#define ROMA_V          147
#define ROMA_VU         150

#define ROMA_L          154
#define ROMA_LA         155
#define ROMA_LI         156
#define ROMA_LU         157
#define ROMA_LE         158
#define ROMA_LO         159

// M-type
#define ROMA_ANN        161
#define ROMA_AKU        162
#define ROMA_ATU        163
#define ROMA_AI         164
#define ROMA_INN        165
#define ROMA_IKU        166
#define ROMA_ITU        167
#define ROMA_UNN        168
#define ROMA_UKU        169
#define ROMA_UTU        170
#define ROMA_ENN        171
#define ROMA_EKI        172
#define ROMA_ETU        173
#define ROMA_EI         174
#define ROMA_ONN        175
#define ROMA_OKU        176
#define ROMA_OTU        177
#define ROMA_OU         178
#define ROMA_C          179
#define ROMA_F          180
#define ROMA_J          181
#define ROMA_Q          182

// Common
#define ROMA_NN         200
#define ROMA_CHOUON     201
#define ROMA_DAKUTEN    202
#define ROMA_HANDAKU    203
#define ROMA_QUESTION   204
#define ROMA_TOUTEN     205     // 、
#define ROMA_KUTEN      206     // 。
#define ROMA_LAB        207     // <
#define ROMA_RAB        208     // >

// Stickney Next
#define KANA_DAKUTEN    209
#define KANA_HANDAKU    210
#define KANA_LCB        211
#define KANA_RCB        212
#define KANA_KE         213     // '
#define KANA_SE         ROMA_P
#define KANA_SO         ROMA_C
#define KANA_HE         214     // =
#define KANA_HO         215     // -
#define KANA_NU         216     // 1
#define KANA_ME         217     // /
#define KANA_MU         218     // KEY_NON_US_HASH
#define KANA_WO         219
#define KANA_RO         220     // KEY_INTERNATIONAL1
#define KANA_TOUTEN     221
#define KANA_KUTEN      222
#define KANA_NAKAGURO   223
#define KANA_CHOUON     224

// Common - Extra
#define ROMA_BANG       225     // !
#define ROMA_SPACE      226     // white space

// Non-common (US, JP, IME)
#define ROMA_LCB        230     // 「
#define ROMA_RCB        231     // 」
#define ROMA_LWCB       232     // 『
#define ROMA_RWCB       233     // 』
#define ROMA_LSB        234     // [
#define ROMA_RSB        235     // ]
#define ROMA_NAKAGURO   236     // ・
#define ROMA_SLASH      237     // ／
#define ROMA_SANTEN     238     // …
#define ROMA_COMMA      239     // ，
#define ROMA_PERIOD     240     // ．
#define ROMA_NAMI       241     // 〜

static inline bool KEYBOARD_IsModifier(uint8_t keycode)
{
    return KEY_LEFT_CONTROL <= keycode && keycode <= KEY_RIGHT_GUI;
}

void KEYBOARD_Initialize(void);
bool KEYBOARD_ScanMatrix(void);
bool KEYBOARD_ProcessMatrix(void);
bool KEYBOARD_Task(void);

void KEYBOARD_EnableLED(bool enable);
void KEYBOARD_SetLED(uint8_t led, bool on);
void KEYBOARD_SetLEDs(uint8_t bits);
void KEYBOARD_UpdateLEDs(void);

int8_t KEYBOARD_GetKeycode(int row, int column);
bool KEYBOARD_IsPressed(int row, int col);
bool KEYBOARD_IsRawKeyPressed(uint8_t key);
bool KEYBOARD_IsKeyPressed(uint8_t key);
bool KEYBOARD_IsMake(int row, int col);

bool KEYBOARD_GetReport(uint8_t* preport);
int8_t KEYBOARD_GetFnReport(uint8_t keycode);
bool KEYBOARD_GetMacroReport(uint8_t* preport);
int8_t KEYBOARD_GetKanaReport(const uint8_t mod);

bool KEYBOARD_GetPasskey(uint32_t* passkey);

bool KEYBOARD_GetConsumerReport(uint8_t* preport);

uint8_t KEYBOARD_GetBoardRevision(void);

#endif  // ESRILLE_KEYBOARD_H
