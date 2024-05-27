/* ************************************************************************** */
/** Descriptive File Name
 
  @Company
    Singular Software

  @File Name
    sdcard.c

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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "sdcard.h"
#include <string.h>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data                                         
// *****************************************************************************
// *****************************************************************************
#define SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0 			"/mnt/myDrive1"
#define SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0			"/dev/mmcblka1"

#define SDCARD_MOUNT_NAME   SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0
#define SDCARD_DEV_NAME     SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0
#define SDCARD_TEST_FILE_NAME    "horizon.txt"
#define SDCARD_TEST_FILE_PATH    APP_MOUNT_NAME"/"APP_FILE_NAME


SDCARD_DATA CACHE_ALIGN sdcardData;

// *****************************************************************************
// *****************************************************************************
// Section: Callback Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************

static bool WaitAndCheckRequestResult(SDCARD_STATES state);

static int SDCARD_CMDProcess(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv) 
{
    if ((argc >= 2) && (!strcmp(argv[1], "-o")))
    {
        if(argc > 2)//get file name
        {
            strcpy((char*)sdcardData.fileName, argv[2]);
        }
        else
        {
            strcpy((char*)sdcardData.fileName, SDCARD_TEST_FILE_NAME);
        }
        
        SYS_CONSOLE_PRINT("\r\nOpening %s\r\n", (char*)sdcardData.fileName);
        sdcardData.state = SDCARD_STATE_OPEN_FILE;
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-d")))
    {
        if(argc > 2)//get file name
        {
            strcpy((char*)sdcardData.fileName, argv[2]);
        }
        else
        {
            strcpy((char*)sdcardData.fileName, SDCARD_TEST_FILE_NAME);
        }
        
        SYS_CONSOLE_PRINT("\r\nDeleting %s\r\n", (char*)sdcardData.fileName);
        sdcardData.state = SDCARD_STATE_DELETE_FILE;
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-n")))
    {
        if(argc > 2)//get file name
        {
            strcpy((char*)sdcardData.fileName, argv[2]);
        }
        else
        {
            strcpy((char*)sdcardData.fileName, SDCARD_TEST_FILE_NAME);
        }
        
        SYS_CONSOLE_PRINT("\r\nCreating %s\r\n", (char*)sdcardData.fileName);
        sdcardData.newFile = true;
        sdcardData.state = SDCARD_STATE_OPEN_FILE;
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-c")))
    {
        SYS_CONSOLE_PRINT("\r\nClosing %s\r\n", (char*)sdcardData.fileName);
        sdcardData.state = SDCARD_STATE_CLOSE_FILE;
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-w")))
    {
        struct tm now_time;
        RTC_GetDateTime(&now_time);
        char text[75];
        sprintf(text, "Horizon test file - %02d:%02d:%02d %02d-%02d-%04d",now_time.tm_hour, now_time.tm_min, now_time.tm_sec, now_time.tm_mday, (now_time.tm_mon + 1), now_time.tm_year);       

        if(argc > 2)//get file name
        {
            strcpy((char*)sdcardData.fileData, argv[2]);
        }
        else
        {
            strcpy((char*)sdcardData.fileData, text);
        }

        SYS_CONSOLE_PRINT("\r\nWriting to %s\r\n", (char*)sdcardData.fileName);
        sdcardData.state = SDCARD_STATE_WRITE_FILE;
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-r")))
    {
        uint32_t readCount = 16;
        
        if(argc > 2)//get file name
        {
            readCount =  strtol(argv[2], NULL, 0);
            if(readCount > sizeof(sdcardData.fileData))
            {
                readCount = sizeof(sdcardData.fileData);
            }
        }
    
        SYS_CONSOLE_PRINT("\r\nReading from %s\r\n", (char*)sdcardData.fileName);

        sdcardData.fileDataReadSize1 = readCount;
        sdcardData.state = SDCARD_STATE_READ_FILE;

        if(WaitAndCheckRequestResult(SDCARD_STATE_READ_FILE))
        {
            for(uint32_t i = 0; i < sdcardData.fileDataReadSizeRead; i++)
            {
                SYS_CONSOLE_PRINT("%02X ",sdcardData.fileData[i]);

                if(i > 0)
                {
                    if((i+1) % 16 == 0)
                    {
                        SYS_CONSOLE_PRINT("\r\n");
                    }
                    if((i+1) % 256 == 0)
                    {
                        SYS_CONSOLE_PRINT("\r\n");
                        //UART need some time to process (another way is to increase the UART buffer size)
                        vTaskDelay(100 / portTICK_PERIOD_MS);
                    }
                }
            }
        }
    }
    else if((argc >= 2) && !strcmp(argv[1], "help"))
    {
        SYS_CONSOLE_PRINT("\r\n-o [file name]: Open file(default=horizon.info).\r\n-d [file name]: Delete file(default=horizon.info).\r\n-n [file name]: Create new file(default=horizon.info).\r\n-c: Close file.\r\n-w: Write to file.\r\n-r [count]: Read from file.\r\n");
    }
    return 0;
}
static const SYS_CMD_DESCRIPTOR SDCARDCmdTbl[] =
{
    {"sdcard", (SYS_CMD_FNC) SDCARD_CMDProcess, ": SDCARD commands processing"},    
};



static void CloseFile()
{
    if(sdcardData.fileHandle != SYS_FS_HANDLE_INVALID)
    {
        SYS_FS_FileClose(sdcardData.fileHandle);
        sdcardData.fileHandle = SYS_FS_HANDLE_INVALID;
        sdcardData.fileSize = 0;
    }
}
bool SDCardInserted()
{
    return SD_CARD_DETECT_Get() != 0;
}

static bool WaitAndCheckRequestResult(SDCARD_STATES state)
{
    for(uint32_t i = 0; i < 1000; i++)
    {
        if(sdcardData.state != state)
        {
            if(sdcardData.state == SDCARD_STATE_ERROR)
            {
                //wait for last error to be updated
                while(sdcardData.state == SDCARD_STATE_ERROR);
                return false;
            }
            return true;
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }    
    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void SDCARD_Initialize ( void )
{
    sdcardData.state = SDCARD_STATE_INIT;
    
    sdcardData.fileHandle = SYS_FS_HANDLE_INVALID;
    sdcardData.deviceAttached = false;
    
    sdcardData.fileDataWriteSize = 0;
    sdcardData.fileDataWrittenSize = 0;
    sdcardData.fileDataReadSize1 = 0;
    sdcardData.fileDataReadSizeRead = 0;
    
    sdcardData.newFile = false;
    sdcardData.fileSync = false;
    
    sdcardData.fileSize = 0;
    
    sdcardData.lastError = SYS_FS_ERROR_OK;

    strcpy((char*)sdcardData.fileName, SDCARD_TEST_FILE_NAME);
    
    SYS_CMD_ADDGRP(SDCARDCmdTbl, sizeof (SDCARDCmdTbl) / sizeof (*SDCARDCmdTbl), "sdcard", ": SD-Card commands"); 
}

void SDCARD_Tasks (void )
{
    if(sdcardData.state != SDCARD_STATE_INIT && sdcardData.state != SDCARD_STATE_CHECK_FS_DEVICE_ATTACH)
    {
        if (!SDCardInserted())//card was removed
        {
            USER_CONSOLE_PRINT("\r\nSD-Card removed.\r\n");

            sdcardData.state = SDCARD_STATE_FS_DEVICE_DETACHED;
        }
    }
    
    switch (sdcardData.state)
    {
        case SDCARD_STATE_INIT:
        {
            sdcardData.state = SDCARD_STATE_CHECK_FS_DEVICE_ATTACH;

            break;
        }

        case SDCARD_STATE_CHECK_FS_DEVICE_ATTACH:
        {
            if (SDCardInserted())
            {
               USER_CONSOLE_PRINT("\r\nSD-Card inserted.\r\n");

               sdcardData.state = SDCARD_STATE_FS_DEVICE_ATACHED;
            }
            break;
        }

        case SDCARD_STATE_FS_DEVICE_ATACHED:
        {
            sdcardData.state = SDCARD_STATE_MOUNT_DISK;

            break;
        }
        
        case SDCARD_STATE_MOUNT_DISK:
        {
            if(SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0)
            {
                sdcardData.state = SDCARD_STATE_MOUNT_DISK;
            }
            else
            {
                sdcardData.state = SDCARD_STATE_IDLE_TASK;
            }
            break;
        }
        
        case SDCARD_STATE_DELETE_FILE:
        {
            CloseFile();
            
            if(SYS_FS_FileDirectoryRemove((char *)sdcardData.fileName) == SYS_FS_RES_SUCCESS)
            {
                sdcardData.state = SDCARD_STATE_IDLE_TASK;
            }
            else
            {
                sdcardData.state = SDCARD_STATE_ERROR;
            }
            break;
        }
        
        case SDCARD_STATE_OPEN_FILE:
        {            
            CloseFile();

            sdcardData.fileHandle = SYS_FS_FileOpen((char *)sdcardData.fileName, (sdcardData.newFile ? SYS_FS_FILE_OPEN_WRITE_PLUS : SYS_FS_FILE_OPEN_READ_PLUS));

            sdcardData.newFile = false;
            if(sdcardData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                sdcardData.state = SDCARD_STATE_ERROR;
            }
            else
            {
                sdcardData.fileSize = SYS_FS_FileSize(sdcardData.fileHandle);
                if(sdcardData.fileSize == -1)
                {
                    sdcardData.state = SDCARD_STATE_ERROR;
                }
                else
                {
                    sdcardData.state = SDCARD_STATE_IDLE_TASK;
                }
            }
            
            break;
        }
        
        case SDCARD_STATE_WRITE_FILE:
        {
            if(sdcardData.fileHandle != SYS_FS_HANDLE_INVALID)
            {
                sdcardData.fileDataWrittenSize = SYS_FS_FileWrite(sdcardData.fileHandle, (void *)sdcardData.fileData, sdcardData.fileDataWriteSize);
                if (sdcardData.fileDataWrittenSize == -1)
                {
                    /* There was an error while writing to the file.*/
                    SYS_FS_FileClose(sdcardData.fileHandle);
                    sdcardData.fileHandle = SYS_FS_HANDLE_INVALID;
                    sdcardData.state = SDCARD_STATE_ERROR;
                }
                else
                {
                    if(sdcardData.fileSync)
                    {
                        sdcardData.fileSync = false;
                        if(SYS_FS_FileSync(sdcardData.fileHandle) != SYS_FS_RES_SUCCESS)
                        {
                            sdcardData.state = SDCARD_STATE_ERROR;
                        }
                    }
                    sdcardData.state = SDCARD_STATE_IDLE_TASK;
                }
            }
            else
            {
                sdcardData.fileDataWrittenSize = 0;
                sdcardData.state = SDCARD_STATE_IDLE_TASK;
            }
            break;
        }
        
        case SDCARD_STATE_READ_FILE:
        {
            if(sdcardData.fileHandle != SYS_FS_HANDLE_INVALID)
            {
                memset(sdcardData.fileData, 0, sizeof(sdcardData.fileData));
                sdcardData.fileDataReadSizeRead = SYS_FS_FileRead(sdcardData.fileHandle, (void *)sdcardData.fileData, sdcardData.fileDataReadSize1);
                if (sdcardData.fileDataReadSizeRead == -1)
                {
                    /* There was an error while reading from the file.*/
                    SYS_FS_FileClose(sdcardData.fileHandle);
                    sdcardData.fileHandle = SYS_FS_HANDLE_INVALID;
                    sdcardData.state = SDCARD_STATE_ERROR;
                }
                else
                {
                    sdcardData.state = SDCARD_STATE_IDLE_TASK;
                }
            }
            else
            {
                sdcardData.fileDataReadSizeRead = 0;
                sdcardData.state = SDCARD_STATE_IDLE_TASK;
            }
            break;
        }
            
        case SDCARD_STATE_CLOSE_FILE:
        {
            CloseFile();
            sdcardData.state = SDCARD_STATE_IDLE_TASK;
            break;
        }
        
        case SDCARD_STATE_FS_DEVICE_DETACHED:
        {
            CloseFile();
            
            SYS_FS_Unmount(SDCARD_MOUNT_NAME);
            
            sdcardData.state = SDCARD_STATE_CHECK_FS_DEVICE_ATTACH;
            break;
        }

        case SDCARD_STATE_IDLE_TASK:
        {
            if (!SDCardInserted())//card was removed
            {
                USER_CONSOLE_PRINT("\r\nSD-Card removed.\r\n");

                sdcardData.state = SDCARD_STATE_FS_DEVICE_DETACHED;
            }
            break;
        }

        /* Application is in an error state */
        case SDCARD_STATE_ERROR:
        {
            USER_CONSOLE_PRINT("\r\n * SDCARD_STATE_ERROR! *");
            sdcardData.lastError = SYS_FS_Error();
            switch(sdcardData.lastError)
            {
                /* Success */
                case SYS_FS_ERROR_OK:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_OK !\r\n");
                    break;
                /* (1) A hard error occurred in the low level disk I/O layer */
                case SYS_FS_ERROR_DISK_ERR:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_DISK_ERR !\r\n");
                    break;
                /* (2) Assertion failed */
                case SYS_FS_ERROR_INT_ERR:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_INT_ERR !\r\n");
                    break;
                /* (3) The physical drive cannot work */
                case SYS_FS_ERROR_NOT_READY:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NOT_READY !\r\n");
                    break;
                /* (4) Could not find the file */
                case SYS_FS_ERROR_NO_FILE:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NO_FILE !\r\n");
                    break;
                /* (5) Could not find the path */
                case SYS_FS_ERROR_NO_PATH:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NO_PATH !\r\n");
                    break;
                /* (6) The path name format is invalid */
                case SYS_FS_ERROR_INVALID_NAME:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_INVALID_NAME !\r\n");
                    break;
                /* (7) Access denied due to prohibited access or directory full */
                case SYS_FS_ERROR_DENIED:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_DENIED !\r\n");
                    break;
                /* (8) Access denied due to prohibited access */
                case SYS_FS_ERROR_EXIST:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_EXIST !\r\n");
                    break;
                /* (9) The file/directory object is invalid */
                case SYS_FS_ERROR_INVALID_OBJECT:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_INVALID_OBJECT !\r\n");
                    break;
                /* (10) The physical drive is write protected */
                case SYS_FS_ERROR_WRITE_PROTECTED:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_WRITE_PROTECTED !\r\n");
                    break;
                /* (11) The logical drive number is invalid */
                case SYS_FS_ERROR_INVALID_DRIVE:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_INVALID_DRIVE !\r\n");
                    break;
                /* (12) The volume has no work area */
                case SYS_FS_ERROR_NOT_ENABLED:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NOT_ENABLED !\r\n");
                    break;
                /* (13) There is no valid volume */
                case SYS_FS_ERROR_NO_FILESYSTEM:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NO_FILESYSTEM !\r\n");
                    break;
                /* (14) The Format() aborted due to any parameter error */
                case SYS_FS_ERROR_FORMAT_ABORTED:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_FORMAT_ABORTED !\r\n");
                    break;
                /* (15) Could not get a grant to access the volume within defined period */
                case SYS_FS_ERROR_TIMEOUT:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_TIMEOUT !\r\n");
                    break;
                /* (16) The operation is rejected according to the file sharing policy */
                case SYS_FS_ERROR_LOCKED:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_LOCKED !\r\n");
                    break;
                /* (17) LFN working buffer could not be allocated */
                case SYS_FS_ERROR_NOT_ENOUGH_CORE:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NOT_ENOUGH_CORE !\r\n");
                    break;
                /* (18) Number of open files */
                case SYS_FS_ERROR_TOO_MANY_OPEN_FILES:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_TOO_MANY_OPEN_FILES !\r\n");
                    break;
                /* (19) Given parameter is invalid */
                case SYS_FS_ERROR_INVALID_PARAMETER:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_INVALID_PARAMETER !\r\n");
                    break;
                /* (20) Too many mounts requested. Not enough free volume available */
                case SYS_FS_ERROR_NOT_ENOUGH_FREE_VOLUME:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NOT_ENOUGH_FREE_VOLUME !\r\n");
                    break;
                /* (21) Requested native file system is not supported */
                case SYS_FS_ERROR_FS_NOT_SUPPORTED:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_FS_NOT_SUPPORTED !\r\n");
                    break;
                /* (22) Requested native file system does not match the format of volume */
                case SYS_FS_ERROR_FS_NOT_MATCH_WITH_VOLUME:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_FS_NOT_MATCH_WITH_VOLUME !\r\n");
                    break;
                /* (23) Function not supported in native file system layer */
                case SYS_FS_ERROR_NOT_SUPPORTED_IN_NATIVE_FS:
                    USER_CONSOLE_PRINT("\r\n! SYS_FS_ERROR_NOT_SUPPORTED_IN_NATIVE_FS !\r\n");
                    break;
            }            
            //restart
            USER_CONSOLE_PRINT("\r\n! RESTARTING SD_CARD !\r\n");
            sdcardData.state = SDCARD_STATE_FS_DEVICE_DETACHED;
        }
        default:
            break;
    }
}
 
// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions                                               
// *****************************************************************************
// *****************************************************************************

uint8_t SDCARD_GetLastError()
{
    return sdcardData.lastError;
}

int32_t SDCARD_FileSize()
{
     return sdcardData.fileSize;
}

bool SDCARD_OpenFile(uint8_t* fileName) 
{
    if(fileName != NULL)
    {
        //max file name = sizeof(sdcardData.fileName)-1
        if(strlen((char*)fileName) > sizeof(sdcardData.fileName)-1)
        {
            strncpy((char*)sdcardData.fileName, (char*)fileName, sizeof(sdcardData.fileName)-1);
        }
        else
        {
            strcpy((char*)sdcardData.fileName, (char*)fileName);
        }

        sdcardData.state = SDCARD_STATE_OPEN_FILE;

        return WaitAndCheckRequestResult(SDCARD_STATE_OPEN_FILE);
    }
    else
    {
        sdcardData.lastError = SYS_FS_ERROR_INVALID_NAME;
        return false;
    }
}

bool SDCARD_DeleteFile(uint8_t* fileName) 
{
    if(fileName != NULL)
    {
        //max file name = sizeof(sdcardData.fileName)-1
        if(strlen((char*)fileName) > sizeof(sdcardData.fileName)-1)
        {
            strncpy((char*)sdcardData.fileName, (char*)fileName, sizeof(sdcardData.fileName)-1);
        }
        else
        {
            strcpy((char*)sdcardData.fileName, (char*)fileName);
        }

        sdcardData.state = SDCARD_STATE_DELETE_FILE;
    
        return WaitAndCheckRequestResult(SDCARD_STATE_DELETE_FILE);
    }
    else
    {
        sdcardData.lastError = SYS_FS_ERROR_INVALID_NAME;
        return false;
    }
}

bool SDCARD_NewFile(uint8_t* fileName) 
{
    if(fileName != NULL)
    {    
        if(strlen((char*)fileName) > sizeof(sdcardData.fileName)-1)
        {
            strncpy((char*)sdcardData.fileName, (char*)fileName, sizeof(sdcardData.fileName)-1);
        }
        else
        {
            strcpy((char*)sdcardData.fileName, (char*)fileName);
        }
        
        sdcardData.newFile = true;
        sdcardData.state = SDCARD_STATE_OPEN_FILE;
    
        return WaitAndCheckRequestResult(SDCARD_STATE_OPEN_FILE);
    }
    else
    {
        strcpy((char*)sdcardData.fileName, SDCARD_TEST_FILE_NAME);
        return false;
    }
}

bool SDCARD_CloseFile() 
{
    if(sdcardData.fileHandle == SYS_FS_HANDLE_INVALID)
    {
        sdcardData.lastError = SYS_FS_ERROR_INVALID_OBJECT;
        return false;
    }
    
    sdcardData.state = SDCARD_STATE_CLOSE_FILE;

    return WaitAndCheckRequestResult(SDCARD_STATE_CLOSE_FILE);
}

bool SDCARD_WriteFile(uint8_t* data, size_t size, size_t* sizeWritten, bool flush) 
{
    if(sdcardData.fileHandle == SYS_FS_HANDLE_INVALID)
    {
        sizeWritten = 0;
        sdcardData.lastError = SYS_FS_ERROR_INVALID_OBJECT;
        return false;
    }
    
    if(data != NULL)
    {
        *sizeWritten = 0;
        
        while(size > 0)
        {
            size_t writeSize = size;
            if (writeSize > sizeof(sdcardData.fileData))
            {
                writeSize = sizeof(sdcardData.fileData);
            }
        
            memcpy(sdcardData.fileData, data+(*sizeWritten), writeSize);
    
            sdcardData.fileSync = flush;
            sdcardData.fileDataWriteSize = writeSize;
            sdcardData.state = SDCARD_STATE_WRITE_FILE;
    
            if(WaitAndCheckRequestResult(SDCARD_STATE_WRITE_FILE))
            {
                if(sdcardData.fileDataWrittenSize > 0)
                {
                    *sizeWritten += sdcardData.fileDataWrittenSize;
                    size -= sdcardData.fileDataWrittenSize;
                }
                else
                {
                    //if sdcardData.fileDataWrittenSize == 0 no more data was written -> end here
                    size = 0;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;    
}

bool SDCARD_ReadFile(uint8_t* data, size_t size, size_t* sizeRead) 
{
    if(sdcardData.fileHandle == SYS_FS_HANDLE_INVALID)
    {
        sizeRead = 0;
        sdcardData.lastError = SYS_FS_ERROR_INVALID_OBJECT;
        return false;
    }
        
    if(data != NULL)
    {
        *sizeRead = 0;
        
        while(size > 0)
        {
            size_t readSize = size;
            if (readSize > sizeof(sdcardData.fileData))
            {
                readSize = sizeof(sdcardData.fileData);
            }

            sdcardData.fileDataReadSize1 = readSize;
            sdcardData.state = SDCARD_STATE_READ_FILE;

            if(WaitAndCheckRequestResult(SDCARD_STATE_READ_FILE))
            {
                if(sdcardData.fileDataReadSizeRead > 0)
                {
                    memcpy(data+(*sizeRead), sdcardData.fileData, sdcardData.fileDataReadSizeRead);

                    *sizeRead += sdcardData.fileDataReadSizeRead;
                    size -= sdcardData.fileDataReadSizeRead;
                }
                else
                {
                    //if sdcardData.fileDataReadSizeRead == 0 no more data can be read from the file -> end here
                    size = 0;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
/*******************************************************************************
 End of File
 */

