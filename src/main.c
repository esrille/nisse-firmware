/*
 * Copyright 2025 Esrille Inc.
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

int main(void)
{
    // Initialize basic modules for both USB and BLE
    SYS_Initialize1(NULL);

    KEYBOARD_Initialize();

    if (!USB_MODE_Get()) {
        if (HOS_CheckModule()) {
            // HOS_MainLoop() checks if the current profile is configured to use a Bluetooth connection.
            // If so, it initiates the HID over SPI functionality and enters a loop to handle Bluetooth
            // communication, not returning to the caller.
            HOS_MainLoop();
        }
    }

    // Initialize USB modules
    SYS_Initialize2(NULL);

    // Activate the USB HID connection
    uint16_t tick = APP_GetTick();
    for (;;) {
        // Continuously poll all MPLAB Harmony state machines to ensure they are functioning correctly
        SYS_Tasks();

        // Check if the tick count has changed since the last iteration
        if (tick != APP_GetTick()) {
            // Update the tick count to the current value
            tick = APP_GetTick();

            if (!APP_Suspended()) {
                // If the application is not suspended, execute the keyboard task
                KEYBOARD_Task();
            } else if (KEYBOARD_ScanMatrix()) {
                // If the application is suspended and any key is pressed, wake up the application
                APP_WakeUp();
            }
        }
    }

    // Execution should not come here during normal operation
    return EXIT_FAILURE;
}
