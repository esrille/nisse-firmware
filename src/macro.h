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

#ifndef ESRILLE_MACRO_H
#define ESRILLE_MACRO_H

#include <stddef.h>
#include <stdint.h>

#define MAX_MACRO_SIZE  512

void MACRO_Begin(size_t max);
uint8_t MACRO_Peek(void);
uint8_t MACRO_Get(void);
void MACRO_Put(uint8_t c);
void MACRO_Puts(const uint8_t* s);
void MACRO_PutNumber(uint16_t n);

#endif  // ESRILLE_MACRO_H
