/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Singular Software

  @File Name
    usb.c

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb.h"
#include "string.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data                                         
// *****************************************************************************
// *****************************************************************************

/* The TUSB320 device can be controlled using I2C */
#define TUSB320_DEVICE_ADDR             0x47
#define TUSB320_DEVICE_ID_CSR_ADDRESS   0x00 //Addresses 0x07 - 0x00 = {0x00 0x54 0x55 0x53 0x42 0x33 0x32 0x30}

#define TUSB320_STATUS_1_CSR_ADDRESS    0x08 //CURRENT_MODE_ADVERTISE[7:6] CURRENT_MODE_DETECT[5:4] ACCESSORY_CONNECTED[3:1] ACTIVE_CABLE_DETECTION[0]
#define TUSB320_STATUS_2_CSR_ADDRESS    0x09 //ATTACHED_STATE[7:6] CABLE_DIR[5] INTERRUPT_STATUS[4] ?[3] DRP_DUTY_CYCLE[2:1] DISABLE_UFP_ACCESSORY[0]
#define TUSB320_STATUS_3_CSR_ADDRESS    0x0A //DEBOUNCE[7:6] MODE_SELECT[5:4] I2C_SOFT_RESET[3] SOURCE_PREF[2:1] DISABLE_TERM[0]
#define TUSB320_STATUS_4_CSR_ADDRESS    0x45 //?[7:3] DISABLE_RD_RP[2] ?[1:0]]

#define USB_NONE    0x01
#define USB_HOST    0x02
#define USB_DEVICE  0x03

/* Receive data buffer */
uint8_t receiveDataBuffer[64] CACHE_ALIGN;
/* Transmit data buffer */
uint8_t  transmitDataBuffer[64] CACHE_ALIGN;

/* ID to Key map table */
const KEY_VALUE ENG_US_KEY[] = {
    {KEY_TYPE_NONE, "NO_EVENT_INDICATED", "NO_EVENT_INDICATED"},
    {KEY_TYPE_NONE, "ERROR_ROLL_OVER", "ERROR_ROLL_OVER"},
    {KEY_TYPE_NONE, "POST_FAIL", "POST_FAIL"},
    {KEY_TYPE_NONE, "ERROR_UNDEFINED", "ERROR_UNDEFINED"},
    {KEY_TYPE_PRINTABLE, "a", "A"},
    {KEY_TYPE_PRINTABLE, "b", "B"},
    {KEY_TYPE_PRINTABLE, "c", "C"},
    {KEY_TYPE_PRINTABLE, "d", "D"},
    {KEY_TYPE_PRINTABLE, "e", "E"},
    {KEY_TYPE_PRINTABLE, "f", "F"},
    {KEY_TYPE_PRINTABLE, "g", "G"},
    {KEY_TYPE_PRINTABLE, "h", "H"},
    {KEY_TYPE_PRINTABLE, "i", "I"},
    {KEY_TYPE_PRINTABLE, "j", "J"},
    {KEY_TYPE_PRINTABLE, "k", "K"},
    {KEY_TYPE_PRINTABLE, "l", "L"},
    {KEY_TYPE_PRINTABLE, "m", "M"},
    {KEY_TYPE_PRINTABLE, "n", "N"},
    {KEY_TYPE_PRINTABLE, "o", "O"},
    {KEY_TYPE_PRINTABLE, "p", "P"},
    {KEY_TYPE_PRINTABLE, "q", "Q"},
    {KEY_TYPE_PRINTABLE, "r", "R"},
    {KEY_TYPE_PRINTABLE, "s", "S"},
    {KEY_TYPE_PRINTABLE, "t", "T"},
    {KEY_TYPE_PRINTABLE, "u", "U"},
    {KEY_TYPE_PRINTABLE, "v", "V"},
    {KEY_TYPE_PRINTABLE, "w", "Q"},
    {KEY_TYPE_PRINTABLE, "x", "X"},
    {KEY_TYPE_PRINTABLE, "y", "Y"},
    {KEY_TYPE_PRINTABLE, "z", "Z"},
    {KEY_TYPE_PRINTABLE, "1", "!"},
    {KEY_TYPE_PRINTABLE, "2", "@"},
    {KEY_TYPE_PRINTABLE, "3", "#"},
    {KEY_TYPE_PRINTABLE, "4", "$"},
    {KEY_TYPE_PRINTABLE, "5", "%"},
    {KEY_TYPE_PRINTABLE, "6", "^"},
    {KEY_TYPE_PRINTABLE, "7", "&"},
    {KEY_TYPE_PRINTABLE, "8", "*"},
    {KEY_TYPE_PRINTABLE, "9", "("},
    {KEY_TYPE_PRINTABLE, "0", ")"},
    {KEY_TYPE_PRINTABLE, "\r", "\r"},
    {KEY_TYPE_CONTROL, "ESCAPE", "ESCAPE"},
    {KEY_TYPE_CONTROL, "DELETE", "DELETE"},
    {KEY_TYPE_CONTROL, "TAB", "TAB"},
    {KEY_TYPE_PRINTABLE, " ", " "},
    {KEY_TYPE_PRINTABLE, "-", "_"},
    {KEY_TYPE_PRINTABLE, "=", "+"},
    {KEY_TYPE_PRINTABLE, "[", "{"},
    {KEY_TYPE_PRINTABLE, "]", "}"},
    {KEY_TYPE_PRINTABLE, "\\0", "|"},
    {KEY_TYPE_PRINTABLE, "#", "~"},
    {KEY_TYPE_PRINTABLE, ";", ":"},
    {KEY_TYPE_PRINTABLE, "'", "\""},
    {KEY_TYPE_PRINTABLE, "`", "~"},
    {KEY_TYPE_PRINTABLE, ",", "<"},
    {KEY_TYPE_PRINTABLE, ".", ">"},
    {KEY_TYPE_PRINTABLE, "/", "?"},
    {KEY_TYPE_CONTROL, "CAPS_LOCK", "CAPS_LOCK"},
    {KEY_TYPE_CONTROL, "F1", "F1"},
    {KEY_TYPE_CONTROL, "F2", "F2"},
    {KEY_TYPE_CONTROL, "F3", "F3"},
    {KEY_TYPE_CONTROL, "F4", "F4"},
    {KEY_TYPE_CONTROL, "F5", "F5"},
    {KEY_TYPE_CONTROL, "F6", "F6"},
    {KEY_TYPE_CONTROL, "F7", "F7"},
    {KEY_TYPE_CONTROL, "F8", "F8"},
    {KEY_TYPE_CONTROL, "F9", "F9"},
    {KEY_TYPE_CONTROL, "F10", "F10"},
    {KEY_TYPE_CONTROL, "F11", "F11"},
    {KEY_TYPE_CONTROL, "F12", "F12"},                              
    {KEY_TYPE_CONTROL, "PRINT_SCREEN", "PRINT_SCREEN"},
    {KEY_TYPE_CONTROL, "SCROLL_LOCK", "SCROLL_LOCK"},
    {KEY_TYPE_CONTROL, "PAUSE", "PAUSE"},
    {KEY_TYPE_CONTROL, "INSERT", "INSERT"},
    {KEY_TYPE_CONTROL, "HOME", "HOME"},
    {KEY_TYPE_CONTROL, "PAGE_UP", "PAGE_UP"},
    {KEY_TYPE_CONTROL, "DELETE", "DELETE"},
    {KEY_TYPE_CONTROL, "END", "END"},
    {KEY_TYPE_CONTROL, "PAGE_DOWN", "PAGE_DOWN"},
    {KEY_TYPE_CONTROL, "RIGHT_ARROW", "RIGHT_ARROW"},
    {KEY_TYPE_CONTROL, "LEFT_ARROW", "LEFT_ARROW"},
    {KEY_TYPE_CONTROL, "DOWN_ARROW", "DOWN_ARROW"},
    {KEY_TYPE_CONTROL, "UP_ARROW", "UP_ARROW"},
    {KEY_TYPE_CONTROL, "NUM_LOCK", "NUM_LOCK"},         //NUM KEYPAD
    {KEY_TYPE_PRINTABLE, "/", "/"},                     //NUM KEYPAD
    {KEY_TYPE_PRINTABLE, "*", "*"},                     //NUM KEYPAD
    {KEY_TYPE_PRINTABLE, "-", "-"},                     //NUM KEYPAD
    {KEY_TYPE_PRINTABLE, "+", "+"},                     //NUM KEYPAD
    {KEY_TYPE_PRINTABLE, "\r", "\r"},               //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "1", "END"},           //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "2", "DOWN_ARROW"},    //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "3", "PAGE_DOWN"},     //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "4", "LEFT_ARROW"},    //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "5", "5"},             //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "6", "RIGHT_ARROW"},   //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "7", "HOME"},          //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "8", "UP_ARROW"},      //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "9", "PAGE_UP"},       //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, "0", "INSERT"},        //NUM KEYPAD
    {KEY_TYPE_PRINTABLE_CONTROL, ".", "DELETE"},        //NUM KEYPAD
    {KEY_TYPE_CONTROL, "\\", "|"},
    {KEY_TYPE_CONTROL, "APPLICATION", "APPLICATION"},
    {KEY_TYPE_CONTROL, "POWER", "POWER"},
    {KEY_TYPE_CONTROL, "EQUAL_SIZE", "EQUAL_SIZE"}    
};

USB_DATA CACHE_ALIGN usbData;

// *****************************************************************************
// *****************************************************************************
// Section: Callback Functions
// *****************************************************************************
// *****************************************************************************
/*******************************************************
 * TUSB320 I2C callbacks
 *******************************************************/
void TUSB320_I2C_Callback(uintptr_t context )
{
    TUSB320_TRANSFER_STATUS* transferStatus = (TUSB320_TRANSFER_STATUS*)context;

    if(I2C3_ErrorGet() == I2C_ERROR_NONE)
    {
        if (transferStatus)
        {
            *transferStatus = TUSB320_TRANSFER_STATUS_SUCCESS;
        }
    }
    else
    {
        if (transferStatus)
        {
            *transferStatus = TUSB320_TRANSFER_STATUS_ERROR;
        }
    }
}
void TUSB320_INT_Callback(void)
{
    usbData.tusb320Interrupt = true;
}

/*******************************************************
 * UNIT AS USB DEVICE
 *******************************************************/
USB_DEVICE_HID_EVENT_RESPONSE AsDeviceHIDEventHandler(USB_DEVICE_HID_INDEX iHID,
                                                       USB_DEVICE_HID_EVENT event,
                                                       void * eventData,
                                                       uintptr_t userData)
{
    USB_DEVICE_HID_EVENT_DATA_REPORT_SENT * reportSent;
    USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED * reportReceived;

    /* Check type of event */
    switch (event)
    {
        case USB_DEVICE_HID_EVENT_REPORT_SENT:

            /* The eventData parameter will be USB_DEVICE_HID_EVENT_REPORT_SENT
             * pointer type containing details about the report that was sent. */
            reportSent = (USB_DEVICE_HID_EVENT_DATA_REPORT_SENT *) eventData;
            if(reportSent->handle == usbData.txTransferHandle )
            {
                // Transfer progressed.
                usbData.hidDataTransmitted = true;
            }
            
            break;

        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:

            /* The eventData parameter will be USB_DEVICE_HID_EVENT_REPORT_RECEIVED
             * pointer type containing details about the report that was received. */
            reportReceived = (USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED *) eventData;
            if(reportReceived->handle == usbData.rxTransferHandle )
            {
                // Transfer progressed.
                usbData.hidDataReceived = true;
            }
          
            break;

        case USB_DEVICE_HID_EVENT_SET_IDLE:

            /* For now we just accept this request as is. We acknowledge
             * this request using the USB_DEVICE_HID_ControlStatus()
             * function with a USB_DEVICE_CONTROL_STATUS_OK flag */
            USB_DEVICE_ControlStatus(usbData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);

            /* Save Idle rate recieved from Host */
            usbData.idleRate = ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)eventData)->duration;
            break;

        case USB_DEVICE_HID_EVENT_GET_IDLE:

            /* Host is requesting for Idle rate. Now send the Idle rate */
            USB_DEVICE_ControlSend(usbData.usbDevHandle, & (usbData.idleRate),1);

            /* On successfully receiving Idle rate, the Host would acknowledge back with a
               Zero Length packet. The HID function driver returns an event
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT upon
               receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates this control transfer
               event is complete */

            break;
        default:
            // Nothing to do.
            break;
    }
    return USB_DEVICE_HID_EVENT_RESPONSE_NONE;
}

void AsDeviceEventHandler(USB_DEVICE_EVENT event, void * eventData, uintptr_t context)
{
    switch(event)
    {
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            /* Host has de configured the device or a bus reset has happened.
             * Device layer is going to de-initialize all function drivers.
             * Hence close handles to all function drivers (Only if they are
             * opened previously. */
            usbData.deviceConfigured = false;
            //usbData.state = USB_STATE_WAIT_FOR_CONFIGURATION;
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            /* Set the flag indicating device is configured. */
            usbData.deviceConfigured = true;

            /* Save the other details for later use. */
            usbData.configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData)->configurationValue;

            /* Register HID event handler */
            USB_DEVICE_HID_EventHandlerSet(USB_DEVICE_HID_INDEX_0, AsDeviceHIDEventHandler, (uintptr_t)&usbData);
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
		
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            usbData.powerDetected = true;
            //USB_DEVICE_Attach (usbData.usbDevHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:
            /* VBUS is not available */
            usbData.powerDetected = false;
            //USB_DEVICE_Detach(usbData.usbDevHandle);
            break;

        /* These events are not used in this demo */
        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}

/*******************************************************
 * USB AS USB HOST
 *******************************************************/
USB_HOST_EVENT_RESPONSE AsHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)
{
    switch (event)
    {
        /* This event occurs when device needs more current than what the host can supply.*/ 
        case USB_HOST_EVENT_DEVICE_REJECTED_INSUFFICIENT_POWER:
            break;
        /* This event occurs when a host layer could not attach any drivers to the
           attached device or when an error has occurred. There is no event data
           associated with this event. */
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        /* This event occurs when the number of hubs connected to the host exceeds the
           configured maximum number of hubs USB_HOST_HUB_TIER_LEVEL. There is no event
           data associated with this event. */
        case USB_HOST_EVENT_HUB_TIER_LEVEL_EXCEEDED:
            break;

        /* This event occurs when an over-current condition is detected at the root
           hub or an external hub port.*/
        case USB_HOST_EVENT_PORT_OVERCURRENT_DETECTED:
            break;
        default:
            break;
    }    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}
/*******************************************************
 * USB HOST HID Layer Events
 *******************************************************/
void AsHostHIDKeyboardEventHandler(USB_HOST_HID_KEYBOARD_HANDLE handle, 
                                USB_HOST_HID_KEYBOARD_EVENT event, void * pData)
{   
    switch ( event)
    {
        case USB_HOST_HID_KEYBOARD_EVENT_ATTACH:
            usbData.handle = handle;
            usbData.state =  USB_STATE_DEVICE_ATTACHED;
            usbData.capsLockPressed = false;
            usbData.scrollLockPressed = false;
            usbData.numLockPressed = false;
            usbData.outputReport = 0;
            break;

        case USB_HOST_HID_KEYBOARD_EVENT_DETACH:
            usbData.handle = handle;
            usbData.state = USB_STATE_DEVICE_DETACHED;
            usbData.capsLockPressed = false;
            usbData.scrollLockPressed = false;
            usbData.numLockPressed = false;
            usbData.outputReport = 0;
            break;

        case USB_HOST_HID_KEYBOARD_EVENT_REPORT_RECEIVED:
            usbData.handle = handle;
            usbData.state = USB_STATE_HOST_MAIN_TASK;
            /* Keyboard Data from device */
            memcpy(&usbData.keyboardUsbHostData, pData, sizeof(usbData.keyboardUsbHostData));
            break;

        default:
            break;
    }
    return;
}

// *****************************************************************************
// *****************************************************************************
// Section: Local Functions
// *****************************************************************************
// *****************************************************************************
static void SetGetStatusState(void)
{
    usbData.csrAddress =  0;
    usbData.i2cState = USB_STATE_TUSB320_SET_CSR_ADDRESS;
    usbData.state = USB_STATE_TUSB320_GET_STATUS;
}

static void USBSendReceive(void)
{
    usbData.hidDataTransmitted = false;
    /* Prepare the USB module to send the data packet to the host */
    USB_DEVICE_HID_ReportSend (USB_DEVICE_HID_INDEX_0,
            &usbData.txTransferHandle, usbData.transmitDataBuffer, 64 );

    usbData.hidDataReceived = false;
    /* Place a new read request. */
    USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
            &usbData.rxTransferHandle, usbData.receiveDataBuffer, 64 );

}

static void AsHostMapKeyToUsage(USB_HID_KEYBOARD_KEYPAD keyCode)
{
    if(keyCode < 0  || keyCode >= sizeof(ENG_US_KEY)/sizeof(KEY_VALUE))
    {
        return;
    }
    
    switch(ENG_US_KEY[keyCode].type)
    {
        /* SKIP */
        case KEY_TYPE_NONE:
        {
        }
        break;

        /* UPDATE CONTROLS STATE*/
        case KEY_TYPE_CONTROL:
        {
            bool sendReport = false;
            uint8_t outputReport = usbData.outputReport;
            char* s = ENG_US_KEY[keyCode].key;
            switch(keyCode)
            {
                case USB_HID_KEYBOARD_KEYPAD_KEYBOARD_CAPS_LOCK:
                {
                    /* CAPS LOCK pressed */
                    if(usbData.capsLockPressed == false)
                    {
                        usbData.capsLockPressed = true;
                        outputReport = outputReport | 0x2;
                    }
                    else
                    {
                        usbData.capsLockPressed = false;
                        outputReport = outputReport & 0xFD;
                    }
                    sendReport = true;
                }
                break;

                case USB_HID_KEYBOARD_KEYPAD_KEYBOARD_SCROLL_LOCK:
                {
                    /* SCROLL LOCK pressed */
                    if(usbData.scrollLockPressed == false)
                    {
                        usbData.scrollLockPressed = true;
                        outputReport = outputReport | 0x4;
                    }
                    else
                    {
                        usbData.scrollLockPressed = false;
                        outputReport = outputReport & 0xFB;
                    }
                    sendReport = true;
                }
                break;

                case USB_HID_KEYBOARD_KEYPAD_KEYPAD_NUM_LOCK_AND_CLEAR:
                {
                    /* NUM LOCK pressed */
                    if(usbData.numLockPressed == false)
                    {
                        usbData.numLockPressed = true;
                        outputReport = outputReport | 0x1;
                    }
                    else
                    {
                        usbData.numLockPressed = false;
                        outputReport = outputReport & 0xFE;
                    }
                    sendReport = true;
                }
                break;
                default:
                    break;
            }
            
            if(sendReport)
            {
                /* Store the changes */
                usbData.outputReport = outputReport;
                /* Send the OUTPUT Report */
                USB_HOST_HID_KEYBOARD_ReportSend(usbData.handle, outputReport);
            }
            
            if(usbData.onControlKeyPressEventHandler != NULL)
            {
                (*usbData.onControlKeyPressEventHandler )(s);
            }
        }
        break;

        /* PRINTABLES */
        case KEY_TYPE_PRINTABLE:
        {
            bool shift = usbData.keyboardUsbHostData.modifierKeysData.leftShift ||
                         usbData.keyboardUsbHostData.modifierKeysData.rightShift;

            bool uppercase = (usbData.capsLockPressed && !shift) ||
                             (!usbData.capsLockPressed && shift);

            char c = *ENG_US_KEY[keyCode].key;

            /* a - z */
            if (keyCode >= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_A &&
                keyCode <= USB_HID_KEYBOARD_KEYPAD_KEYBOARD_Z)
            {
                if(uppercase)
                {
                    c = *ENG_US_KEY[keyCode].shift;
                }
            }
            else
            {
                if(shift)
                {
                    c = *ENG_US_KEY[keyCode].shift;
                }
            }

            if(usbData.onPrintableKeyPressEventHandler != NULL)
            {
                (*usbData.onPrintableKeyPressEventHandler )(c);
            }
        }
        break;

        /* NUM LOCK KEYPAD */
        case KEY_TYPE_PRINTABLE_CONTROL:
        {
            if(usbData.numLockPressed)
            {
                char c = *ENG_US_KEY[keyCode].key;
                if(usbData.onPrintableKeyPressEventHandler != NULL)
                {
                    (*usbData.onPrintableKeyPressEventHandler )(c);
                }
            }
            else
            {                
                char* s = ENG_US_KEY[keyCode].shift;
                if(usbData.onControlKeyPressEventHandler != NULL)
                {
                    (*usbData.onControlKeyPressEventHandler )(s);
                }
            }            
        }
        break;
    }
}

static void StateTUSB320ReadDeviceID(void)
{
    switch(usbData.i2cState)
    {
        case USB_STATE_TUSB320_SET_CSR_ADDRESS://set TUSB320 start CSR address to read
            usbData.csrAddress = TUSB320_DEVICE_ID_CSR_ADDRESS;
            usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_IN_PROGRESS;
            I2C3_Write(TUSB320_DEVICE_ADDR, &usbData.csrAddress, sizeof(usbData.csrAddress));
            usbData.i2cState = USB_STATE_TUSB320_CSR_READ;
            break;
        case USB_STATE_TUSB320_CSR_READ:
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {
                //read device id address 07->00
                usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_IN_PROGRESS;

                I2C3_Read(TUSB320_DEVICE_ADDR, &usbData.tusb320DeviceID[0], sizeof(usbData.tusb320DeviceID));
                usbData.i2cState = USB_STATE_TUSB320_VERIFY_CSR_READ;
            }
            break;
        case USB_STATE_TUSB320_VERIFY_CSR_READ:
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {
                SetGetStatusState();
            }
            break;
        default:
            break;                   
    }    
}

static void StateTUSB320GetStatus(void)
{
    switch(usbData.i2cState)
    {
        case USB_STATE_TUSB320_SET_CSR_ADDRESS://set TUSB320 start CSR address to read

            switch(usbData.csrAddress)
            {
                case 0:
                    usbData.csrAddress = usbData.csr08.address;
                    break;
                case TUSB320_STATUS_1_CSR_ADDRESS:
                    usbData.csrAddress = usbData.csr09.address;
                    break;
                case TUSB320_STATUS_2_CSR_ADDRESS:
                    usbData.csrAddress = usbData.csr0A.address;
                    break;
                case TUSB320_STATUS_3_CSR_ADDRESS:
                    usbData.csrAddress = usbData.csr45.address;
                    break;
                default:
                    usbData.csrAddress =  0;
                    break;
            }
            if(usbData.csrAddress == 0)//no more status to get -> if interrupt set set state to clear it else go to set role
            {
                if(usbData.csr09.csr.status.INTERRUPT_STATUS == 1)
                {
                    usbData.state = USB_STATE_TUSB320_RESET_INT_STATUS;
                    usbData.i2cState = USB_STATE_TUSB320_CSR_WRITE;
                }
                else
                {
                    usbData.state = USB_STATE_TUSB320_CHECK_STATUS;
                }
            }
            else
            {
                usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_IN_PROGRESS;
                I2C3_Write(TUSB320_DEVICE_ADDR, &usbData.csrAddress, sizeof(usbData.csrAddress));
                usbData.i2cState = USB_STATE_TUSB320_CSR_READ;
            }
            break;
        case USB_STATE_TUSB320_CSR_READ:
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {
                usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_IN_PROGRESS;
                switch(usbData.csrAddress)
                {
                    case TUSB320_STATUS_1_CSR_ADDRESS:
                        I2C3_Read(TUSB320_DEVICE_ADDR, &usbData.csr08.csr.csr, sizeof(usbData.csr08.csr.csr));
                        break;
                    case TUSB320_STATUS_2_CSR_ADDRESS:
                        I2C3_Read(TUSB320_DEVICE_ADDR, &usbData.csr09.csr.csr, sizeof(usbData.csr09.csr.csr));
                        break;
                    case TUSB320_STATUS_3_CSR_ADDRESS:
                        I2C3_Read(TUSB320_DEVICE_ADDR, &usbData.csr0A.csr.csr, sizeof(usbData.csr0A.csr.csr));
                        break;
                    case TUSB320_STATUS_4_CSR_ADDRESS:
                        I2C3_Read(TUSB320_DEVICE_ADDR, &usbData.csr45.csr.csr, sizeof(usbData.csr45.csr.csr));
                        break;
                }
                usbData.i2cState = USB_STATE_TUSB320_VERIFY_CSR_READ;
            }
            break;

        case USB_STATE_TUSB320_VERIFY_CSR_READ:
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {                        
                //next
                usbData.i2cState = USB_STATE_TUSB320_SET_CSR_ADDRESS;
            }
            break;

        default:
            break;

    }    
}

static void StateTUSB320ResetINT(void)
{
    switch(usbData.i2cState)
    {
        case USB_STATE_TUSB320_CSR_WRITE:
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {
                //write status1 to clear interrupt
                usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_IN_PROGRESS;
                I2C3_Write(TUSB320_DEVICE_ADDR, (uint8_t*)&usbData.csr09, sizeof(usbData.csr09));

                usbData.i2cState = USB_STATE_TUSB320_VERIFY_CSR_WRITE_1;
            }
            break;
        case USB_STATE_TUSB320_VERIFY_CSR_WRITE_1://set TUSB320 start CSR address to read
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {                        
                usbData.csrAddress = TUSB320_STATUS_2_CSR_ADDRESS;
                usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_IN_PROGRESS;
                I2C3_Write(TUSB320_DEVICE_ADDR, &usbData.csrAddress, sizeof(usbData.csrAddress));
                usbData.i2cState = USB_STATE_TUSB320_VERIFY_CSR_WRITE_2;
            }
            break;
        case USB_STATE_TUSB320_VERIFY_CSR_WRITE_2:
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {                        
                usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_IN_PROGRESS;
                I2C3_Read(TUSB320_DEVICE_ADDR, &usbData.csr09.csr.csr, sizeof(usbData.csr09.csr.csr));
                usbData.i2cState = USB_STATE_TUSB320_VERIFY_CSR_WRITE_3;
            }
            break;
        case USB_STATE_TUSB320_VERIFY_CSR_WRITE_3:
            if (usbData.i2cTransferStatus == TUSB320_TRANSFER_STATUS_SUCCESS)
            {                        
                usbData.tusb320Interrupt = false;
                if(usbData.csr09.csr.status.INTERRUPT_STATUS == 1)
                {
                    usbData.state = USB_STATE_TUSB320_CHECK_STATUS;
                    usbData.i2cState = USB_STATE_TUSB320_SET_CSR_ADDRESS;
                }
                else
                {
                    usbData.state = USB_DEVICE_STATE_INIT;
                }
            }
            break;
        default:
            break;
    }
}

static void StateAsHostMainTask(void)
{
    for(int count = 0; count < USB_KEYBOARD_REPORTS_COUNT; count++)
    {
        switch(usbData.keyboardUsbHostData.nonModifierKeysData[count].event)
        {
            case USB_HID_KEY_RELEASED:
            {
                if (usbData.keyPressedProcessed[count])
                {
                    usbData.keyPressedProcessed[count] = false;
                }
            }
            break;
            case USB_HID_KEY_PRESSED:
            {
                if(!usbData.keyPressedProcessed[count])
                {
                    AsHostMapKeyToUsage(usbData.keyboardUsbHostData.nonModifierKeysData[count].keyCode);
                    usbData.keyPressedProcessed[count] = true;
                }
            }
            break;
        }
    }
}

static void StateAsDeviceMainTask(void)
{
    if(!usbData.deviceConfigured)
    {
        /* Device is not configured */
        usbData.state = USB_STATE_WAIT_FOR_CONFIGURATION;
    }
    else if( usbData.hidDataReceived )
    {
        switch(usbData.receiveDataBuffer[0])
        {                   
            case USB_GET_FIRMWARE_VERSION:
                if(usbData.hidDataTransmitted)
                {
                    usbData.transmitDataBuffer[0] = USB_FIRMWARE_VERSION;
                    usbData.transmitDataBuffer[1] = FIRMWARE_VERSION_MAJOR;
                    usbData.transmitDataBuffer[2] = FIRMWARE_VERSION_MINOR;
                    usbData.transmitDataBuffer[3] = FIRMWARE_VERSION_BUILD;

                    USBSendReceive();                        
                }
                break;
                
            case USB_SDCARD_OPEN_FILE:
                if(usbData.hidDataTransmitted)
                {
                    if(SDCARD_OpenFile(&usbData.receiveDataBuffer[2]))
                    {
                        usbData.transmitDataBuffer[0] = USB_SDCARD_FILE_OPENED;
                        int32_t fileSize = SDCARD_FileSize();
                        memcpy(&usbData.transmitDataBuffer[1], &fileSize, sizeof(int32_t));
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SDCARD_GetLastError();
                    }                    

                    USBSendReceive();
                }
                break;

            case USB_SDCARD_CLOSE_FILE:
                if(usbData.hidDataTransmitted)
                {
                    if(SDCARD_CloseFile())
                    {
                        usbData.transmitDataBuffer[0] = USB_SDCARD_FILE_CLOSED;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SDCARD_GetLastError();
                    }                    

                    USBSendReceive();
                }
                break;

            case USB_SDCARD_WRITE_FILE:
                if(usbData.hidDataTransmitted)
                {
                    size_t writtenSize;
                    bool flush = usbData.receiveDataBuffer[1] != 0;
                    if (SDCARD_WriteFile(&usbData.receiveDataBuffer[3], usbData.receiveDataBuffer[2], &writtenSize, flush))
                    {
                        usbData.transmitDataBuffer[0] = USB_SDCARD_FILE_WRITTEN;
                        usbData.transmitDataBuffer[1] = writtenSize;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SDCARD_GetLastError();
                    }                    

                    USBSendReceive();
                }
                break;

            case USB_SDCARD_READ_FILE:
                if(usbData.hidDataTransmitted)
                {
                    size_t readSize;
                    if (SDCARD_ReadFile(&usbData.transmitDataBuffer[2], usbData.receiveDataBuffer[1], &readSize))
                    {
                        usbData.transmitDataBuffer[0] = USB_SDCARD_READ_FILE_READ;
                        usbData.transmitDataBuffer[1] = readSize;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SDCARD_GetLastError();
                    }                    

                    USBSendReceive();
                }
                break;

            case USB_SDCARD_NEW_FILE:
                if(usbData.hidDataTransmitted)
                {
                    if (SDCARD_NewFile(&usbData.receiveDataBuffer[2]))
                    {
                        usbData.transmitDataBuffer[0] = USB_SDCARD_FILE_CREATED;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SDCARD_GetLastError();
                    }                    
                    
                    USBSendReceive();
                }
                break;

            case USB_SDCARD_DELETE_FILE:
                if(usbData.hidDataTransmitted)
                {
                    if (SDCARD_DeleteFile(&usbData.receiveDataBuffer[2]))
                    {
                        usbData.transmitDataBuffer[0] = USB_SDCARD_FILE_DELETED;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SDCARD_GetLastError();
                    }                    

                    USBSendReceive();
                }
                break;
            
            case USB_FLASH_BLOCK_SIZE:
                if(usbData.hidDataTransmitted)
                {
                    usbData.transmitDataBuffer[0] = USB_FLASH_BLOCK_SIZE_IS;
                    uint32_t block_size = SST26_BlockSize();
                    memcpy(&usbData.transmitDataBuffer[1], &block_size, sizeof(uint32_t));

                    USBSendReceive();
                }
                break;               
                
            case USB_ERASE_FLASH:
                if(usbData.hidDataTransmitted)
                {
                    uint32_t address;
                    uint32_t blockCount;
                    memcpy(&address, &usbData.receiveDataBuffer[2], sizeof(uint32_t));
                    memcpy(&blockCount, &usbData.receiveDataBuffer[6], sizeof(uint32_t));
                                        
                    if(SST26_Erase(address, blockCount))
                    {
                        usbData.transmitDataBuffer[0] = USB_FLASH_ERASED;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SST26_GetLastStatus();
                    }

                    USBSendReceive();
                }
                break;

            case USB_WRITE_TO_FLASH:
                if(usbData.hidDataTransmitted)
                {
                    uint32_t address;
                    uint32_t size;
                    memcpy(&address, &usbData.receiveDataBuffer[2], sizeof(uint32_t));
                    memcpy(&size, &usbData.receiveDataBuffer[6], sizeof(uint32_t));
                    uint8_t* buffer = &usbData.receiveDataBuffer[10];

                    if (SST26_Write(buffer, size, address))
                    {
                        usbData.transmitDataBuffer[0] = USB_FLASH_WRITTEN;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SST26_GetLastStatus();
                    }            

                    USBSendReceive();
                }
                break;

            case USB_READ_FROM_FLASH:
                if(usbData.hidDataTransmitted)
                {
                    uint32_t address;
                    uint32_t size;
                    memcpy(&address, &usbData.receiveDataBuffer[2], sizeof(uint32_t));
                    memcpy(&size, &usbData.receiveDataBuffer[6], sizeof(uint32_t));
                    uint8_t* buffer = &usbData.transmitDataBuffer[10];

                    if (SST26_Read(buffer, size, address))
                    {
                        usbData.transmitDataBuffer[0] = USB_FLASH_READ;
                    }
                    else
                    {
                        usbData.transmitDataBuffer[0] = USB_REQUEST_ERROR;
                        usbData.transmitDataBuffer[1] = SST26_GetLastStatus();
                    }            
                    
                    USBSendReceive();
                }
                break;


            default:
                usbData.hidDataReceived = false;
                /* Place a new read request. */
                USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
                        &usbData.rxTransferHandle, usbData.receiveDataBuffer, 64 );
                break;
        }
    }
    
}


// *****************************************************************************
// *****************************************************************************
// Section: Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
void USB_Initialize ( void )
{
    usbData.state = USB_STATE_INIT;
            
    usbData.usbDevHandle = USB_DEVICE_HANDLE_INVALID;
    usbData.deviceConfigured = false;
    usbData.txTransferHandle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
    usbData.rxTransferHandle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
    usbData.hidDataReceived = false;
    usbData.hidDataTransmitted = true;
    usbData.receiveDataBuffer = &receiveDataBuffer[0];
    usbData.transmitDataBuffer = &transmitDataBuffer[0];
    
    usbData.tusb320Interrupt = false;
    
    usbData.onPrintableKeyPressEventHandler = NULL;
    usbData.onControlKeyPressEventHandler = NULL;
    
    for(int i=0; i < sizeof(usbData.keyPressedProcessed); i++)
    {
        usbData.keyPressedProcessed[i] = false;
    }
}

void USB_Tasks (void )
{
    switch(usbData.state)
    {
        case USB_STATE_INIT:
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);

            usbData.i2cTransferStatus = TUSB320_TRANSFER_STATUS_ERROR;
            //set i2c callback
            I2C3_CallbackRegister(TUSB320_I2C_Callback, (uintptr_t)&usbData.i2cTransferStatus );

            GPIO_PinInterruptCallbackRegister(TUSB320_INT_PIN, (GPIO_PIN_CALLBACK)TUSB320_INT_Callback, 0);
            TUSB320_INT_InterruptEnable();

            usbData.csr08.address = TUSB320_STATUS_1_CSR_ADDRESS;
            usbData.csr09.address = TUSB320_STATUS_2_CSR_ADDRESS;
            usbData.csr0A.address = TUSB320_STATUS_3_CSR_ADDRESS;
            usbData.csr45.address = TUSB320_STATUS_4_CSR_ADDRESS;
    
            //first get TUSB320 device id
            usbData.state = USB_STATE_TUSB320_READ_DEVICE_ID;
            usbData.i2cState = USB_STATE_TUSB320_SET_CSR_ADDRESS;
        }
            break;

        case USB_STATE_TUSB320_READ_DEVICE_ID:
            StateTUSB320ReadDeviceID();//next USB_STATE_TUSB320_GET_STATUS
            break;

        case USB_STATE_TUSB320_GET_STATUS:
            StateTUSB320GetStatus();//next USB_STATE_TUSB320_CHECK_STATUS or USB_STATE_TUSB320_RESET_INT_STATUS if INT is set
            break;
            
        case USB_STATE_TUSB320_RESET_INT_STATUS:
            StateTUSB320ResetINT();//next USB_STATE_TUSB320_CHECK_STATUS
            break;
            
        case USB_STATE_TUSB320_CHECK_STATUS:
            //check ok start usb role
            usbData.state = USB_DEVICE_STATE_INIT;
            break;

        case USB_DEVICE_STATE_INIT:
             /* Register a callback with host layer to get event notification */
            USB_HOST_EventHandlerSet(AsHostEventHandler, 0);
            USB_HOST_HID_KEYBOARD_EventHandlerSet(AsHostHIDKeyboardEventHandler);
            
            /* Open the device layer */
            if(usbData.usbDevHandle == USB_DEVICE_HANDLE_INVALID)//one time
            {
                usbData.usbDevHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );

                if(usbData.usbDevHandle != USB_DEVICE_HANDLE_INVALID)
                {
                    /* Register a callback with device layer to get event notification (for end point 0) */
                    USB_DEVICE_EventHandlerSet(usbData.usbDevHandle, AsDeviceEventHandler, 0);
                    usbData.state = USB_STATE_SELECT_USB_ROLE;
                }
                else
                {
                    /* The Device Layer is not ready to be opened. We should try
                     * again later. */
                }
            }
            else
            {
                usbData.state = USB_STATE_SELECT_USB_ROLE;
            }

            break;
            
        case USB_STATE_SELECT_USB_ROLE:      
            switch (usbData.csr09.csr.status.ATTACHED_STATE)
            {
                case 0: //00 ? Not attached (default)
                    USER_CONSOLE_PRINT("\r\nNot attached (default)\r\n");

                    if(usbData.currentRole == USB_DEVICE)
                    {
                        if(USB_DEVICE_StateGet(usbData.usbDevHandle)== USB_DEVICE_STATE_ATTACHED)
                        {
                            USB_DEVICE_Detach(usbData.usbDevHandle);
                        }
                    }
                    else if(usbData.currentRole == USB_HOST)
                    {
                        USB_HOST_BusDisable(0);
                        while(USB_HOST_RESULT_TRUE != USB_HOST_BusIsDisabled(0));
                    }

                    usbData.currentRole = USB_NONE;
                    usbData.state = USB_STATE_IDLE;
                    break;
                    
                case 1: //01 ? Attached.SRC (DFP)
                    USER_CONSOLE_PRINT("\r\nAttached.SRC (DFP)\r\n");
                    
                    //if last usb state was device detach
                    if(USB_DEVICE_StateGet(usbData.usbDevHandle)== USB_DEVICE_STATE_ATTACHED)
                    {
                        USB_DEVICE_Detach(usbData.usbDevHandle);
                    }
                    usbData.state = USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
                    USB_HOST_BusEnable(0);
                    usbData.currentRole = USB_HOST ;
                    break;
                case 2: //10 ? Attached.SNK (UFP)
                    USER_CONSOLE_PRINT("\r\nAttached.SNK (UFP)\r\n");

                    //if last usb state was host disable bus
                    USB_HOST_BusDisable(0);
                    usbData.state = USB_STATE_WAIT_FOR_BUS_DISABLE_COMPLETE;
                    
                    usbData.currentRole = USB_DEVICE ;
                    break;
                    
                case 3: //11 ? Attached to an accessory - NOT SUPPORTED
                    USER_CONSOLE_PRINT("\r\nAttached to an accessory - NOT SUPPORTED\r\n");

                    if(usbData.currentRole == USB_DEVICE)
                    {
                        if(USB_DEVICE_StateGet(usbData.usbDevHandle)== USB_DEVICE_STATE_ATTACHED)
                        {
                            USB_DEVICE_Detach(usbData.usbDevHandle);
                        }
                    }
                    else if(usbData.currentRole == USB_HOST)
                    {
                        USB_HOST_BusDisable(0);
                        while(USB_HOST_RESULT_TRUE != USB_HOST_BusIsDisabled(0));
                    }

                    usbData.currentRole = USB_NONE;
                    usbData.state = USB_STATE_IDLE;
                    break;
            }
            break;
            
        //SWITCH TO HOST    
        case USB_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
            if( USB_HOST_RESULT_TRUE == USB_HOST_BusIsEnabled(0) )
            {                
                usbData.state = USB_STATE_WAIT_FOR_DEVICE_ATTACH;
            }
            break;
            
        //SWITCH TO DEVICE
        case USB_STATE_WAIT_FOR_BUS_DISABLE_COMPLETE:
            
            if(USB_HOST_RESULT_TRUE == USB_HOST_BusIsDisabled(0))
            {
                USB_DEVICE_Attach(usbData.usbDevHandle);
                usbData.state = USB_STATE_WAIT_FOR_CONFIGURATION;
            }
            
            break;        
            
        /*********  USB AS HOST  **********/    
            
        case USB_STATE_WAIT_FOR_DEVICE_ATTACH:
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            break;

        case USB_STATE_DEVICE_ATTACHED:
            /* Wait for device report */
            USER_CONSOLE_PRINT("---Keyboard Connected---\r\n");
            usbData.state = USB_STATE_IDLE;
            break;
        case USB_STATE_DEVICE_DETACHED:
            /* Wait for device report */
            USER_CONSOLE_PRINT("---Keyboard Disconnected---\r\n");
            SetGetStatusState();
            break;       
            
            
        case USB_STATE_HOST_MAIN_TASK:
            StateAsHostMainTask();
            usbData.state = USB_STATE_IDLE;
            break;            
            
        /*********  USB AS DEVICE  **********/
            
        case USB_STATE_WAIT_FOR_POWER_DETECT :
            
            if ( usbData.powerDetected == true )
            {
                /* VBUS was detected. We can attach the device */
                USB_DEVICE_Attach(usbData.usbDevHandle);
                usbData.state = USB_STATE_WAIT_FOR_CONFIGURATION;
            }
            break;    
            
        case USB_STATE_WAIT_FOR_CONFIGURATION:

            if(usbData.deviceConfigured == true)
            {
                /* Device is ready to run the main task */
                usbData.hidDataReceived = false;
                usbData.hidDataTransmitted = true;
                usbData.state = USB_STATE_DEVICE_MAIN_TASK;
                        
                /* Place a new read request. */
                USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
                        &usbData.rxTransferHandle, usbData.receiveDataBuffer, 64);
            }
            break;

        case USB_STATE_DEVICE_MAIN_TASK:
            StateAsDeviceMainTask();
            if(usbData.tusb320Interrupt)
            {
                vTaskDelay(100 / portTICK_PERIOD_MS);

                SetGetStatusState();
                usbData.tusb320Interrupt = false;
            }
            break;
            
        /***************************************************************/
        case USB_STATE_IDLE://wait for a state change
            if(usbData.tusb320Interrupt)
            {
                vTaskDelay(100 / portTICK_PERIOD_MS);

                SetGetStatusState();
                usbData.tusb320Interrupt = false;
            }
            break;
            
        case USB_STATE_ERROR:
            break;
        default:
            break;
    }
}
 

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions                                               
// *****************************************************************************
// *****************************************************************************
void USB_PrintableKeyPressCallbackRegister(PrintableKeyboardKeyPressEventHandler keyPressEventHandler)
{
    usbData.onPrintableKeyPressEventHandler = keyPressEventHandler;
}
void USB_ControlKeyPressCallbackRegister(ControlKeyboardKeyPressEventHandler keyPressEventHandler)
{
    usbData.onControlKeyPressEventHandler = keyPressEventHandler;
}
/*******************************************************************************
 End of File
 */

