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

#ifndef ESRILLE_PROFILE_H
#define ESRILLE_PROFILE_H

#include <stdbool.h>
#include <stdint.h>

#define PROFILE_DATA_SIZE   10

#define PROFILE_INDEX_USB   0
#define PROFILE_INDEX_BLE1  1
#define PROFILE_INDEX_BLE2  2
#define PROFILE_INDEX_BLE3  3
#define PROFILE_INDEX_MAX   3

void PROFILE_Initialize(const void* initialData);
uint8_t PROFILE_Read(uint8_t offset);
void PROFILE_Write(uint8_t offset, uint8_t value);

void PROFILE_Select(uint8_t profile);
uint8_t PROFILE_GetCurrent(void);
bool PROFILE_IsUSBMode(void);

#endif  // ESRILLE_PROFILE_H
