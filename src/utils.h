/*
 * Copyright (C) 2025 Esrille Inc.
 *
 * This file is based on plib_port.h file provided by Microchip Technology, Inc.
 * See the file NOTICE for copying permission.
 *
 */

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

#ifndef ESRILLE_UTILS_H
#define ESRILLE_UTILS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "peripheral/port/plib_port.h"
#include "peripheral/tc/plib_tc3.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

/*******************************************************************************
  Function:
    void PORT_GroupInputPullEnable(PORT_GROUP group, uint32_t mask)

  Summary:
    Enables internal pull-up or pull-down resistors for a specified group of ports.

  Description:
    This function configures the specified PORT_GROUP to enable internal pull-up
    or pull-down resistors based on the provided mask. The mask determines which
    pins within the group will have the pull resistors enabled.

  Precondition:
    The specified PORT_GROUP must be initialized and configured properly before
    calling this function.

  Parameters:
    - group: The PORT_GROUP to configure (e.g., PORTA, PORTB, etc.).
    - mask: A bitmask indicating which pins in the group to enable the pull
             resistors for. Each bit corresponds to a pin in the group.

  Returns:
    None.

  Example:
    <code>
    PORT_GroupInputPullEnable(PORTA, 0x03); // Enable pull resistors for pins 0 and 1 of PORTA
    </code>

  Remarks:
    Ensure that the pull-up or pull-down configuration is appropriate for the
    intended application, as incorrect settings may lead to unexpected behavior
    in the circuit.
 */
void PORT_GroupPullEnable(PORT_GROUP group, uint32_t mask);

static inline void PORT_PinPullEnable(PORT_PIN pin)
{
  PORT_GroupPullEnable(GET_PORT_GROUP(pin), GET_PIN_MASK(pin));
}

/*******************************************************************************
  Function:
    void TC3_DelayUs(int16_t us)

  Summary:
    Creates a busy-wait delay for a specified number of microseconds.

  Description:
    This function implements a busy-wait loop that blocks the execution
    of the program for a duration specified in microseconds. It uses
    the timer to measure the elapsed time and ensures that the delay
    is accurate to the best extent possible given the system clock.

  Precondition:
    The timer must be initialized and configured properly before calling
    this function.

  Parameters:
    - us: The number of microseconds to wait. The value should be a
          positive integer.

  Returns:
    None.

  Example:
    <code>
    BusyWaitUS(100); // Wait for 100 microseconds
    </code>

  Remarks:
    This function is typically used for short delays where precise timing
    is required. However, it is not recommended for long delays as it
    blocks the CPU and prevents other tasks from executing.
 */
void TC3_DelayUs(int16_t us);

uint16_t isqrt16(uint16_t x);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif // ESRILLE_UTILS_H
