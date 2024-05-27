/* ************************************************************************** */
/** USB HOST/DEVICE

  @Company
    Singular Software

  @File Name
    usb.h

  @Summary
    USB Host/Device access.

  @Description
    Demonstrate the use of the USB as Host and Device.
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


#ifndef _USB_H /* Guard against multiple inclusion */
#define _USB_H

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
#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"

// Provide C++ Compatibility
#ifdef __cplusplus  
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Constants                                                         
// *****************************************************************************
// *****************************************************************************
#define USB_KEYBOARD_REPORTS_COUNT 6
// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
typedef void (*ControlKeyboardKeyPressEventHandler)(char*);
typedef void (*PrintableKeyboardKeyPressEventHandler)(char);

typedef enum
{
    /* USB initializing */
    USB_STATE_INIT,
    /* USB idle */
    USB_STATE_IDLE,
    /* TUSB320 comms */
    USB_STATE_TUSB320_READ_DEVICE_ID,            
    USB_STATE_TUSB320_GET_STATUS,
    USB_STATE_TUSB320_CHECK_STATUS,
    /*AS host enable bus power complete */
    USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE,
    /*AS device disable bus power complete */
    USB_STATE_WAIT_FOR_BUS_DISABLE_COMPLETE,
    /*Wait for USB power from host*/
    USB_STATE_WAIT_FOR_POWER_DETECT,
    /*Wait for USB attach*/
    USB_STATE_WAIT_FOR_DEVICE_ATTACH,
    /* Clear TUSB320 interrupt */
    USB_STATE_TUSB320_RESET_INT_STATUS,
    /* Set USB as Host or Device */
    USB_STATE_SELECT_USB_ROLE,
     /* USB attached */
    USB_STATE_DEVICE_ATTACHED,
     /* USB detached */
    USB_STATE_DEVICE_DETACHED,
    /* USB device initializing */
    USB_DEVICE_STATE_INIT,
    /*  waiting for configuration */
    USB_STATE_WAIT_FOR_CONFIGURATION,
    /* running as USB device mode*/
    USB_STATE_DEVICE_MAIN_TASK,
    /* running as USB HOST mode (for keyboard)*/
    USB_STATE_HOST_MAIN_TASK,
    /* USB error state */
    USB_STATE_ERROR
} USB_STATES;

/* TUSB320 I2C states */
typedef enum
{
    USB_STATE_TUSB320_SET_CSR_ADDRESS,
    USB_STATE_TUSB320_CSR_WRITE,
    USB_STATE_TUSB320_VERIFY_CSR_WRITE_1,
    USB_STATE_TUSB320_VERIFY_CSR_WRITE_2,
    USB_STATE_TUSB320_VERIFY_CSR_WRITE_3,
    USB_STATE_TUSB320_CSR_READ,
    USB_STATE_TUSB320_VERIFY_CSR_READ,
} USB_TUSB320_I2C_STATES;
typedef enum
{
    TUSB320_TRANSFER_STATUS_IN_PROGRESS,
    TUSB320_TRANSFER_STATUS_SUCCESS,
    TUSB320_TRANSFER_STATUS_ERROR,
    TUSB320_TRANSFER_STATUS_IDLE,

} TUSB320_TRANSFER_STATUS;

/*TUSB320 registers */
typedef struct {
    uint8_t ACTIVE_CABLE_DETECTION:1;
    uint8_t ACCESSORY_CONNECTED:3;
    uint8_t CURRENT_MODE_DETECT:2;
    uint8_t CURRENT_MODE_ADVERTISE:2;

}tusb320Status1Bitfields;
typedef struct {
    uint8_t DISABLE_UFP_ACCESSORY:1;
    uint8_t DRP_DUTY_CYCLE:2;
    uint8_t dummy:1;
    uint8_t INTERRUPT_STATUS:1;
    uint8_t CABLE_DIR:1;
    uint8_t ATTACHED_STATE:2;
}tusb320Status2Bitfields;
typedef struct {
    uint8_t DISABLE_TERM:1;
    uint8_t SOURCE_PREF:2;
    uint8_t I2C_SOFT_RESET:1;
    uint8_t MODE_SELECT:2;
    uint8_t DEBOUNCE:2;
}tusb320Status3Bitfields;
typedef struct {
    uint8_t dummy2:2;
    uint8_t DISABLE_RD_RP:1;
    uint8_t dummy1:5;
}tusb320Status4Bitfields;
typedef union {
    uint8_t csr;
    tusb320Status1Bitfields status;    
}tusb320SCR0x08;
typedef union {
    uint8_t csr;
    tusb320Status2Bitfields status;    
}tusb320SCR0x09;
typedef union {
    uint8_t csr;
    tusb320Status3Bitfields status;    
}tusb320SCR0x0A;
typedef union {
    uint8_t csr;
    tusb320Status4Bitfields status;    
}tusb320SCR0x45;
typedef struct __attribute__((__packed__)) {
    uint8_t address;
    tusb320SCR0x08 csr;
}tusb320Status1;
typedef struct __attribute__((__packed__)) {
    uint8_t address;
    tusb320SCR0x09 csr;
}tusb320Status2;
typedef struct __attribute__((__packed__))  {
    uint8_t address;
    tusb320SCR0x0A csr;
}tusb320Status3;
typedef struct __attribute__((__packed__))  {
    uint8_t address;
    tusb320SCR0x45 csr;
}tusb320Status4;

/* As HID -> messages*/
typedef enum
{
    /* Request: Firmware version */
    USB_GET_FIRMWARE_VERSION = 10,
    /* Response: Firmware version */
    USB_FIRMWARE_VERSION,
    /* Request: Open a file on the SD-Card */
    USB_SDCARD_OPEN_FILE,
    /* Request: Close the opened file on the SD-Card */
    USB_SDCARD_CLOSE_FILE,
    /* Request: Write to the opened file on the SD-Card */
    USB_SDCARD_WRITE_FILE,
    /* Request: Read from the opened file on the SD-Card */
    USB_SDCARD_READ_FILE,
    /* Request: Create a new file on the SD-Card (create and open)*/
    USB_SDCARD_NEW_FILE,
    /* Request: Delete a file on the SD-Card (by name)*/
    USB_SDCARD_DELETE_FILE,
    /* Response: File opened (success / failure)*/
    USB_SDCARD_FILE_OPENED,
    /* Response:> File closed (success / failure)*/
    USB_SDCARD_FILE_CLOSED,
    /* Response: File written (success / failure)*/
    USB_SDCARD_FILE_WRITTEN,
    /* Response: File read + data(success / failure)*/
    USB_SDCARD_READ_FILE_READ,
    /* Response: File created (success / failure)*/
    USB_SDCARD_FILE_CREATED,
    /* Response: File deleted (success / failure)*/
    USB_SDCARD_FILE_DELETED,
    /* Request: Erase block size */
    USB_FLASH_BLOCK_SIZE,            
    /* Request: Erase flash (address, block-count)*/
    USB_ERASE_FLASH,
    /* Request: Write to flash (data, address, byte-count)*/
    USB_WRITE_TO_FLASH,
    /* Request: Read from flash (address, byte-count)*/
    USB_READ_FROM_FLASH,
    /* Response: Erase block size */
    USB_FLASH_BLOCK_SIZE_IS,            
    /* Response: Flash erase (success / failure)*/
    USB_FLASH_ERASED,
    /* Response: Flash written (success / failure)*/
    USB_FLASH_WRITTEN,
    /* Response: Flash read (data, success / failure)*/
    USB_FLASH_READ,
    /* HID USB error */        
    USB_REQUEST_ERROR = 100
} USB_MESSAGE;

typedef struct
{
    /* USB task current state */
    USB_STATES state;
    /* TUSB320 data */
    USB_TUSB320_I2C_STATES i2cState;
    TUSB320_TRANSFER_STATUS i2cTransferStatus;
    uint8_t csrAddress;
    uint8_t tusb320DeviceID[8];
    tusb320Status1 csr08;
    tusb320Status2 csr09;
    tusb320Status3 csr0A;
    tusb320Status4 csr45;
    bool tusb320Interrupt;
    uint8_t currentRole;    
    /* Keyboard USB host data */    
    USB_HOST_HID_KEYBOARD_DATA keyboardUsbHostData;
    bool keyPressedProcessed[USB_KEYBOARD_REPORTS_COUNT];
    USB_HOST_HID_KEYBOARD_HANDLE handle;
    bool capsLockPressed;
    bool scrollLockPressed;
    bool numLockPressed;
    uint8_t outputReport;
    /* HID USB device data */
    USB_DEVICE_HANDLE  usbDevHandle;
    uint8_t * receiveDataBuffer;
    uint8_t * transmitDataBuffer;
    bool deviceConfigured;
    bool powerDetected;
    USB_DEVICE_HID_TRANSFER_HANDLE txTransferHandle;
    USB_DEVICE_HID_TRANSFER_HANDLE rxTransferHandle;
    uint8_t configurationValue;
    bool hidDataReceived;
    bool hidDataTransmitted;
    uint8_t idleRate;
    /* Key press event callback */
    PrintableKeyboardKeyPressEventHandler onPrintableKeyPressEventHandler;
    ControlKeyboardKeyPressEventHandler onControlKeyPressEventHandler;
} USB_DATA;

typedef enum
{
    KEY_TYPE_NONE = 0,
    KEY_TYPE_CONTROL,
    KEY_TYPE_PRINTABLE,
    KEY_TYPE_PRINTABLE_CONTROL           
}KEY_TYPE;

typedef struct
{
    KEY_TYPE type;
    char * key;
    char * shift;
}KEY_VALUE;

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
void USB_Initialize ( void );
void USB_Tasks( void );

// *****************************************************************************
// *****************************************************************************
// Section: API functions 
// *****************************************************************************
// *****************************************************************************
void USB_PrintableKeyPressCallbackRegister(PrintableKeyboardKeyPressEventHandler keyPressEventHandler);
void USB_ControlKeyPressCallbackRegister(ControlKeyboardKeyPressEventHandler keyPressEventHandler);

// Provide C++ Compatibility
#ifdef __cplusplus
}
#endif

#endif /* _USB_H */

/*******************************************************************************
 End of File
 */

