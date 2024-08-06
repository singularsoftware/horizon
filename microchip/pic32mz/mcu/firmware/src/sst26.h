/* ************************************************************************** */
/** SST26VF032B FLASH

  @Company
    Singular Software

  @File Name
    sst26.h

  @Summary
    SST26 32Mbit flash access.

  @Description
    Demonstrate the use of the on-board SST26 32Mbit flash.
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


#ifndef _SST26_H /* Guard against multiple inclusion */
#define _SST26_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "config/default/driver/sst26/drv_sst26.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Constants                                                         
// *****************************************************************************
// *****************************************************************************
    
#define PAGE_SIZE     256
    
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/* SST26 Flash Task States */
typedef enum
{
    /* Flash initial state. */
    SST26_STATE_INIT,
    /* Open the flash driver */
    SST26_STATE_OPEN_DRIVER,
    /* Get flash device geometry */
    SST26_STATE_GEOMETRY_GET,
    /* Flash is ready*/
    SST26_STATE_READY,
    /* Erase flash */
    SST26_STATE_ERASE_FLASH,
    /* Erase flash wait */
    SST26_STATE_ERASE_WAIT,
    /* Write to flash */
    SST26_STATE_WRITE_MEMORY,
    /* Write flash Wait */
    SST26_STATE_WRITE_WAIT,
    /* Read from flash */
    SST26_STATE_READ_MEMORY,
    /* Read flash wait */
    SST26_STATE_READ_WAIT,
    /* Flash close */
    SST26_STATE_CLOSE,
    /* Wait for request */
    SST26_STATE_IDLE,
    /* An error has occurred */
    SST26_STATE_ERROR
} SST26_STATES;

/* SST26 Flash Task Data */
typedef struct
{
    /* Current state */
    SST26_STATES  state;
    /* Driver handle */
    DRV_HANDLE handle;
    /* SST26 device geometry */
    DRV_SST26_GEOMETRY geometry;
    /* Jedec-ID*/
    uint32_t jedec_id;
    /* Read/Write SST26 address */
    uint32_t address;
    /* counters */
    uint32_t dataSize;
    uint32_t eraseIndex;
    uint32_t eraseBlockCount;
    uint32_t writeIndex;
    /* Read buffer */
    uint8_t readBuffer[PAGE_SIZE];
    /* Write buffer*/
    uint8_t writeBuffer[PAGE_SIZE];
    /* Last status value (in case of error)*/
    DRV_SST26_TRANSFER_STATUS lastStatus;
    /* Read/Write test buffer */
    uint8_t testBuffer[PAGE_SIZE*2];
    /* Show progress flag*/
    bool showProgress;
} SST26_DATA;


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
void SST26_Initialize ( void );
void SST26_Tasks( void );

// *****************************************************************************
// *****************************************************************************
// Section: API functions 
// *****************************************************************************
// *****************************************************************************
bool SST26_Ready();
bool SST26_Error();
uint32_t SST26_BlockSize();
uint32_t SST26_GetLastStatus();

bool SST26_Erase(uint32_t address, uint32_t blockCount);
bool SST26_Write(uint8_t* buffer, uint32_t size, uint32_t address);
bool SST26_Read(uint8_t* buffer, uint32_t size, uint32_t address);


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SST26_H */

/*******************************************************************************
 End of File
 */