/*
 * Copyright (C) 2025 Esrille Inc.
 *
 * This file is based on plib_port.c file provided by Microchip Technology, Inc.
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

#include "utils.h"

/*****************************************************************************
  Function:
    void PORT_GroupInputPullEnable(PORT_GROUP group, uint32_t mask)

  Summary:
    Configures the selected IO pins of a specified port group as input with pull-up
    resistors enabled.

  Description:
    This function configures the selected IO pins of a specified port group as input.
    The pins to be configured as input are determined by the `mask` parameter, where
    each bit corresponds to a pin in the group. A bit set to 1 in the mask indicates
    that the corresponding pin should be configured as an input. Additionally, this
    function enables the internal pull-up resistors for the configured pins.

  Remarks:
    Refer plib_port.h file for more information.
*/

void PORT_GroupPullEnable(PORT_GROUP group, uint32_t mask)
{
    ((port_group_registers_t*)group)->PORT_DIRCLR = mask;

    for(uint32_t i = 0U; i < 32U; i++)
    {
        if((mask & ((uint32_t)1U << i)) != 0U)
        {
            ((port_group_registers_t*)group)->PORT_PINCFG[i] |= PORT_PINCFG_PULLEN_Msk;
        }
    }
}

/******************************************************************************
  Function:
    void TC3_DelayUs(int16_t us)

  Description:
    This function creates a busy-wait delay for a specified number of microseconds.
    It uses a timer to count the elapsed time and blocks execution until the
    desired delay has been reached. This is a simple but inefficient way to
    implement a delay, as it keeps the CPU busy during the wait.

  Parameters:
    int16_t us - The number of microseconds to wait. The value should be positive
                 and represent the desired delay duration.

  Remarks:
    This function may not be suitable for low-power applications, as it prevents
    the CPU from entering a low-power state during the wait period. Consider using
    a timer interrupt or other methods for more efficient delay handling.

 */

void TC3_DelayUs(int16_t us)
{
    uint16_t startCount = TC3_Timer16bitCounterGet();
    int16_t targetTicks = (TC3_TimerFrequencyGet() * us / 1000000UL);

    while ((TC3_Timer16bitCounterGet() - startCount) < targetTicks)
        ;
}

uint16_t isqrt16(uint16_t x)
{
    uint16_t r = 0, bit = 1u << 14;
    while (bit > x)
        bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}
