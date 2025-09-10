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

#define XY_DEAD_ZONE        24
#define XY_RANGE            (128 - XY_DEAD_ZONE)
#define TOUCH_DELAY         12      // a very short touch should be ignored.
#define TOUCH_THRESH        1000
#define TOUCH_MAX           4095
#define TOUCH_RAMP_LIMIT    100

typedef struct {
    uint8_t x;
    uint8_t y;
} POSITION;

typedef struct {
    int16_t x;
    int16_t y;
} SCALED_POSITION;

typedef struct {
    uint8_t count;
    uint8_t x;
    uint8_t y;
    uint16_t touch;
} SERIAL_DATA;

typedef struct {
    uint16_t current;
    uint16_t thresh;
    uint16_t high;
    uint16_t max;
    uint8_t  delay;
} TOUCH_SENSOR;

typedef struct {
    uint8_t         resolution;
    SERIAL_DATA     rawData;
    TOUCH_SENSOR    touchSensor;
    POSITION        center;
    POSITION        rawPrev;
    SCALED_POSITION scaledPosition;
    bool            aim;
    uint8_t         mouse[MOUSE_REPORT_LEN];
    uint8_t         mousePrev[MOUSE_REPORT_LEN];
    int8_t          wheel;
} TSAP_CONTROL;

static const uint8_t aboutTSAP[] = {
    KEY_P, KEY_A, KEY_D, KEY_SPACE, 0
};

static TSAP_CONTROL controller;

static void LoadResolution(void)
{
    controller.resolution = PROFILE_Read(EEPROM_MOUSE);
    if (PAD_SENSE_MAX < controller.resolution) {
        controller.resolution = 0;
    }
}

static void SetResolution(uint8_t val)
{
    controller.resolution = (PAD_SENSE_MAX < val) ? 0 : val;
    PROFILE_Write(EEPROM_MOUSE, controller.resolution);
}

static uint8_t GetDistance(uint8_t raw, uint8_t center)
{
    return (raw < center) ? (center - raw) : (raw - center);
}

// Return 0.75 * prev + (1 - 0.75) * raw
static uint16_t LowPassFilter(uint16_t prev, uint16_t raw)
{
    prev -= (prev >> 2);
    raw >>= 2;
    return prev + raw;
}

static void ProcessSerialData(void)
{
    // touch
    uint16_t touch = controller.rawData.touch;

    if (touch < TOUCH_THRESH) { // too weak
        touch = TOUCH_THRESH;
    }

    // Limit max increase to TOUCH_RAMP_LIMIT to avoid sudden jumps from noise or glitches.
    if (controller.touchSensor.max < touch) {
        uint16_t limit = controller.touchSensor.max + TOUCH_RAMP_LIMIT;

        if (limit < touch) {
            touch = limit;
        }
        controller.touchSensor.max = touch;
    }

    controller.touchSensor.current = LowPassFilter(controller.touchSensor.current, touch);
    if (controller.touchSensor.high < controller.touchSensor.current) {
        controller.touchSensor.high = controller.touchSensor.current;
        controller.touchSensor.thresh = (controller.touchSensor.high * 5) / 7;
    }
    if (controller.touchSensor.current < controller.touchSensor.thresh) { // touched?
        if (controller.touchSensor.delay < TOUCH_DELAY) {
            ++controller.touchSensor.delay;
        }
    } else {
        if ((GetDistance(controller.rawData.x, 128u) < XY_DEAD_ZONE &&
                GetDistance(controller.rawData.y, 128u) < XY_DEAD_ZONE) ||
                (controller.center.x == 0 && controller.center.y == 0)) {
            if (controller.rawData.x == controller.rawPrev.x && controller.rawData.y == controller.rawPrev.y) {
                controller.center.x = controller.rawData.x;
                controller.center.y = controller.rawData.y;
                if (controller.touchSensor.delay == TOUCH_DELAY) { // previously touched?
                    controller.touchSensor.high = (controller.touchSensor.high * 8) / 9;
                    if (controller.touchSensor.high < controller.touchSensor.current) {
                        controller.touchSensor.high = controller.touchSensor.current;
                    }
                    controller.touchSensor.thresh = (controller.touchSensor.high * 5) / 7;
                }
            }
        }
        controller.touchSensor.delay = 0;
    }

    if (TSAP_IsTouched()) {
        if (KEYBOARD_IsKeyPressed(KEY_I) || KEYBOARD_IsKeyPressed(KEY_D)) {
            controller.wheel = 1;
        } else if (KEYBOARD_IsKeyPressed(KEY_K) || KEYBOARD_IsKeyPressed(KEY_C)) {
            controller.wheel = -1;
        } else {
            controller.wheel = 0;
        }
    }

    // (x, y)
    int8_t sign_x = 1;
    int8_t sign_y = 1;
    int16_t x = controller.rawData.x - controller.center.x;
    int16_t y = controller.rawData.y - controller.center.y;
    if (x < 0) {
        sign_x = -1;
        x = -x;
    }
    if (y < 0) {
        sign_y = -1;
        y = -y;
    }
    uint16_t r = isqrt16(x * x + y * y);
    if (r <= XY_DEAD_ZONE) {
        x = y = 0;
    } else {
        x = x * (r - XY_DEAD_ZONE) / r;
        y = y * (r - XY_DEAD_ZONE) / r;
        x *= x;
        y *= y;
    }

    controller.scaledPosition.x += sign_x * x;
    controller.scaledPosition.y += sign_y * y;

    controller.rawPrev.x = controller.rawData.x;
    controller.rawPrev.y = controller.rawData.y;
}

// Protocol:
// 1  tB tA t9 t8 t7 y7 x7
// 0  t6 t5 t4 t3 t2 t1 t0
// 0  x6 x5 x4 x3 x2 x1 x0
// 0  y6 y5 y4 y3 y2 y1 y0
static bool ProcessOctet(uint8_t octet)
{
    bool ready = false;

    if (octet & 0x80) {
        controller.rawData.count = 1;
    }
    switch (controller.rawData.count) {
    case 1:
        controller.rawData.touch = ((uint16_t) (octet & 0x7c)) << 5;
        controller.rawData.y = (octet & 0x02) << 6;
        controller.rawData.x = (octet & 0x01) << 7;
        ++controller.rawData.count;
        break;
    case 2:
        controller.rawData.touch |= octet;
        ++controller.rawData.count;
        break;
    case 3:
        controller.rawData.x |= octet;
        ++controller.rawData.count;
        break;
    case 4:
        controller.rawData.y |= octet;
        controller.rawData.count = 0;
        ProcessSerialData();
        ready = true;
        break;
    default:
        break;
    }
    return ready;
}

void TSAP_Task(uint8_t x, uint8_t y, uint16_t touch)
{
    controller.rawData.x = x;
    controller.rawData.y = y;
    controller.rawData.touch = touch;
    ProcessSerialData();
}

bool TSAP_IsTouched(void)
{
    if (controller.touchSensor.current < controller.touchSensor.thresh && TOUCH_DELAY <= controller.touchSensor.delay) {
        return true;
    }
    return false;
}

static char receiveBuffer[1];

void TSAP_ReadCallback(uintptr_t context)
{
    if (USART_ERROR_NONE != SERCOM4_USART_ErrorGet()) {
        //Handle error case
    } else {
        //Transfer completed successfully
        ProcessOctet(receiveBuffer[0]);
    }
    SERCOM4_USART_Read(receiveBuffer, sizeof receiveBuffer);
}

void TSAP_Reset(void)
{
    bool interruptStatus = NVIC_INT_Disable();
    memset(&controller, 0, sizeof controller);
    LoadResolution();
    NVIC_INT_Restore(interruptStatus);
}

void TSAP_Initialize1(void)
{
    LoadResolution();
}

void TSAP_Initialize2(void)
{
    // SERCOM4 is used to monitor TSAP in the USB mode
    SERCOM4_USART_Initialize();
    SERCOM4_USART_ReadCallbackRegister(TSAP_ReadCallback, (uintptr_t) NULL);
    SERCOM4_USART_Read(receiveBuffer, sizeof receiveBuffer);
}

bool MOUSE_GetReport(uint8_t* preport)
{
    if (TSAP_IsTouched()) {
        uint8_t buttons = 0;

        if (KEYBOARD_IsKeyPressed(KEY_F9)) {
            SetResolution(PAD_SENSE_4);
        } else if (KEYBOARD_IsKeyPressed(KEY_F10)) {
            SetResolution(PAD_SENSE_3);
        } else if (KEYBOARD_IsKeyPressed(KEY_F11)) {
            SetResolution(PAD_SENSE_2);
        } else if (KEYBOARD_IsKeyPressed(KEY_F12)) {
            SetResolution(PAD_SENSE_1);
        } else if (KEYBOARD_IsKeyPressed(KEY_J) || KEYBOARD_IsKeyPressed(KEY_V)) {
            buttons |= MOUSE_BUTTON_LEFT;
        } else if (KEYBOARD_IsKeyPressed(KEY_L) || KEYBOARD_IsKeyPressed(KEY_X)) {
            buttons |= MOUSE_BUTTON_RIGHT;
        } else if (KEYBOARD_IsKeyPressed(KEY_COMMA)) {
            buttons |= MOUSE_BUTTON_MIDDLE;
        } else if (KEYBOARD_IsKeyPressed(KEY_U) || KEYBOARD_IsKeyPressed(KEY_Z)) {
            buttons |= MOUSE_BUTTON_BACKWARD;
        } else if (KEYBOARD_IsKeyPressed(KEY_O) || KEYBOARD_IsKeyPressed(KEY_B)) {
            buttons |= MOUSE_BUTTON_FORWARD;
        }
        controller.aim = KEYBOARD_IsKeyPressed(KEY_SEMICOLON);
        controller.mouse[0] = buttons;
        bool interruptStatus = NVIC_INT_Disable();
        int16_t x = controller.scaledPosition.x;
        int16_t y = controller.scaledPosition.y;
        int16_t gain = XY_RANGE * (4 - controller.resolution);
        x /= gain;
        y /= gain;
        controller.scaledPosition.x -= gain * x;
        controller.scaledPosition.y -= gain * y;
        controller.mouse[3] = controller.wheel;
        controller.wheel = 0;
        NVIC_INT_Restore(interruptStatus);
        controller.mouse[1] = x;
        controller.mouse[2] = y;
    } else {
        controller.aim = false;
        memset(controller.mouse, 0, MOUSE_REPORT_LEN);
    }

    if (controller.mouse[1] || controller.mouse[2] || controller.mouse[3] || memcmp(controller.mouse, controller.mousePrev, MOUSE_REPORT_LEN)) {
        memmove(preport, controller.mouse, MOUSE_REPORT_LEN);
        memmove(controller.mousePrev, controller.mouse, MOUSE_REPORT_LEN);
        return true;
    }
    return false;
}

void MOUSE_About(void)
{
    MACRO_Puts(aboutTSAP);
    MACRO_PutNumber(1 + PROFILE_Read(EEPROM_MOUSE));

    MACRO_Put(KEY_SPACE);
    MACRO_PutNumber(controller.touchSensor.current);
    MACRO_Put(KEY_SLASH);
    MACRO_PutNumber(controller.touchSensor.thresh);
    MACRO_Put(KEY_SLASH);
    MACRO_PutNumber(controller.touchSensor.high);
    MACRO_Put(KEY_SLASH);
    MACRO_PutNumber(controller.touchSensor.max);
    MACRO_Put(KEY_SPACE);
    MACRO_PutNumber(controller.rawPrev.x);
    MACRO_Put(KEY_COMMA);
    MACRO_PutNumber(controller.rawPrev.y);
    MACRO_Put(KEY_ENTER);
}
