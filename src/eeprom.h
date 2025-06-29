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

#ifndef ESRILLE_EEPROM_H
#define ESRILLE_EEPROM_H

#include <stddef.h>

void EEPROM_Initialize(void);

int EEPROM_Read(size_t offset, void* buffer, size_t count);

int EEPROM_Write(size_t offset, void* buffer, size_t count);

#endif  // ESRILLE_EEPROM_H
