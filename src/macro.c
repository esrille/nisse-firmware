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
    uint8_t keys[MAX_MACRO_SIZE];
    size_t pos;
    size_t max;
} MACRO_DATA;

static MACRO_DATA macroData;

void MACRO_Begin(size_t max)
{
    macroData.pos = 0;
    macroData.max = max;
}

uint8_t MACRO_Peek(void)
{
    if (macroData.pos < macroData.max) {
        return macroData.keys[macroData.pos];
    }
    return 0;
}

uint8_t MACRO_Get(void)
{
    if (macroData.pos < macroData.max) {
        uint8_t key = macroData.keys[macroData.pos++];
        if (key) {
            return key;
        }
    }
    macroData.pos = macroData.max = 0;
    return 0;
}

void MACRO_Put(uint8_t c)
{
    if (macroData.pos < MAX_MACRO_SIZE) {
        macroData.keys[macroData.pos++] = c;
        if (macroData.pos < MAX_MACRO_SIZE) {
            macroData.keys[macroData.pos] = 0;
        }
    }
}

void MACRO_Puts(const uint8_t* s)
{
    while (*s) {
        MACRO_Put(*s++);
    }
}

static uint8_t GetKeyForN(unsigned n)
{
    if (n == 0)
        return KEY_0;
    if (1 <= n && n <= 9)
        return KEY_1 - 1 + n;
    return KEY_SPACE;
}

void MACRO_PutNumber(uint16_t n)
{
    bool zero = false;

    for (uint16_t i = 10000; 0 < i; i /= 10) {
        uint8_t d = n / i;
        if (d || i == 1 || zero) {
            zero = true;
            MACRO_Put(GetKeyForN(d));
        }
        n %= i;
    }
}
