/*
 * Copyright (C) 2025 Esrille Inc.
 *
 * This file, reworked by Esrille Inc., is based on the app.c files originally
 * available from Microchip Technology Inc. in the following repository.
 * See the file NOTICE for copying permission.
 *
 * https://github.com/Microchip-MPLAB-Harmony/usb_apps_device
 *
 */

/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Application Function prototypes
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

/*Keyboard Report to be transmitted*/
KEYBOARD_INPUT_REPORT __attribute__((aligned(16))) keyboardInputReport USB_ALIGN;
/* Keyboard output report */
KEYBOARD_OUTPUT_REPORT __attribute__((aligned(16))) keyboardOutputReport USB_ALIGN;

CONSUMER_REPORT __attribute__((aligned(16))) consumerReport USB_ALIGN;

MOUSE_REPORT __attribute__((aligned(16))) mouseReport USB_ALIGN;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

USB_DEVICE_HID_EVENT_RESPONSE APP_USBDeviceHIDEventHandler
(
    USB_DEVICE_HID_INDEX hidInstance,
    USB_DEVICE_HID_EVENT event,
    void * eventData,
    uintptr_t userData
)
{
    APP_DATA * appDataObject = (APP_DATA *)userData;

    switch(event)
    {
        case USB_DEVICE_HID_EVENT_REPORT_SENT:

            /* This means the mouse report was sent.
             We are free to send another report */

            appDataObject->hidObjects[hidInstance].isReportSentComplete = true;
            break;

        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:

            /* This means we have received a report */
            appDataObject->hidObjects[hidInstance].isReportReceived = true;
            break;

        case USB_DEVICE_HID_EVENT_SET_IDLE:

             /* Acknowledge the Control Write Transfer */
           USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            /* save Idle rate received from Host */
            appDataObject->hidObjects[hidInstance].idleRate = ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)eventData)->duration;
            break;

        case USB_DEVICE_HID_EVENT_GET_IDLE:

            /* Host is requesting for Idle rate. Now send the Idle rate */
            USB_DEVICE_ControlSend(appDataObject->deviceHandle, &(appDataObject->hidObjects[hidInstance].idleRate),1);

            /* On successfully receiving Idle rate, the Host would acknowledge back with a
               Zero Length packet. The HID function driver returns an event
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the application upon
               receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates this control transfer
               event is complete */

            break;

        case USB_DEVICE_HID_EVENT_SET_PROTOCOL:
            /* Host is trying set protocol. Now receive the protocol and save */
            appDataObject->hidObjects[hidInstance].activeProtocol
                = ((USB_DEVICE_HID_EVENT_DATA_SET_PROTOCOL *)eventData)->protocolCode;

              /* Acknowledge the Control Write Transfer */
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case  USB_DEVICE_HID_EVENT_GET_PROTOCOL:

            /* Host is requesting for Current Protocol. Now send the Idle rate */
             USB_DEVICE_ControlSend(appDataObject->deviceHandle, &(appDataObject->hidObjects[hidInstance].activeProtocol), 1);

             /* On successfully receiving Idle rate, the Host would acknowledge
               back with a Zero Length packet. The HID function driver returns
               an event USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the
               application upon receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates
               this control transfer event is complete */
             break;

        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT:
            break;

        default:
            break;
    }

    return USB_DEVICE_HID_EVENT_RESPONSE_NONE;
}

/*******************************************************************************
  Function:
    void APP_USBDeviceEventHandler (USB_DEVICE_EVENT event,
        USB_DEVICE_EVENT_DATA * eventData)

  Summary:
    Event callback generated by USB device layer.

  Description:
    This event handler will handle all device layer events.

  Parameters:
    None.

  Returns:
    None.
 */

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event,
        void * eventData, uintptr_t context)
{
    USB_DEVICE_EVENT_DATA_CONFIGURED *configurationValue;

    switch(event)
    {
        case USB_DEVICE_EVENT_SOF:
            for (USB_DEVICE_HID_INDEX i = 0; i < HID_INDEX_COUNT; ++i)
            {
                if (0 < appData.hidObjects[i].idleTimer) {
                    --appData.hidObjects[i].idleTimer;
                }
            }
            break;

        case USB_DEVICE_EVENT_RESET:
            appData.isSuspended = false;

        case USB_DEVICE_EVENT_DECONFIGURED:

            /* Device got de-configured */

            appData.isConfigured = false;
            appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;

            KEYBOARD_EnableLED(false);
            KEYBOARD_SetLEDs(0);

            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Device is configured */

            configurationValue = (USB_DEVICE_EVENT_DATA_CONFIGURED *)eventData;
            if(configurationValue->configurationValue == 1)
            {
                appData.isConfigured = true;

                KEYBOARD_SetLEDs(0);
                KEYBOARD_EnableLED(true);

                /* Register the Application HID Event Handler. */
                for (USB_DEVICE_HID_INDEX i = 0; i < HID_INDEX_COUNT; ++i)
                {
                    USB_DEVICE_HID_EventHandlerSet(i, APP_USBDeviceHIDEventHandler, (uintptr_t)&appData);
                }
            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            KEYBOARD_EnableLED(false);
            /* Device is suspended. */
            appData.isSuspended = true;
            break;

        case USB_DEVICE_EVENT_RESUMED:
            if(appData.isConfigured == true)
            {
                KEYBOARD_EnableLED(true);
            }
            /* Device is resumed. */
            appData.isSuspended = false;
            appData.remoteWakeUpInProgress = false;
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* Attach the device */
            USB_DEVICE_Attach(appData.deviceHandle);
            appData.isAttached = true;
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* There is no VBUS. We can detach the device */
            USB_DEVICE_Detach(appData.deviceHandle);
            KEYBOARD_EnableLED(false);
            appData.isAttached = false;
            break;

        case USB_DEVICE_EVENT_ERROR:
        default:

            break;

    }
}

void APP_Timer_Callback ( uintptr_t context )
{
    appData.tmrExpired = true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/********************************************************
 * Application Keyboard LED update routine.
 ********************************************************/

void APP_KeyboardLEDStatus(void)
{
    /* This means we have a valid output report from the host*/
    KEYBOARD_SetLEDs(keyboardOutputReport.data[0]);
}

/**********************************************
 * This function is called by when the device
 * is de-configured. It resets the application
 * state in anticipation for the next device
 * configured event
 **********************************************/

void APP_StateReset(void)
{
    for (USB_DEVICE_HID_INDEX i = 0; i < HID_INDEX_COUNT; ++i) {
        appData.hidObjects[i].isReportReceived = false;
        appData.hidObjects[i].isReportSentComplete = true;
    }
    memset(&keyboardOutputReport.data, 0, 64);
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

static void TickCallback( uintptr_t context)
{
    ++appData.tick;
}

void APP_Initialize ( void )
{
    /* Place the application state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID;
    appData.isConfigured = false;

    /* Initialize the led state */
    memset(keyboardOutputReport.data, 0, 64);

    /* Initialize remote wakeup state */
    appData.tmrExpired = false;
    appData.isAttached = false;
    appData.isSuspended = false;
    appData.remoteWakeUpInProgress = false;
    appData.wakeUp = false;

    /* Initialize HID objects */
    for (USB_DEVICE_HID_INDEX i = 0; i < HID_INDEX_COUNT; ++i) {
        APP_HID_OBJECT* instance = &appData.hidObjects[i];

        instance->hidInstance = i;
        instance->isReportReceived = false;
        instance->isReportSentComplete = true;
        instance->idleRate = 4 * APP_USB_CONVERT_TO_MILLISECOND;
        instance->idleTimer = 0;
    }

    /* Initialize and start the tick timer */
    appData.tick = 0;
    appData.tickTimer = SYS_TIME_TimerCreate(0, SYS_TIME_MSToCount(16), TickCallback, (uintptr_t) NULL, SYS_TIME_PERIODIC);
    SYS_TIME_TimerStart(appData.tickTimer);
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    /* Flag to store the CPU interrupt state when it enters critical section */
    bool interruptStatus;
    APP_HID_OBJECT* instance = NULL;

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            /* Open the device layer */
            appData.deviceHandle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);

            if(appData.deviceHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle, APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }
            break;
        }

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            /* Check if the device is configured. The
             * isConfigured flag is updated in the
             * Device Event Handler */

            if(appData.isConfigured)
            {
                /* Initialize the flag and place a request for a keyboard
                 * output report */
                instance = &appData.hidObjects[HID_INDEX_KEYBOARD];
                instance->isReportReceived = false;
                USB_DEVICE_HID_ReportReceive(HID_INDEX_KEYBOARD,
                        &instance->receiveTransferHandle,
                        (uint8_t *)&keyboardOutputReport,64);

                appData.state = APP_STATE_CHECK_IF_CONFIGURED;
            }

            break;

        case APP_STATE_CHECK_IF_CONFIGURED:

            /* This state is needed because the device can get
             * unconfigured asynchronously. Any application state
             * machine reset should happen within the state machine
             * context only. */

            if(appData.isConfigured)
            {
                appData.state = APP_STATE_CHECK_FOR_OUTPUT_REPORT;
            }
            else
            {
                /* This means the device got de-configured.
                 * We reset the state and the wait for configuration */

                APP_StateReset();
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            break;

        case APP_STATE_CHECK_FOR_OUTPUT_REPORT:

            instance = &appData.hidObjects[HID_INDEX_KEYBOARD];
            if(instance->isReportReceived == true)
            {
                /* Update the LED and schedule and
                 * request */

                APP_KeyboardLEDStatus();

                instance->isReportReceived = false;
                USB_DEVICE_HID_ReportReceive(instance->hidInstance,
                        &instance->receiveTransferHandle,
                        (uint8_t *)&keyboardOutputReport,64);
            }

            if (appData.isSuspended == true)
            {
                /* USB Device is suspended. */
                appData.state = APP_STATE_USB_SUSPENDED;

                SYS_CONSOLE_MESSAGE("USB Device Suspended\r\n");
            }
            else
            {
                appData.state = APP_STATE_EMULATE_KEYBOARD;
            }
            break;

        case APP_STATE_EMULATE_KEYBOARD:

            instance = &appData.hidObjects[HID_INDEX_KEYBOARD];
            if(instance->isReportSentComplete && instance->idleTimer == 0)
            {
                /* This means report can be sent */
                if (KEYBOARD_GetReport(keyboardInputReport.data)) {
                    instance->isReportSentComplete = false;
                    instance->idleTimer = instance->idleRate * APP_USB_CONVERT_TO_MILLISECOND;
                    USB_DEVICE_HID_ReportSend(instance->hidInstance,
                        &instance->sendTransferHandle,
                        keyboardInputReport.data,
                        sizeof(KEYBOARD_INPUT_REPORT));
                }
            }
            appData.state = APP_STATE_EMULATE_CONSUMER;
            break;

        case APP_STATE_EMULATE_CONSUMER:

            instance = &appData.hidObjects[HID_INDEX_CONSUMER];
            if(instance->isReportSentComplete && instance->idleTimer == 0)
            {
                /* This means we can send the consumer report. */
                if (KEYBOARD_GetConsumerReport(consumerReport.data)) {
                    instance->isReportSentComplete = false;
                    instance->idleTimer = instance->idleRate * APP_USB_CONVERT_TO_MILLISECOND;
                    USB_DEVICE_HID_ReportSend(instance->hidInstance,
                        &instance->sendTransferHandle,
                        consumerReport.data,
                        sizeof(CONSUMER_REPORT));
                }
            }

            if (APP_HasMouseInterface())
                appData.state = APP_STATE_EMULATE_MOUSE;
            else
                appData.state = APP_STATE_CHECK_IF_CONFIGURED;

            break;

        case APP_STATE_EMULATE_MOUSE:

            instance = &appData.hidObjects[HID_INDEX_MOUSE];
            if(instance->isReportSentComplete && instance->idleTimer == 0)
            {
                /* This means we can send the mouse report. */
                if (MOUSE_GetReport(mouseReport.data)) {
                    instance->isReportSentComplete = false;
                    instance->idleTimer = instance->idleRate * APP_USB_CONVERT_TO_MILLISECOND;
                    USB_DEVICE_HID_ReportSend(instance->hidInstance,
                        &instance->sendTransferHandle,
                        mouseReport.data,
                        sizeof(MOUSE_REPORT));
                }
            }
            appData.state = APP_STATE_CHECK_IF_CONFIGURED;
            break;

        case APP_STATE_USB_SUSPENDED:

            /* USB Device is suspended. This could be due to two reason:
             * The UI on PC host is not active, so PC host suspends the device.
             * If so, Remote wake up will not be enabled.
             * The PC is put to Sleep and hence it suspends the device.
             * In this case Remote Wake up is enabled. Check this and display
             * appropriate message. */
            if(USB_DEVICE_RemoteWakeupStatusGet(appData.deviceHandle) == USB_DEVICE_REMOTE_WAKEUP_ENABLED)
            {
                SYS_CONSOLE_MESSAGE("USB host in Sleep mode - Remote wakeup enabled. Press Button to wakeup Host.\r\n");
            }
            else
            {
                /* There are also chances that the USB Device is not allowed to
                 * Remote wake up the PC Host. This can be changed in the PC
                 * Sleep settings in order to do a remote wake up from USB Device */
                SYS_CONSOLE_MESSAGE("USB host Suspend device or Remote wakeup is not enabled\r\n");
            }

            /* Device goes to Standby Sleep mode in either of the cases */
            SYS_CONSOLE_MESSAGE("MCU on Standby Sleep mode\r\n");

            /* Reset the timer status flag */
            appData.tmrExpired = false;

            /* Start the timer for 5ms. This is to ensure Remote wakeup is
             * initiated after 5ms of suspend time - as per spec */
            appData.tmrHandle = SYS_TIME_CallbackRegisterMS(APP_Timer_Callback, 0, USB_SUSPEND_DURATION_5MS, SYS_TIME_SINGLE);

            /* Change the Application State */
            appData.state = APP_STATE_MCU_ON_STANDBY;

            break;

        case APP_STATE_MCU_ON_STANDBY:

#ifdef SYS_CONSOLE_DEFAULT_INSTANCE
            /* Ensure that the SYS Console has transmitted all the bytes in queue */
            while(SYS_CONSOLE_WriteCountGet(SYS_CONSOLE_DEFAULT_INSTANCE) != 0)
                ;
#endif

            /* Disable the system interrupt before going to Standby Mode */
            interruptStatus = NVIC_INT_Disable();

            /* Disable SERCOM interrupts */
            //DISABLE_SERCOM_INTERRUPT();

            //SYSTICK_TimerInterruptDisable();

            /* We must ensure that no wake up interrupt is coming before entering in standby */
            if(appData.isSuspended == false)
            {
                /* USB activity is seen on the bus */
                SYS_CONSOLE_MESSAGE("USB Device Resumed\r\n");

                /* Restore the system interrupt state when exiting the Standby Mode */
                NVIC_INT_Restore(interruptStatus);

                //SYSTICK_TimerInterruptEnable();

                /* Enable SERCOM interrupts */
                //ENABLE_SERCOM_INTERRUPT();

                /* Go back to executing Main HID tasks */
                appData.state = APP_STATE_CHECK_IF_CONFIGURED;
            }
            else
            {
                /* Enter Standby Mode */
                PM_StandbyModeEnter();

                /* Restore the system interrupt state when exiting the Standby Mode */
                NVIC_INT_Restore(interruptStatus);

                //SYSTICK_TimerInterruptEnable();

                /* Enable SERCOM interrupts */
                //ENABLE_SERCOM_INTERRUPT();

                /* There could be only two wakeup sources. USB activity by Host
                 * or User Switch Press */
                if(appData.isSuspended == false)
                {
                    /* USB activity is seen on the bus */
                    SYS_CONSOLE_MESSAGE("USB Device Resumed\r\n");

                    appData.wakeUp = false;

                    /* Go back to executing Main HID tasks */
                    appData.state = APP_STATE_CHECK_IF_CONFIGURED;
                }
                else if((appData.wakeUp == true) && (appData.tmrExpired == true) && (appData.remoteWakeUpInProgress == false))
                {
                    appData.wakeUp = false;

                    /* User has pressed the Switch. This is to wake up the USB Host */
                    if(USB_DEVICE_RemoteWakeupStatusGet(appData.deviceHandle) == USB_DEVICE_REMOTE_WAKEUP_ENABLED)
                    {
                        /* PC host has enabled Remote Wakeup by USB Device, so,
                         * initiate a Remote Wakeup Start and go back to Standby
                         * Sleep Mode and wait for PC host to drive USB device
                         * resume. */
                        /*A 10ms delay has been added between the remote wakeup
                        start and stop function as per the USB Spec*/
                        USB_DEVICE_RemoteWakeupStart(appData.deviceHandle);
                        appData.remoteWakeUpInProgress = true;
                        SYS_TIME_HANDLE timeh;
                        SYS_TIME_DelayMS ( 10,  &timeh );
                        while(SYS_TIME_DelayIsComplete(timeh) != true)
                            ;
                        USB_DEVICE_RemoteWakeupStop(appData.deviceHandle);
                        SYS_CONSOLE_MESSAGE("Remote Wakeup Start Sent\r\n");
                    }
                    else
                    {
                        /* PC host has not enabled Remote Wakeup by USB Device.
                         * This has to be changed by modifying the sleep settings
                         * of PC host. Just display the message and go to Standby
                         * Sleep mode. */
                        SYS_CONSOLE_MESSAGE("Remote wakeup not enabled by PC host. Check PC Sleep mode settings\r\n");
                    }
                }
                else if (appData.isAttached == false)
                {
                    /* This means USB device is detached from the bus. */
                    SYS_CONSOLE_MESSAGE("USB Device detached\r\n");

                    appData.wakeUp = false;

                    /* Update the configuration flag. */
                    appData.isConfigured = false;

                    /* Device is detached. Change the Application State */
                    appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
                }
            }
            break;

        case APP_STATE_ERROR:
            break;

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

/******************************************************************************
  Function:
    void APP_ResetTick( void )

  Description:
    This function resets the application tick counter to zero.
 */

void APP_ResetTick( void )
{
    appData.tick = 0;
}

/******************************************************************************
  Function:
    uint32_t APP_GetTick ( void )

  Description:
    This function retrieves the current tick count from the system timer,
    which is updated by the `TickCallback` function.

  Parameters:
    None

  Returns:
    uint16_t - The current tick count, which is incremented by the
               `TickCallback` at regular intervals.

 */

uint16_t APP_GetTick( void )
{
    return appData.tick;
}

/******************************************************************************
  Function:
    void APP_WakeUp ( void )

  Description:
    This function sets the wake-up request flag to initiate application resume
    from low-power mode.
 */

 void APP_WakeUp( void )
{
    appData.wakeUp = true;
}

/*******************************************************************************
  Function:
    bool APP_Suspended( void )

  Description:
    This function checks the current state of the application to determine if
    the microcontroller is in a low-power standby state. It returns true if
    the application is suspended, and false otherwise.
 */

bool APP_Suspended( void )
{
    return appData.state == APP_STATE_MCU_ON_STANDBY;
}

/*******************************************************************************
 End of File
 */
