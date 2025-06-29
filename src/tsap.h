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

#ifndef ESRILLE_TSAP_H
#define ESRILLE_TSAP_H

#include <stdbool.h>
#include <stdint.h>

void TSAP_Initialize1(void);
void TSAP_Initialize2(void);    // for USB mode
void TSAP_Task(uint8_t x, uint8_t y, uint16_t touch);
bool TSAP_IsTouched(void);

#endif  // ESRILLE_TSAP_H
