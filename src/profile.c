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

#include <string.h>

#include "profile.h"
#include "eeprom.h"

#define VERSION     1

typedef struct __attribute__((__packed__)) {
    uint8_t data[PROFILE_DATA_SIZE];
} PROFILE_DATA;

typedef struct __attribute__((__packed__)) {
    PROFILE_DATA profiles[PROFILE_INDEX_MAX + 1];
    uint8_t currentProfile;
    uint8_t version;
} PROFILE_PAGE;

static PROFILE_PAGE cache;

static bool IsValidPage(const PROFILE_PAGE* page)
{
    if (page->version != VERSION)
        return false;
    if (PROFILE_INDEX_MAX < page->currentProfile)
        return false;
    return true;
}

void PROFILE_Initialize(const void* initialData)
{
    EEPROM_Initialize();
    EEPROM_Read(0, &cache, sizeof(cache));
    if (!IsValidPage(&cache)) {
        cache.version = VERSION;
        cache.currentProfile = 0;
        for (int i = 0; i <= PROFILE_INDEX_MAX; ++i) {
            memcpy(cache.profiles[i].data, initialData, PROFILE_DATA_SIZE);
        }
    }
}

uint8_t PROFILE_Read(uint8_t offset)
{
    return cache.profiles[PROFILE_GetCurrent()].data[offset];
}

void PROFILE_Write(uint8_t offset, uint8_t value)
{
    cache.profiles[PROFILE_GetCurrent()].data[offset] = value;
    EEPROM_Write(0, &cache, sizeof(cache));
}

void PROFILE_Select(uint8_t index)
{
    cache.currentProfile = index;
    EEPROM_Write(0, &cache, sizeof(cache));
}

uint8_t PROFILE_GetCurrent(void)
{
    return cache.currentProfile;
}

bool PROFILE_IsUSBMode(void)
{
    return cache.currentProfile == PROFILE_INDEX_USB;
}
