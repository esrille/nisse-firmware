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

#ifndef ESRILLE_MOUSE_H
#define ESRILLE_MOUSE_H

#include <stdbool.h>
#include <stdint.h>

#define MOUSE_REPORT_LEN        4

#define MOUSE_BUTTON_LEFT       0x01    // Left button
#define MOUSE_BUTTON_RIGHT      0x02    // Right button
#define MOUSE_BUTTON_MIDDLE     0x04    // Middle button
#define MOUSE_BUTTON_BACKWARD   0x08    // Backward button
#define MOUSE_BUTTON_FORWARD    0x10    // Forward button

typedef struct
{
    uint8_t data[MOUSE_REPORT_LEN];
} MOUSE_REPORT;

bool MOUSE_GetReport(uint8_t* preport);

#endif  // ESRILLE_MOUSE_H
