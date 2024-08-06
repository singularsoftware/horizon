/* ************************************************************************** */
/** SD-CARD

  @Company
    Singular Software

  @File Name
    sdcard.h

  @Summary
    SD-Card access.

  @Description
    Demonstrate the use of the on-board SD-Card.
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


#ifndef _SDCARD_H /* Guard against multiple inclusion */
#define _SDCARD_H

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
#include "definitions.h"

// Provide C++ Compatibility
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
/* SD Card task states */
typedef enum
{
    /* SD-Card initial state. */
    SDCARD_STATE_INIT,
    /* IS SD-Card attach?. */
    SDCARD_STATE_CHECK_FS_DEVICE_ATTACH,
    /* IS SD-Card attach?. */
    SDCARD_STATE_FS_DEVICE_ATACHED,
    /* SD-Card detached. */
    SDCARD_STATE_FS_DEVICE_DETACHED,
    /* Mount file system. */
    SDCARD_STATE_MOUNT_DISK,
    /* Open file request */
    SDCARD_STATE_OPEN_FILE,
    /* Write file request */
    SDCARD_STATE_WRITE_FILE,
    /* Read file request */
    SDCARD_STATE_READ_FILE,
    /* Close file request */
    SDCARD_STATE_CLOSE_FILE,
    /* Delete file request */
    SDCARD_STATE_DELETE_FILE,
    /* Unmount file system. */
    SDCARD_STATE_UNMOUNT_DISK,
    /* Wait for request */
    SDCARD_STATE_IDLE_TASK,
    /* An error has occurred */
    SDCARD_STATE_ERROR
} SDCARD_STATES;

/* SD-Card Task Data */
typedef struct
{
    /* Current state */
    volatile SDCARD_STATES state;
    /* File handle */
    SYS_FS_HANDLE fileHandle;
     /* Current opened file size */
    int32_t fileSize;
    /* File state */
    SYS_FS_FSTAT fileStat;
    /* SD-card attached state*/
    bool deviceAttached;
    /* Last error value */
    SYS_FS_ERROR lastError;
    /* Create new file flag */
    bool newFile;
    /* Flush file after write */
    bool fileSync;
    /* Requested write count*/
    size_t fileDataWriteSize;
    /* Written count*/
    size_t fileDataWrittenSize;
    /* Requested read count*/
    size_t fileDataReadSize1;
    /* Read count*/
    size_t fileDataReadSizeRead;
    /* Buffers */
    uint8_t fileData[256];
    uint8_t fileName[256];
    
} SDCARD_DATA;

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
void SDCARD_Initialize ( void );
void SDCARD_Tasks( void );

// *****************************************************************************
// *****************************************************************************
// Section: API functions 
// *****************************************************************************
// *****************************************************************************
uint8_t SDCARD_GetLastError();
int32_t SDCARD_FileSize();
bool SDCARD_OpenFile(uint8_t* fileName);
bool SDCARD_DeleteFile(uint8_t* fileName); 
bool SDCARD_NewFile(uint8_t* fileName);
bool SDCARD_CloseFile();
bool SDCARD_WriteFile(uint8_t* data, size_t size, size_t* sizeWritten, bool flush);
bool SDCARD_ReadFile(uint8_t* data, size_t size, size_t* sizeRead);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SDCARD_H */

/*******************************************************************************
 End of File
 */

