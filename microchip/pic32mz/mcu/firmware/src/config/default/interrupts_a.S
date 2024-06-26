/*******************************************************************************
  System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include "ISR_Support.h"

    .extern  CORE_TIMER_Handler

    .section   .vector_0,code, keep
    .equ     __vector_dispatch_0, IntVectorCORE_TIMER_Handler
    .global  __vector_dispatch_0
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorCORE_TIMER_Handler

IntVectorCORE_TIMER_Handler:
    portSAVE_CONTEXT
    la    s6,  CORE_TIMER_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorCORE_TIMER_Handler
    .extern  CHANGE_NOTICE_H_Handler

    .section   .vector_125,code, keep
    .equ     __vector_dispatch_125, IntVectorCHANGE_NOTICE_H_Handler
    .global  __vector_dispatch_125
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorCHANGE_NOTICE_H_Handler

IntVectorCHANGE_NOTICE_H_Handler:
    portSAVE_CONTEXT
    la    s6,  CHANGE_NOTICE_H_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorCHANGE_NOTICE_H_Handler
    .extern  USB_Handler

    .section   .vector_132,code, keep
    .equ     __vector_dispatch_132, IntVectorUSB_Handler
    .global  __vector_dispatch_132
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUSB_Handler

IntVectorUSB_Handler:
    portSAVE_CONTEXT
    la    s6,  USB_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUSB_Handler
    .extern  USB_DMA_Handler

    .section   .vector_133,code, keep
    .equ     __vector_dispatch_133, IntVectorUSB_DMA_Handler
    .global  __vector_dispatch_133
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUSB_DMA_Handler

IntVectorUSB_DMA_Handler:
    portSAVE_CONTEXT
    la    s6,  USB_DMA_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUSB_DMA_Handler
    .extern  I2C3_BUS_Handler

    .section   .vector_160,code, keep
    .equ     __vector_dispatch_160, IntVectorI2C3_BUS_Handler
    .global  __vector_dispatch_160
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorI2C3_BUS_Handler

IntVectorI2C3_BUS_Handler:
    portSAVE_CONTEXT
    la    s6,  I2C3_BUS_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorI2C3_BUS_Handler
    .extern  I2C3_MASTER_Handler

    .section   .vector_162,code, keep
    .equ     __vector_dispatch_162, IntVectorI2C3_MASTER_Handler
    .global  __vector_dispatch_162
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorI2C3_MASTER_Handler

IntVectorI2C3_MASTER_Handler:
    portSAVE_CONTEXT
    la    s6,  I2C3_MASTER_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorI2C3_MASTER_Handler
    .extern  SQI1_Handler

    .section   .vector_169,code, keep
    .equ     __vector_dispatch_169, IntVectorSQI1_Handler
    .global  __vector_dispatch_169
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorSQI1_Handler

IntVectorSQI1_Handler:
    portSAVE_CONTEXT
    la    s6,  SQI1_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorSQI1_Handler
    .extern  SPI5_RX_Handler

    .section   .vector_177,code, keep
    .equ     __vector_dispatch_177, IntVectorSPI5_RX_Handler
    .global  __vector_dispatch_177
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorSPI5_RX_Handler

IntVectorSPI5_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  SPI5_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorSPI5_RX_Handler
    .extern  SPI5_TX_Handler

    .section   .vector_178,code, keep
    .equ     __vector_dispatch_178, IntVectorSPI5_TX_Handler
    .global  __vector_dispatch_178
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorSPI5_TX_Handler

IntVectorSPI5_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  SPI5_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorSPI5_TX_Handler
    .extern  UART6_FAULT_Handler

    .section   .vector_188,code, keep
    .equ     __vector_dispatch_188, IntVectorUART6_FAULT_Handler
    .global  __vector_dispatch_188
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART6_FAULT_Handler

IntVectorUART6_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART6_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART6_FAULT_Handler
    .extern  UART6_RX_Handler

    .section   .vector_189,code, keep
    .equ     __vector_dispatch_189, IntVectorUART6_RX_Handler
    .global  __vector_dispatch_189
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART6_RX_Handler

IntVectorUART6_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART6_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART6_RX_Handler
    .extern  UART6_TX_Handler

    .section   .vector_190,code, keep
    .equ     __vector_dispatch_190, IntVectorUART6_TX_Handler
    .global  __vector_dispatch_190
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART6_TX_Handler

IntVectorUART6_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART6_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART6_TX_Handler

