/*
 * Copyright 2016-2025 Esrille Inc.
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

/*
 * ⚠ ATTENTION
 *
 * This file must not be modified to maintain Bluetooth® Qualification of the
 * Esrille New Keyboard - NISSE completed by Esrille Inc.
 *
 * See "Bluetooth Qualification and Declaration Processes" for more details:
 *   https://www.bluetooth.org/en-us/test-qualification/qualification-overview
 */

#include "app.h"

#define HOS_TYPE_DEFAULT    (APP_HAS_MOUSE_INTERFACE ? HOS_TYPE_TSAP : HOS_TYPE_INFO)
#define WDT_FREQ            60u

// BSP indication states
#define ADVERTISING_DIRECTED_LED_ON_INTERVAL   200      // Directed advertising
#define ADVERTISING_DIRECTED_LED_OFF_INTERVAL  200      // Period 0.4 sec, duty cycle 50%
#define ADVERTISING_WHITELIST_LED_ON_INTERVAL  300      // Fast/slow whitelist advertising
#define ADVERTISING_WHITELIST_LED_OFF_INTERVAL 700      // Period 1 sec, duty cycle 30%
#define ADVERTISING_LED_ON_INTERVAL            200      // Fast advertising
#define ADVERTISING_LED_OFF_INTERVAL           800      // Period 1 sec, duty cycle 20%
#define ADVERTISING_SLOW_LED_ON_INTERVAL       100      // Slow advertizing
#define ADVERTISING_SLOW_LED_OFF_INTERVAL      900      // Period 1 sec, duty cycle 10%
#define BONDING_INTERVAL                       100      // Bonding

#define RETRY_MAX   5
#define RETRY_WAIT  128 // [usec]

#define BATTERY_LEVELS_SIZE                     100     // from 2.00 (200) to 2.99 (299)
#define BATTERY_LEVEL_MEAS_INTERVAL             (WDT_FREQ * HOS_BATTERY_LEVEL_MEAS_INTERVAL / 1000)

#define SPI_TX_BUF_SIZE 128u                // SPI TX buffer size
#define SPI_RX_BUF_SIZE (HOS_STATE_LAST+1)  // SPI RX buffer size

static uint8_t status[HOS_STATE_COMMON_LAST + 1];

typedef struct {
    uint8_t revisionMajor;
    uint8_t revisionMinor;
    uint8_t versionMajor;
    uint8_t versionMinor;
} INFO_DATA;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t touchLo;
    uint8_t touchHi;
} TSAP_DATA;

static INFO_DATA info;
static TSAP_DATA tsap;
static bool isModuleInstalled = false;

static uint16_t batteryVoltage;
static uint8_t  batteryLevel;

static const uint8_t batteryLevels[BATTERY_LEVELS_SIZE] = {
//  .00  .01  .02  .03  .04  .05  .06  .07  .08  .09
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   1,   1,   1,   1,
      1,   1,   1,   1,   2,   2,   2,   2,   3,   3,
      4,   5,   5,   6,   7,   8,   9,  10,  11,  12,
     14,  15,  17,  18,  21,  24,  28,  33,  40,  47,
     54,  64,  72,  76,  78,  80,  82,  83,  85,  86,
     87,  88,  89,  90,  91,  92,  93,  93,  94,  95,
     95,  96,  96,  97,  97,  98,  98,  99,  99,  99,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
};

static int8_t CheckProfile(uint8_t profile)
{
    return ((~profile >> 4) & 0x0f) == (profile & 0x0f);
}

int8_t HOS_Report(uint8_t type, uint8_t cmd, uint8_t len, const uint8_t* data)
{
    static uint8_t txBuf[SPI_TX_BUF_SIZE];
    static uint8_t rxBuf[SPI_RX_BUF_SIZE];
    size_t rxLen;
    int good = 0;

    for (int retry = 0; retry < RETRY_MAX; ++retry) {
        rxLen = 0;
        txBuf[rxLen++] = type;
        txBuf[rxLen++] = cmd;
        txBuf[rxLen++] = len;
        memmove(txBuf + rxLen, data, len);
        rxLen += len;

        /* SPI Write Read */
        CS_Clear();
        SERCOM5_SPI_WriteRead(txBuf, rxLen, rxBuf, SPI_RX_BUF_SIZE);
        CS_Set();

        if (rxBuf[0] == HOS_DEF_CHARACTER) {
            TC3_DelayUs(RETRY_WAIT);
            continue;
        }

        if (CheckProfile(rxBuf[HOS_STATE_PROFILE])) {
            memmove(status, rxBuf, HOS_STATE_COMMON_LAST + 1);
            switch (status[HOS_STATE_TYPE]) {
            case HOS_TYPE_INFO:
                memmove(&info, rxBuf + HOS_STATE_REV_MAJOR, HOS_STATE_VER_MINOR - HOS_STATE_REV_MAJOR + 1);
                break;
            case HOS_TYPE_TSAP:
                memmove(&tsap, rxBuf + HOS_STATE_X, HOS_STATE_TOUCH_HI - HOS_STATE_X + 1);
                break;
            default:
                break;
            }
            good = 1;
        }
        break;
    }
    return good;
}

int8_t HOS_GetStatus(uint8_t type)
{
    return HOS_Report(type, HOS_CMD_GET_STATUS, 0, NULL);
}

int8_t HOS_SetEvent(uint8_t type, uint8_t key)
{
    return HOS_Report(type, HOS_CMD_SET_EVENT, 1, &key);
}

int8_t HOS_Sleep(uint8_t type)
{
    int8_t good = HOS_SetEvent(type, HOS_EVENT_SLEEP);
    if (good) {
        status[HOS_STATE_INDICATE] = HOS_BLE_STATE_IDLE;
    }
    return good;
}

uint16_t HOS_GetBatteryVoltage(void)
{
    return (HOS_BATTERY_VOLTAGE_OFFSET < batteryVoltage) ? batteryVoltage : 0;
}

uint8_t HOS_GetBatteryLevel(void)
{
    uint8_t  level;
    uint16_t voltage = HOS_GetBatteryVoltage();

    if (300 <= voltage)
        level = 100u;
    else if (voltage < 200)
        level = 0u;
    else
        level = batteryLevels[voltage - 200];
    return level;
}

static uint8_t UpdateBatteryLevel(uint16_t tick)
{
    int8_t good = 1;

    if (!(tick % BATTERY_LEVEL_MEAS_INTERVAL)) {
        uint16_t v = HOS_BATTERY_VOLTAGE_OFFSET + status[HOS_STATE_BATT];
        uint16_t diff = (batteryVoltage < v) ? (v - batteryVoltage) : (batteryVoltage - v);
        if (50 < diff) {
            batteryVoltage = v;
        } else {
            // Apply low pass filter:
            // 0.75 * prev + (1 - 0.75) * current
            batteryVoltage += (v >> 2) - (batteryVoltage >> 2);
        }

        uint8_t level = HOS_GetBatteryLevel();
        if (batteryLevel != level) {
            batteryLevel = level;
            good = HOS_SetBatteryLevel(HOS_TYPE_DEFAULT, batteryLevel);
        }
    }
    return good;
}

int8_t HOS_SetBatteryLevel(uint8_t type, uint8_t level)
{
    return HOS_Report(type, HOS_CMD_BATT_REPORT, 1, &level);
}

uint8_t HOS_GetLED(void)
{
    return status[HOS_STATE_LED];
}

uint8_t HOS_GetProfile(void)
{
    return status[HOS_STATE_PROFILE] & 0x0f;
}

uint8_t HOS_GetIndication(void)
{
    return status[HOS_STATE_INDICATE] & ~(HOS_BLE_STATE_LESC | HOS_BLE_STATE_SUSPENDED);
}

uint8_t HOS_GetSuspended(void)
{
    return (status[HOS_STATE_INDICATE] & HOS_BLE_STATE_SUSPENDED) ? 1 : 0;
}

uint8_t HOS_GetLESC(void)
{
    return (status[HOS_STATE_INDICATE] & HOS_BLE_STATE_LESC) ? 1 : 0;
}

static int8_t IsInInterval(uint16_t onInterval, uint16_t offInterval, uint16_t tick)
{
    uint16_t range = onInterval + offInterval;

    tick *= (1000 / WDT_FREQ);  // tick to msec
    tick %= range;
    return tick < onInterval;
}

void HOS_UpdateLED(uint8_t profile, uint16_t tick)
{
    uint8_t indicate = HOS_BLE_STATE_IDLE;

    if (profile != 0) {
        indicate = HOS_GetIndication();
    }
    switch (indicate) {
    case HOS_BLE_STATE_SCANNING:
    case HOS_BLE_STATE_ADVERTISING:
        KEYBOARD_SetLED(profile, IsInInterval(ADVERTISING_LED_ON_INTERVAL, ADVERTISING_LED_OFF_INTERVAL, tick));
        break;
    case HOS_BLE_STATE_ADVERTISING_WHITELIST:
        KEYBOARD_SetLED(profile, IsInInterval(ADVERTISING_WHITELIST_LED_ON_INTERVAL, ADVERTISING_WHITELIST_LED_OFF_INTERVAL, tick));
        break;
    case HOS_BLE_STATE_ADVERTISING_SLOW:
        KEYBOARD_SetLED(profile, IsInInterval(ADVERTISING_SLOW_LED_ON_INTERVAL, ADVERTISING_SLOW_LED_OFF_INTERVAL, tick));
        break;
    case HOS_BLE_STATE_ADVERTISING_DIRECTED:
        KEYBOARD_SetLED(profile, IsInInterval(ADVERTISING_DIRECTED_LED_ON_INTERVAL, ADVERTISING_DIRECTED_LED_OFF_INTERVAL, tick));
        break;
    case HOS_BLE_STATE_BONDING:
        KEYBOARD_SetLED(profile, IsInInterval(BONDING_INTERVAL, BONDING_INTERVAL, tick));
        break;
    case HOS_BLE_STATE_CONNECTED:
        break;
    default:
        KEYBOARD_SetLEDs(0);
        break;
    }
}

uint16_t HOS_GetTouch(void)
{
    return (tsap.touchHi << 8) | tsap.touchLo;
}

uint8_t HOS_GetKeyboardMouseX(void)
{
    return tsap.x;
}

uint8_t HOS_GetKeyboardMouseY(void)
{
    return tsap.y;
}

uint16_t HOS_GetVersion(void)
{
    return (info.versionMajor << 8) | info.versionMinor;
}

uint16_t HOS_GetRevision(void)
{
    return (info.revisionMajor << 8) | info.revisionMinor;
}

bool HOS_IsModuleInstalled(void)
{
    return isModuleInstalled;
}

bool HOS_CheckModule(void)
{
    bool dfu = false;

    for (int i = 0; i < HOS_STARTUP_DELAY; ++i) {
        KEYBOARD_ScanMatrix();
        if (!isModuleInstalled && HOS_GetStatus(HOS_TYPE_INFO)) {
            isModuleInstalled = true;
        }
        if (isModuleInstalled) {
            if (!KEYBOARD_IsRawKeyPressed(KEY_APPLICATION))
                break;
            if (HOS_SetEvent(HOS_TYPE_INFO, HOS_EVENT_DFU)) {
                dfu = true;
                break;
            }
        }
        PM_StandbyModeEnter();
    }

    if (dfu) {
        KEYBOARD_SetLEDs(0);
        for (;;) {
            KEYBOARD_UpdateLEDs();
            if (HOS_GetStatus(HOS_TYPE_INFO))
                break;
            uint16_t range = APP_GetTick() * (1000 / WDT_FREQ) % 1000;
            KEYBOARD_SetLED(LED_SCROLL_LOCK, range < 500);
            PM_StandbyModeEnter();
        }
    }

    return isModuleInstalled;
}

static inline bool IsBusPowered(void)
{
    return USB_VBUS_SENSE_Get() == 1;
}

void HOS_Initialize(void)
{
    memset(status, 0, sizeof status);
}

static void WaitForResume(void)
{
    KEYBOARD_EnableLED(false);

    SYS_CONSOLE_MESSAGE("BLE host in Sleep mode.\r\n");
#ifdef SYS_CONSOLE_DEFAULT_INSTANCE
    /* Ensure that the SYS Console has transmitted all the bytes in queue */
    while(SYS_CONSOLE_WriteCountGet(SYS_CONSOLE_DEFAULT_INSTANCE) != 0)
        ;
#endif

    bool interruptStatus = NVIC_INT_Disable();
    while (HOS_GetSuspended() || HOS_GetIndication() == HOS_BLE_STATE_IDLE) {
        PM_StandbyModeEnter();
        if (KEYBOARD_ScanMatrix()) {
            break;
        }
    }
    NVIC_INT_Restore(interruptStatus);

    KEYBOARD_EnableLED(true);
}

void HOS_MainLoop(void)
{
    static int8_t starting = 1;
    static uint8_t keyboardReport[KEYBOARD_REPORT_LEN];
    static uint16_t ccReport;
#if APP_HAS_MOUSE_INTERFACE
    static uint8_t mouseReport[MOUSE_REPORT_LEN];
#endif

    bool hasReport;
    bool hasConsumer;

    if (PROFILE_IsUSBMode() && IsBusPowered()) {
        for (uint16_t i = 0; i < HOS_STARTUP_DELAY; ++i) {
            if (HOS_Sleep(HOS_TYPE_DEFAULT)) {
                batteryVoltage = HOS_BATTERY_VOLTAGE_OFFSET + status[HOS_STATE_BATT];;
                break;
            }
            PM_StandbyModeEnter();
        }
        return;
    }

    for (uint16_t i = 0; i < HOS_STARTUP_DELAY; ++i) {
        if (HOS_GetStatus(HOS_TYPE_INFO)) {
            batteryVoltage = HOS_BATTERY_VOLTAGE_OFFSET + status[HOS_STATE_BATT];;
            break;
        }
        PM_StandbyModeEnter();
    }
    KEYBOARD_SetLEDs(0);

    uint16_t tick = APP_GetTick();
    for (;;)
    {
        if (tick == APP_GetTick()) {
            PM_StandbyModeEnter();
            continue;
        }
        tick = APP_GetTick();
        if (KEYBOARD_Task()) {
            hasReport = KEYBOARD_GetReport(keyboardReport);
            hasConsumer = KEYBOARD_GetConsumerReport((uint8_t*) &ccReport);
        } else {
            hasReport = false;
            hasConsumer = false;
        }

        if (HOS_GetProfile() != PROFILE_GetCurrent()) {
            if (HOS_GetIndication() == HOS_BLE_STATE_CONNECTED && hasReport) {
                // Send break
                HOS_Report(HOS_TYPE_DEFAULT, HOS_CMD_KEYBOARD_REPORT, 8, keyboardReport);
                PM_StandbyModeEnter();
                continue;
            }
        }

        if (PROFILE_IsUSBMode() && IsBusPowered()) {
            HOS_GetStatus(HOS_TYPE_INFO);  // Get info after reset.
            NVIC_SystemReset();
            // NOT REACHED HERE
        }

        if (HOS_GetProfile() != PROFILE_GetCurrent()) {
            for (uint8_t i = 0; !HOS_GetStatus(HOS_TYPE_DEFAULT) && i < HOS_SYNC_DELAY; ++i) {
                PM_StandbyModeEnter();
            }
            if (HOS_GetProfile() != PROFILE_GetCurrent()) {
                HOS_SetEvent(HOS_TYPE_DEFAULT, HOS_EVENT_KEY_0 + PROFILE_GetCurrent());
                KEYBOARD_SetLEDs(1u << (PROFILE_GetCurrent() - 1));
            }
            APP_ResetTick();
        }
        else
        {
            switch (HOS_GetIndication()) {
            case HOS_BLE_STATE_IDLE:
                if (hasReport || starting) {
                    starting = 1;
                    APP_ResetTick();
                    HOS_GetStatus(HOS_TYPE_DEFAULT);
                }
                HOS_UpdateLED(PROFILE_GetCurrent(), tick);
                break;

            case HOS_BLE_STATE_ADVERTISING:
            case HOS_BLE_STATE_ADVERTISING_WHITELIST:
            case HOS_BLE_STATE_ADVERTISING_SLOW:
            case HOS_BLE_STATE_ADVERTISING_DIRECTED:
                starting = 0;
                HOS_GetStatus(HOS_TYPE_DEFAULT);
                // A new bonding process can be interrupted if there are pre-bonded peers that are active.
                // In such a case, the BLE module timers are also reset, and we must manually stop
                // advertising if a new bonding cannot be made within a reasonable time.
                if (HOS_ADV_TIMEOUT < tick) {
                    HOS_Sleep(HOS_TYPE_DEFAULT);
                }
                HOS_UpdateLED(PROFILE_GetCurrent(), tick);
                break;

            case HOS_BLE_STATE_BONDING:
                if (hasReport) {
                    // Send HOS_CMD_KEYBOARD_REPORT anyway to support passkey entry.
                    HOS_Report(HOS_TYPE_DEFAULT, HOS_CMD_KEYBOARD_REPORT, 8, keyboardReport);
                } else {
                    HOS_GetStatus(HOS_TYPE_DEFAULT);
                }
                HOS_UpdateLED(PROFILE_GetCurrent(), tick);
                break;

            case HOS_BLE_STATE_CONNECTED:
                if (!hasReport && !hasConsumer) {
                    HOS_GetStatus(HOS_TYPE_DEFAULT);
                } else {
                    /* The NISSE BLE module converts keyboard reports into
                     * consumer device reports internally.
                     */
                    if (hasConsumer) {
                        int ccIndex = 2;
                        if (hasReport) {
                            while (ccIndex < 8 && keyboardReport[ccIndex]) {
                                ++ccIndex;
                            }
                        } else {
                            memset(keyboardReport, 0, 8);
                        }
                        switch (ccReport) {
                        case USB_HID_CONSUMER_MUTE:
                            keyboardReport[ccIndex] = KEY_MUTE;
                            break;
                        case USB_HID_CONSUMER_VOLUME_DECREMENT:
                            keyboardReport[ccIndex] = KEY_VOLUME_DOWN;
                            break;
                        case USB_HID_CONSUMER_VOLUME_INCREMENT:
                            keyboardReport[ccIndex] = KEY_VOLUME_UP;
                            break;
                        default:
                            break;
                        }
                    }
                    HOS_Report(HOS_TYPE_DEFAULT, HOS_CMD_KEYBOARD_REPORT, 8, keyboardReport);
                }
#if APP_HAS_MOUSE_INTERFACE
                TSAP_Task(HOS_GetKeyboardMouseX(), HOS_GetKeyboardMouseY(), HOS_GetTouch());
                // Do not report unchanged state.
                if (MOUSE_GetReport(mouseReport)) {
                    HOS_Report(HOS_TYPE_DEFAULT, HOS_CMD_MOUSE_REPORT, sizeof mouseReport, mouseReport);
                }
#endif
                KEYBOARD_SetLEDs(HOS_GetLED());
                UpdateBatteryLevel(tick);
                break;

            default:
                KEYBOARD_EnableLED(true);
                HOS_GetStatus(HOS_TYPE_INFO);  // Get info after reset.
                NVIC_SystemReset();
                // NOT REACHED HERE
                break;
            }

            if (HOS_GetSuspended() || HOS_GetIndication() == HOS_BLE_STATE_IDLE) {
                WaitForResume();
            }
        }

        PM_StandbyModeEnter();
    }
}
