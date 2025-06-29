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

#include "definitions.h"
#include "eeprom.h"

#define NVRAM_SIZE      1024
#define NVRAM_ADDRESS   (0x40000 - NVRAM_SIZE)
#define PAGE_SIZE       NVMCTRL_FLASH_PAGESIZE      // 64 bytes
#define SECTOR_SIZE     NVMCTRL_FLASH_ROWSIZE       // 256 ybtes
#define MAX_SECTORS     (NVRAM_SIZE / SECTOR_SIZE)
#define DATA_SIZE       (PAGE_SIZE - 1)

typedef struct __attribute__((__packed__)) {
    uint8_t data[DATA_SIZE];
    uint8_t group;  // 0x00-0xfe: in use, 0xff: erased
} EEPROM_PAGE;

typedef struct __attribute__((__packed__)) {
    EEPROM_PAGE pages[SECTOR_SIZE / PAGE_SIZE];
} EEPROM_SECTOR;

typedef struct {
    uint8_t currentGroup;
    uint8_t currentSector;
    uint8_t currentPage;
    EEPROM_PAGE __attribute__((aligned(NVMCTRL_FLASH_PAGESIZE))) cache;
} EEPROM_CONTROL;

static EEPROM_CONTROL eepromControl;

void EEPROM_Initialize(void)
{
    NVMCTRL_Initialize();

    eepromControl.currentGroup = 0xff;
    for (int i = 0; i < MAX_SECTORS; ++i) {
        for (int j = 0; j < SECTOR_SIZE / PAGE_SIZE; ++j) {
            while (NVMCTRL_IsBusy())
                ;
            NVMCTRL_ErrorGet();
            NVMCTRL_Read((void*) &eepromControl.cache,
                         PAGE_SIZE,
                         NVRAM_ADDRESS + SECTOR_SIZE * i + PAGE_SIZE * j);
            if (eepromControl.cache.group == 0xff) {
                if (eepromControl.currentGroup == 0xff) {
                    eepromControl.currentSector = MAX_SECTORS - 1;
                    eepromControl.currentPage = SECTOR_SIZE / PAGE_SIZE - 1;
                    eepromControl.cache.group = 0;
                    memset(eepromControl.cache.data, 0xff, DATA_SIZE);
                } else {
                    // reload the current page
                    while (NVMCTRL_IsBusy())
                        ;
                    NVMCTRL_ErrorGet();
                    NVMCTRL_Read((void*) &eepromControl.cache,
                                 PAGE_SIZE,
                                 NVRAM_ADDRESS + SECTOR_SIZE * eepromControl.currentSector + PAGE_SIZE * eepromControl.currentPage);
                }
                return;
            }
            if (eepromControl.currentGroup == 0xff || 0 <= (int8_t) eepromControl.cache.group - (int8_t) eepromControl.currentGroup) {
                eepromControl.currentGroup = eepromControl.cache.group;
                eepromControl.currentSector = i;
                eepromControl.currentPage = j;
            }
        }
    }
}

int EEPROM_Read(size_t offset, void* buffer, size_t count)
{
    memcpy(buffer, eepromControl.cache.data + offset, count);
    return count;
}

int EEPROM_Write(size_t offset, void* buffer, size_t count)
{
    uint32_t addr = 0;
    bool state;

    memcpy(eepromControl.cache.data + offset, buffer, count);
    state = NVIC_INT_Disable();
    if (SECTOR_SIZE / PAGE_SIZE <= ++eepromControl.currentPage) {
        eepromControl.currentPage = 0;
        if (MAX_SECTORS <= ++eepromControl.currentSector) {
            eepromControl.currentSector = 0;
            eepromControl.cache.group = ++eepromControl.currentGroup;
            if (eepromControl.cache.group == 0xff) {
                eepromControl.cache.group = 0;
            }
        }
        addr = NVRAM_ADDRESS + SECTOR_SIZE * eepromControl.currentSector;
        while (NVMCTRL_IsBusy())
            ;
        NVMCTRL_ErrorGet();
        NVMCTRL_RowErase(addr);
    }
    while (NVMCTRL_IsBusy())
        ;
    NVMCTRL_ErrorGet();
    addr = NVRAM_ADDRESS + SECTOR_SIZE * eepromControl.currentSector + PAGE_SIZE * eepromControl.currentPage;
    NVMCTRL_PageWrite((void*) &eepromControl.cache, addr);
    NVIC_INT_Restore(state);
    return count;
}
