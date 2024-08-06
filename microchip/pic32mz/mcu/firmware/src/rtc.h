/* ************************************************************************** */
/** Real Time Clock

  @Company
    Singular Software

  @File Name
    rtc.h

  @Summary
    Real time clock access.

  @Description
    Demonstrate the use of the real time clock.
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


#ifndef _RTC_H /* Guard against multiple inclusion */
#define _RTC_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files                                                    
// *****************************************************************************
// *****************************************************************************

#include "definitions.h" 

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Constants                                                         
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Callback Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void RTC_Initialize( void );

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
void RTC_CallbackRegister( RTCC_CALLBACK callback, uintptr_t context );
bool RTC_SetDateTime(struct tm *sys_time);
void RTC_GetDateTime(struct tm *sys_time);
bool RTC_AlarmSetDateTime(struct tm *alarm_time, RTCC_ALARM_MASK rtccAlarmMask);


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _RTC_H */

/* *****************************************************************************
 End of File
 */
