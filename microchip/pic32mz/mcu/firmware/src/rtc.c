/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Singular Software

  @File Name
    rtcc.c

  @Summary
   Real time clock set/get source code

  @Description
    Describe the purpose of this file.
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
#include "rtc.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data                                         
// *****************************************************************************
// *****************************************************************************

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
void RTC_Initialize ( void )
{
}

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions                                               
// *****************************************************************************
// *****************************************************************************
void RTC_CallbackRegister( RTCC_CALLBACK callback, uintptr_t context )
{
    RTCC_CallbackRegister(callback, context);
}

bool RTC_SetDateTime(struct tm *sys_time)
{
    return RTCC_TimeSet(sys_time);
}

void RTC_GetDateTime(struct tm *sys_time)
{
    RTCC_TimeGet(sys_time);
}
    
bool RTC_AlarmSetDateTime(struct tm *alarm_time, RTCC_ALARM_MASK rtccAlarmMask)
{
    return RTCC_AlarmSet(alarm_time, rtccAlarmMask);
}
    
/* *****************************************************************************
 End of File
 */
