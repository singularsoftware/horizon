/* ************************************************************************** */
/** MAIN FIRMWARE

  @Company
    Singular Software

  @File Name
    firmware.c

  @Summary
    Main firmware tasks.

  @Description
    Demonstrate the use of the main firmware task.
 */
/* ************************************************************************** */
/*******************************************************************************
* Copyright (C) 2024 Singular Software Limited.
*
* Subject to your compliance with these terms, you may use Singular-Software
* software and any derivatives exclusively with Singular-Software products. 
* It is your responsibility to comply with third party license terms applicable 
* to your use of third party software (including open source software) that may 
* accompany Singular-Software software.
*
* THIS SOFTWARE IS SUPPLIED BY SINGULAR-SOFTWARE "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR 
* PURPOSE.
*
* IN NO EVENT WILL SINGULAR-SOFTWARE BE LIABLE FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF SINGULAR-SOFTWARE
* HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, SINGULAR-SOFTWARE'S TOTAL LIABILITY ON ALL 
* CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES,
* IF ANY, THAT YOU HAVE PAID DIRECTLY TO SINGULAR-SOFTWARE FOR THIS SOFTWARE.
*******************************************************************************/ 


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "firmware.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data                                         
// *****************************************************************************
// *****************************************************************************

FIRMWARE_DATA CACHE_ALIGN firmwareData;

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Callback Functions
// *****************************************************************************
// *****************************************************************************
void UsbKeyboardPrintableKeyPressEventHandler(char c)
{
    if(c == '\r')
    {
        USER_ECHO_TO_CONSOLE_PRINT("\r\n");
    }
    else
    {
        USER_ECHO_TO_CONSOLE_PRINT("%c", c);
    }
}
void UsbKeyboardControlKeyPressEventHandler(char* s)
{
    USER_ECHO_TO_CONSOLE_PRINT(" %s", s);
}
// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void FIRMWARE_Initialize ( void )
{
    firmwareData.state = FIRMWARE_STATE_INIT;
    SYS_ERROR_LEVEL level = SYS_ERROR_FATAL;
    SYS_DEBUG_ErrorLevelSet(level);
}

void FIRMWARE_Tasks ( void )
{
    switch ( firmwareData.state )
    {
        case FIRMWARE_STATE_INIT:
        {
            /* Initialize */
            USB_PrintableKeyPressCallbackRegister(UsbKeyboardPrintableKeyPressEventHandler);
            USB_ControlKeyPressCallbackRegister(UsbKeyboardControlKeyPressEventHandler);
            firmwareData.state = FIRMWARE_STATE_SERVICE_TASKS;
            break;
        }

        case FIRMWARE_STATE_SERVICE_TASKS:
        {
            /* Service */
            break;
        }
        
        default:
        {
            break;
        }
    }
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*******************************************************************************
 End of File
 */
