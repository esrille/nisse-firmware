/*
 * Copyright (C) 2025 Esrille Inc.
 *
 * This file, reworked by Esrille Inc., is based on the app.h files originally
 * available from Microchip Technology Inc. in the following repository.
 * See the file NOTICE for copying permission.
 *
 * https://github.com/Microchip-MPLAB-Harmony/usb_apps_device
 *
 */

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"

#include "keyboard.h"
#include "mouse.h"

#include "nisse.h"

#include "eeprom.h"
#include "macro.h"
#include "profile.h"
#include "hos_master.h"
#include "tsap.h"
#include "utils.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define HID_INDEX_KEYBOARD  USB_DEVICE_HID_INDEX_0
#define HID_INDEX_CONSUMER  USB_DEVICE_HID_INDEX_1
#define HID_INDEX_MOUSE     USB_DEVICE_HID_INDEX_2
#define HID_INDEX_COUNT     USB_DEVICE_HID_INSTANCES_NUMBER

/* Defines minimum suspend duration before which Remote Wakeup cannot occur */
#define USB_SUSPEND_DURATION_5MS        5

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum {
    /* Application's state machine's initial state. */
    APP_STATE_INIT = 0,

    /* Application waits for configuration in this state */
    APP_STATE_WAIT_FOR_CONFIGURATION,

    /* Application checks if an output report is available */
    APP_STATE_CHECK_FOR_OUTPUT_REPORT,

    /* Application updates the switch states */
    APP_STATE_SWITCH_PROCESS,

    /* Application checks if it is still configured*/
    APP_STATE_CHECK_IF_CONFIGURED,

    /* Application emulates keyboard */
    APP_STATE_EMULATE_KEYBOARD,

    /* Application emulates consumer device */
    APP_STATE_EMULATE_CONSUMER,

    /* Application emulates mouse */
    APP_STATE_EMULATE_MOUSE,

    /* USB Device is in suspend State */
    APP_STATE_USB_SUSPENDED,

    /* Application has put the Microcontroller in Standby sleep state */
    APP_STATE_MCU_ON_STANDBY,

    /* Application error state */
    APP_STATE_ERROR

} APP_STATES;

/******************************************************
 * HID Event State
 ******************************************************/

typedef struct {
    /* Application HID instance */
    USB_DEVICE_HID_INDEX hidInstance;

    /* Track the send report status */
    bool isReportSentComplete;

    /* Track if a report was received */
    bool isReportReceived;

    /* Receive transfer handle */
    USB_DEVICE_HID_TRANSFER_HANDLE receiveTransferHandle;

    /* Send transfer handle */
    USB_DEVICE_HID_TRANSFER_HANDLE sendTransferHandle;

    /* USB HID active Protocol */
    USB_HID_PROTOCOL_CODE activeProtocol;

    /* USB HID current Idle */
    uint8_t idleRate;
    uint16_t idleTimer;

} APP_HID_OBJECT;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct {
    /* The application's current state */
    APP_STATES state;

    /* Is device configured */
    bool isConfigured;

    /* Base tick timer */
    SYS_TIME_HANDLE tickTimer;

    /* Current tick count */
    volatile uint16_t tick;

    /*
     * USB device state
     */

    /* Handle to the device layer */
    USB_DEVICE_HANDLE deviceHandle;

    /*
     * Remote wakeup support
     */

    /* Flag determines whether USB device is suspended or not */
    bool isSuspended;

    /* Flag determines whether USB device is still attached or not */
    bool isAttached;

    /* System timer handle */
    SYS_TIME_HANDLE tmrHandle;

    /* Flag to track the timer status */
    bool tmrExpired;

    bool remoteWakeUpInProgress;

    bool wakeUp;

    /*
     * HID objects
     */
    APP_HID_OBJECT hidObjects[HID_INDEX_COUNT];

} APP_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize(void);

/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks(void);

/******************************************************************************
  Function:
    void APP_ResetTick( void )

  Description:
    This function resets the application tick counter to zero.

   Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_ResetTick();
    </code>

 */

void APP_ResetTick(void);

/*******************************************************************************
  Function:
    uint16_t APP_GetTick ( void )

  Summary:
    Retrieves the current tick count of the application.

  Description:
    This function returns the current tick count, which can be used for
    timing purposes within the application. The tick count is incremented
    at regular intervals by a timer.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this function.

  Parameters:
    None.

  Returns:
    uint16_t - The current tick count.

  Example:
    <code>
    uint16_t currentTick = APP_GetTick();
    </code>

  Remarks:
    This function can be called at any time after initialization to get
    the current tick count.
 */
uint16_t APP_GetTick(void);

/*******************************************************************************
  Function:
    void APP_WakeUp(void)

  Summary:
    Wakes up the application from a low-power state.

  Description:
    This function sets the wake-up request flag to initiate application resume
    from low-power mode.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this function.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_WakeUp();
    </code>

  Remarks:
    This function should be called when the application needs to resume
    operation after being in a low-power state.
 */
void APP_WakeUp(void);

/*******************************************************************************
  Function:
    bool APP_Suspended(void)

  Summary:
    Checks if the application is in a suspended state.

  Description:
    This function checks the current state of the application to determine
    if it is in a suspended state (i.e., the microcontroller is in standby mode).
    It returns true if the application is suspended, and false otherwise.

  Precondition:
    The system and application initialization ("SYS_Initialize") should
    be called before calling this function.

  Parameters:
    None.

  Returns:
    - true: if the application is in a suspended state.
    - false: if the application is not suspended.

  Example:
    <code>
    if (APP_Suspended()) {
        // Handle suspended state
    }
    </code>

  Remarks:
    This function is useful for managing power consumption and ensuring
    that the application behaves correctly when in low-power modes.
 */
bool APP_Suspended(void);

/******************************************************************************
  Function:
    bool APP_HasMouseInterface(void)

  Description:
    This function checks whether a mouse device is combined with the keyboard.

   Parameters:
    None.

  Returns:
    - true: if a mouse device is combined with the keyboard.
    - false: otherwise.

  Example:
    <code>
    bool hasMouse = APP_HasMouseInterface();
    </code>
 */

static inline bool APP_HasMouseInterface(void)
{
    return (HID_INDEX_MOUSE < USB_DEVICE_HID_INSTANCES_NUMBER);
}

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */

/*******************************************************************************
 End of File
 */
