/* ************************************************************************** */
/** Descriptive File Name
 
  @Company
    Singular Software

  @File Name
    sst26.c

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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files                                                    
// *****************************************************************************
// *****************************************************************************

#include "sst26.h"
#include <string.h>
#include "configuration.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data                                         
// *****************************************************************************
// *****************************************************************************

SST26_DATA CACHE_ALIGN sst26Data;
char horizontestString[] = "Horizon write test.";

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

// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************

static bool WaitForReadyState(uint32_t timeout);

static bool EraseTest(uint32_t address, uint32_t blockCount)
{
    SST26_Erase(address, blockCount);
    
    return WaitForReadyState(blockCount * 100);
}

static bool EraseAllTest()
{
    SST26_Erase(0, sst26Data.geometry.erase_numBlocks);
    
    return WaitForReadyState(sst26Data.geometry.erase_numBlocks * 100);
}

static bool WriteTest(uint32_t address, uint8_t* data, uint32_t size)
{
    if(size > sizeof(sst26Data.testBuffer)-sizeof(uint32_t)-1)
    {
        SYS_CONSOLE_PRINT("\r\nData truncated to testBuffer size.");
        size = sizeof(sst26Data.testBuffer)-sizeof(uint32_t)-1;
    }
    
    memcpy(&sst26Data.testBuffer, data, size);
    
    return SST26_Write(sst26Data.testBuffer, size, address);
}                

static bool WriteAllTest()
{
    for(uint32_t i = 0; i < sizeof(sst26Data.testBuffer)/2; i++)
    {
        sst26Data.testBuffer[i] = (uint8_t)i;
    }
   
    for(uint32_t i = 0; i < sst26Data.geometry.write_numBlocks; i++)
    {
        uint32_t address = i * sst26Data.geometry.write_blockSize;
        
        if(SST26_Write(sst26Data.testBuffer, sizeof(sst26Data.testBuffer)/2, address))
        {
            SYS_CONSOLE_PRINT("\rWrite successful. 0X%08X", address);
        }
        else
        {
            SYS_CONSOLE_PRINT("\r\n! Write failed. !");
            return false;;
        }
    }
    return true;
}                

static bool ReadTest(uint32_t address, uint32_t size, bool writeToFile)
{
    bool asString = false;
    uint32_t readSize = size;
    uint32_t endAddress = address + size;
    
    if(writeToFile)
    {
        if(!SDCARD_NewFile((uint8_t*)"flash_content.bin"))
        {
            SYS_CONSOLE_PRINT("\r\n! Error opening new file on the SD-Card. !");
            return false;
        }
    }
    
    for(uint32_t readAddress = address ; readAddress < endAddress && size > 0; readAddress += sizeof(sst26Data.testBuffer))
    {
        readSize = size;
        if(readSize > sizeof(sst26Data.testBuffer))
        {
            readSize = sizeof(sst26Data.testBuffer);        
        }
        size-=readSize;
        
        memset(sst26Data.testBuffer, 0, sizeof(sst26Data.testBuffer));
        
        if(SST26_Read(sst26Data.testBuffer, readSize, readAddress))
        {
            if(writeToFile)
            {
                SYS_CONSOLE_PRINT("\r%08X", readAddress);
                size_t sizeWritten;
                if(!SDCARD_WriteFile(sst26Data.testBuffer, sizeof(sst26Data.testBuffer), &sizeWritten, true))
                {
                    SYS_CONSOLE_PRINT("\r\n! Error writing to file on the SD-Card. !");
                    return false;
                }
            }
            else
            {
                if(!asString)
                {
                    SYS_CONSOLE_PRINT("\r\n");
                    SYS_CONSOLE_PRINT("\r\nAddress: %08X\r\n", readAddress);
                }
                for(uint32_t i = 0; i < readSize; i++)
                {
                    if(asString)
                    {
                        SYS_CONSOLE_PRINT("%c",sst26Data.testBuffer[i]);
                    }
                    else
                    {
                        SYS_CONSOLE_PRINT("%02X ",sst26Data.testBuffer[i]);
                    }
                    
                    if(i > 0 && !asString)
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
                
                if(!asString)
                {
                    SYS_CONSOLE_PRINT("\r\n");
                }
            }
        }
        else
        {
            SYS_CONSOLE_PRINT("\r\n! Flash read failed. !");
            return false;
        }
    }
    
    if(writeToFile)
    {    
        if(!SDCARD_CloseFile())
        {
            SYS_CONSOLE_PRINT("\r\n! Error closing to file on the SD-Card. !");
            return false;
        }
    }
    
    return true;
}

static int SST26_CMDProcess(SYS_CMD_DEVICE_NODE* pCmdNode, int argc, char** argv) 
{
    if ((argc >= 2) && (!strcmp(argv[1], "-e")))
    {
        uint32_t address = 0x0000;
        uint32_t blockCount = 1;
        
        if(argc >= 3)//get address
        {
            address =  strtol(argv[2], NULL, 0);

            if(argc >= 4)//get count
            {
                blockCount = strtol(argv[3], NULL, 0);
            }
        }
        
        SYS_CONSOLE_PRINT("\r\nErasing %d blocks from %08X", blockCount, address);

        sst26Data.showProgress = true;
        if(EraseTest(address, blockCount))
        {
            SYS_CONSOLE_PRINT("\r\nErase successful.\r\n");
        }
        else
        {
            SYS_CONSOLE_PRINT("\r\nErase failed.\r\n");
        }
        sst26Data.showProgress = false;
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-ea")))
    {
        sst26Data.showProgress = true;
        if(EraseAllTest())
        {
            SYS_CONSOLE_PRINT("\r\nErase successful.\r\n");
        }
        else
        {
            SYS_CONSOLE_PRINT("\r\nErase failed.\r\n");
        }        
        sst26Data.showProgress = false;
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-w")))
    {
        uint32_t address = 0x0000;
        char* s = NULL;
        if(argc >= 3)//get address
        {
            address =  strtol(argv[2], NULL, 0);

            if(argc >= 4)//get string
            {
                s = argv[3];
            }
        }

        if(s == NULL)
        {
            s = horizontestString;
        }
        uint32_t size = strlen(s);
        
        SYS_CONSOLE_PRINT("\r\nWriting to address: %08X", address);
        
        if(WriteTest(address, (uint8_t*)s, size))
        {
            SYS_CONSOLE_PRINT("\r\nWrite successful. %d\r\n",size);
        }
        else
        {
            SYS_CONSOLE_PRINT("\r\nWrite Failed.\r\n");
        }
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-wa")))
    {
        SYS_CONSOLE_PRINT("\r\nFilling flash with test pattern\r\n");
        WriteAllTest();
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-r")))
    {
        bool writeToFile = false;
        uint32_t address = 0x0000;
        uint32_t size = strlen(horizontestString);
        if(argc >= 3)//get address
        {
            address =  strtol(argv[2], NULL, 0);
        }
        if(argc >= 4)//get address
        {
            size =  strtol(argv[3], NULL, 0);
        }
        if(argc >= 5)//write to file
        {
            writeToFile =  strtol(argv[2], NULL, 0) != 0;
        }
        
        if(ReadTest(address, size, writeToFile))
        {
        }
        else
        {
        }
    }
    else if ((argc >= 2) && (!strcmp(argv[1], "-ra")))
    {
        bool writeToFile = false;
        uint32_t address = 0x0000;
        uint32_t size = 0x400000;

        if(argc >= 3)//write to file
        {
            writeToFile =  strtol(argv[2], NULL, 0) != 0;
        }

        if(writeToFile)
        {
            SYS_CONSOLE_PRINT("\r\nReading flash into flash_content.bin on the SD-Card(address:0X%08X %d bytes", address, size);
        }
        else
        {
            SYS_CONSOLE_PRINT("\r\nReading flash (address:0X%08X %d bytes", address, size);
        }
        
        if(ReadTest(address, size, writeToFile))
        {
        }
        else
        {
        }
    }
    else if((argc >= 2) && !strcmp(argv[1], "help"))
    {
        SYS_CONSOLE_PRINT("\r\n-e [address] [count]: Erase flash blocks.\r\n-ea: Erase all blocks.\r\n-w [address] [data]: Write test string to flash.\r\n-wa: Fill flash with test data.\r\n-r [address] [size] [toFile?]: Read  from flash.\r\n-ra [toFile?]: Read all flash.\r\n");
    }
    return 0;
}
static const SYS_CMD_DESCRIPTOR SST26CmdTbl[] =
{
    {"flash", (SYS_CMD_FNC) SST26_CMDProcess, ": flash-SST26 commands processing"},    
};

static bool WaitForReadyState(uint32_t timeout)
{
    if(sst26Data.showProgress)
    {
        USER_CONSOLE_PRINT("\r\n");
    }

    for(uint32_t i = 0; i < timeout; i++)
    {
        if(sst26Data.showProgress)
        {
            USER_CONSOLE_PRINT("\r%d", i);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
        if(SST26_Ready())
        {
            if(sst26Data.showProgress)
            {
                USER_CONSOLE_PRINT("\r\n");
            }
            return true;
        }
    }    

    if(sst26Data.showProgress)
    {
        USER_CONSOLE_PRINT("\r\n");
    }
    return false;
}
// *****************************************************************************
// *****************************************************************************
// Section: Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void SST26_Initialize ( void )
{
    //SST26VF032B 32-Mbit=4M-byte  15827 pages of 256 bytes
    sst26Data.state = SST26_STATE_INIT;
    sst26Data.address = 0;
    sst26Data.eraseIndex = 0;
    sst26Data.writeIndex = 0;
    sst26Data.dataSize = PAGE_SIZE;
    
    sst26Data.showProgress = false;
    
    SYS_CMD_ADDGRP(SST26CmdTbl, sizeof (SST26CmdTbl) / sizeof (*SST26CmdTbl), "flash", ": flash-SST26 commands"); 
}

void SST26_Tasks ( void )
{
    DRV_SST26_TRANSFER_STATUS transferStatus = DRV_SST26_TRANSFER_ERROR_UNKNOWN;

    switch ( sst26Data.state )
    {
        case SST26_STATE_INIT:
        {
            if (DRV_SST26_Status(DRV_SST26_INDEX) == SYS_STATUS_READY)
            {
                sst26Data.state = SST26_STATE_OPEN_DRIVER;
            }
            break;
        }

        case SST26_STATE_OPEN_DRIVER:
        {
            sst26Data.handle = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);

            if (sst26Data.handle != DRV_HANDLE_INVALID)
            {
                sst26Data.state = SST26_STATE_GEOMETRY_GET;
            }

            break;
        }
        case SST26_STATE_GEOMETRY_GET:
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            if (DRV_SST26_GeometryGet(sst26Data.handle, &sst26Data.geometry) != true)
            {
                sst26Data.state = SST26_STATE_ERROR;
                break;
            }

            sst26Data.address = 0;
            sst26Data.eraseIndex = 0;
            sst26Data.writeIndex = 0;
            sst26Data.dataSize = PAGE_SIZE;
            /*
            USER_CONSOLE_PRINT("\r\nFlash read_blockSize %d   \r\n", sst26Data.geometry.read_blockSize);       //Flash read_blockSize 1
            USER_CONSOLE_PRINT("\r\nFlash read_numBlocks %d   \r\n", sst26Data.geometry.read_numBlocks);       //Flash read_numBlocks 4194304
            USER_CONSOLE_PRINT("\r\nFlash numReadRegions %d   \r\n", sst26Data.geometry.numReadRegions);       //Flash numReadRegions 1
            USER_CONSOLE_PRINT("\r\nFlash write_blockSize %d  \r\n", sst26Data.geometry.write_blockSize);     //Flash write_blockSize 256
            USER_CONSOLE_PRINT("\r\nFlash write_numBlocks %d  \r\n", sst26Data.geometry.write_numBlocks);     //Flash write_numBlocks 16384
            USER_CONSOLE_PRINT("\r\nFlash numWriteRegions %d  \r\n", sst26Data.geometry.numWriteRegions);     //Flash numWriteRegions 1
            USER_CONSOLE_PRINT("\r\nFlash erase_blockSize %d  \r\n", sst26Data.geometry.erase_blockSize);     //Flash erase_blockSize 4096
            USER_CONSOLE_PRINT("\r\nFlash erase_numBlocks %d  \r\n", sst26Data.geometry.erase_numBlocks);     //Flash erase_numBlocks 1024
            USER_CONSOLE_PRINT("\r\nFlash numEraseRegions %d  \r\n", sst26Data.geometry.numEraseRegions);     //Flash numEraseRegions 1
            USER_CONSOLE_PRINT("\r\nFlash blockStartAddress %d\r\n", sst26Data.geometry.blockStartAddress); //Flash blockStartAddress 0
            */
            sst26Data.state = SST26_STATE_READY;
            USER_CONSOLE_PRINT("\r\nFlash is ready! \r\n");
            
            break;
        }
            
        case SST26_STATE_READY://wait for command
            break;
            
            
        case SST26_STATE_ERASE_FLASH:
        {
            if (DRV_SST26_SectorErase(sst26Data.handle, (sst26Data.address + sst26Data.eraseIndex)) != true)
            {
                sst26Data.state = SST26_STATE_ERROR;
            }

            sst26Data.state = SST26_STATE_ERASE_WAIT;

            break;
        }

        case SST26_STATE_ERASE_WAIT:
        {
            transferStatus = DRV_SST26_TransferStatusGet(sst26Data.handle);

            if(transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
                sst26Data.eraseIndex += sst26Data.geometry.erase_blockSize;

                if (sst26Data.eraseIndex < sst26Data.geometry.erase_blockSize * sst26Data.eraseBlockCount)
                {
                    sst26Data.state = SST26_STATE_ERASE_FLASH;
                }
                else
                {
                    sst26Data.state = SST26_STATE_READY;
                }
            }
            else if (transferStatus == DRV_SST26_TRANSFER_ERROR_UNKNOWN)
            {
                sst26Data.state = SST26_STATE_ERROR;
            }
            break;
        }

        case SST26_STATE_WRITE_MEMORY:
        {
            if (DRV_SST26_PageWrite(sst26Data.handle, (uint32_t *)&sst26Data.writeBuffer[sst26Data.writeIndex], (sst26Data.address + sst26Data.writeIndex)) != true)
            {
                sst26Data.state = SST26_STATE_ERROR;
                break;
            }

            sst26Data.state = SST26_STATE_WRITE_WAIT;

            break;
        }

        case SST26_STATE_WRITE_WAIT:
        {
            transferStatus = DRV_SST26_TransferStatusGet(sst26Data.handle);

            if(transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
                sst26Data.writeIndex += sst26Data.geometry.write_blockSize;

                if (sst26Data.writeIndex < sst26Data.dataSize)
                {
                    sst26Data.state = SST26_STATE_WRITE_MEMORY;
                }
                else
                {
                    sst26Data.state = SST26_STATE_READY;
                }
            }
            else if (transferStatus == DRV_SST26_TRANSFER_ERROR_UNKNOWN)
            {
                sst26Data.state = SST26_STATE_ERROR;
            }

            break;
        }

        case SST26_STATE_READ_MEMORY:
        {
            if (DRV_SST26_Read(sst26Data.handle, (uint32_t *)&sst26Data.readBuffer, sst26Data.dataSize, sst26Data.address) != true)
            {
                sst26Data.state = SST26_STATE_ERROR;
            }
            else
            {
                sst26Data.state = SST26_STATE_READ_WAIT;
            }

            break;
        }

        case SST26_STATE_READ_WAIT:
        {
            transferStatus = DRV_SST26_TransferStatusGet(sst26Data.handle);

            if(transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
                sst26Data.state = SST26_STATE_READY;
            }
            else if (transferStatus == DRV_SST26_TRANSFER_ERROR_UNKNOWN)
            {
                sst26Data.state = SST26_STATE_ERROR;
            }

            break;
        }

        case SST26_STATE_CLOSE:
        {
            DRV_SST26_Close(sst26Data.handle);
            sst26Data.state = SST26_STATE_IDLE;
            break;
        }
        
        case SST26_STATE_IDLE:
        {
            break;
        }
        
        case SST26_STATE_ERROR:
        default:
        {
            sst26Data.lastStatus =  DRV_SST26_TransferStatusGet(sst26Data.handle);
            DRV_SST26_Close(sst26Data.handle);
            sst26Data.state = SST26_STATE_INIT;//retry
            break;
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions                                               
// *****************************************************************************
// *****************************************************************************

bool SST26_Ready()
{
    return sst26Data.state == SST26_STATE_READY;
}

bool SST26_Error()
{
    return sst26Data.state == SST26_STATE_ERROR;
}

uint32_t SST26_BlockSize()
{
    return sst26Data.geometry.erase_blockSize;
}

uint32_t SST26_GetLastStatus()
{
    return sst26Data.lastStatus;
}

bool SST26_Erase(uint32_t address, uint32_t blockCount)
{
    //erase address must be the start of a sector
    if(SST26_Ready() && address % sst26Data.geometry.erase_blockSize == 0)
    {
        sst26Data.eraseIndex = 0;
        sst26Data.address = address;
        sst26Data.eraseBlockCount = blockCount;
        sst26Data.state = SST26_STATE_ERASE_FLASH;
        
        return WaitForReadyState(blockCount * 100);
    }
    return false;
}

bool SST26_Write(uint8_t* data, uint32_t size, uint32_t address)
{
    if(SST26_Ready())
    {
        //write address must be the start of a page
        uint32_t writeAddress = (address / sst26Data.geometry.write_blockSize) * sst26Data.geometry.write_blockSize;
        uint32_t addressOffset = address - writeAddress;
        uint32_t dataIndex = 0;
        
        while(size > 0)
        {
            bool fullPage = false;
            uint32_t writeSize = size;
            
            if(addressOffset + writeSize > sizeof(sst26Data.writeBuffer))
            {
                //the amount of free space at the end of the block
                writeSize = sizeof(sst26Data.writeBuffer) - addressOffset;
            }
            
            //if this write is of a full page no need to read first
            fullPage = writeSize == sst26Data.geometry.write_blockSize && writeAddress % sst26Data.geometry.write_blockSize == 0 && addressOffset == 0;
                    
            if(!fullPage)
            {
                //read page
                sst26Data.dataSize = sizeof(sst26Data.readBuffer);
                sst26Data.address = writeAddress;
                sst26Data.state = SST26_STATE_READ_MEMORY;
            }
            
            if(fullPage || WaitForReadyState(writeSize * 100))
            {
                if(!fullPage)
                {
                    //copy the current content of the memory
                    for(uint32_t i = 0; i< sizeof(sst26Data.writeBuffer); i ++)
                    {
                        sst26Data.writeBuffer[i] = sst26Data.readBuffer[i];
                    }
                }
                
                //copy new data to write buffer
                for(uint32_t i = addressOffset; i < addressOffset + writeSize; i++)
                {
                    sst26Data.writeBuffer[i] = data[dataIndex++];
                }

                sst26Data.writeIndex = 0;
                sst26Data.dataSize = writeSize;
                sst26Data.address = writeAddress;
                sst26Data.state = SST26_STATE_WRITE_MEMORY;
                if(WaitForReadyState(writeSize * 100))
                {
                    size -= writeSize;
                    if(size > 0)//if more data move to next page
                    {
                        writeAddress += sst26Data.geometry.write_blockSize;
                        addressOffset = 0;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }        
    }
    
    USER_CONSOLE_PRINT("\r\n! Error SST26_Write. !\r\n");
    return false;
}

bool SST26_Read(uint8_t* outData, uint32_t size, uint32_t address)
{
    if(SST26_Ready())
    {
        uint32_t index = 0;
        uint32_t readSize = size;
        uint32_t outputPageIndex = index * sst26Data.geometry.write_blockSize;
         
        while(size > 0)
        {
            readSize = size;
            outputPageIndex = index * sst26Data.geometry.write_blockSize;
            
            if(readSize > sizeof(sst26Data.readBuffer))
            {
                readSize = sizeof(sst26Data.readBuffer);
            }
            
            sst26Data.dataSize = readSize;
            sst26Data.address = address;
            sst26Data.state = SST26_STATE_READ_MEMORY;

            if(WaitForReadyState(readSize * 100))
            {
                //copy output buffer
                for(uint32_t i = 0; i< readSize; i++)
                {
                    *(outData + outputPageIndex + i) = sst26Data.readBuffer[i];
                }
                
                size -= readSize;
                if(size > 0)//if need to read more data move to next page
                {
                    address += sst26Data.geometry.write_blockSize;
                    index++;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                break;
            }
        }
    }
    
    USER_CONSOLE_PRINT("\r\n! Error SST26_Read. !\r\n");
    return false;
}

/*******************************************************************************
 End of File
 */
