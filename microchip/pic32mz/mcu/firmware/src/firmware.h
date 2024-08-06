/* ************************************************************************** */
/** MAIN FIRMWARE

  @Company
    Singular Software

  @File Name
    firmware.h

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


#ifndef _FIRMWARE_H /* Guard against multiple inclusion */
#define _FIRMWARE_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"

// Provide C++ Compatibility
#ifdef __cplusplus  
extern "C" {
#endif
    
// *****************************************************************************
// *****************************************************************************
// Section: Constants                                                         
// *****************************************************************************
// *****************************************************************************

#define FIRMWARE_VERSION_NUMBER "v1.0.0"
#define FIRMWARE_VERSION_MAJOR 1
#define FIRMWARE_VERSION_MINOR 0
#define FIRMWARE_VERSION_BUILD 0
    
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

typedef enum
{
    /* Initial state. */
    FIRMWARE_STATE_INIT=0,
    FIRMWARE_STATE_SERVICE_TASKS,

} FIRMWARE_STATES;


typedef struct
{
    /* Current state */
    FIRMWARE_STATES state;

} FIRMWARE_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void FIRMWARE_Initialize ( void );
void FIRMWARE_Tasks( void );

// *****************************************************************************
// *****************************************************************************
// Section: API functions 
// *****************************************************************************
// *****************************************************************************

// Provide C++ Compatibility
#ifdef __cplusplus
}
#endif

#endif /* _FIRMWARE_H */

/*******************************************************************************
 End of File
 */

