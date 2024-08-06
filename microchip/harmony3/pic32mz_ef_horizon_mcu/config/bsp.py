"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""

def instantiateComponent(bspComponent):
    #MAIN
    Database.setSymbolValue("core", "BSP_PIN_1_FUNCTION_NAME", "SD_CARD_DETECT")
    Database.setSymbolValue("core", "BSP_PIN_1_FUNCTION_TYPE", "GPIO")
    Database.setSymbolValue("core", "BSP_PIN_1_MODE", "DIGITAL")
    Database.setSymbolValue("core", "BSP_PIN_1_DIR", "In")

    Database.setSymbolValue("core", "BSP_PIN_23_FUNCTION_NAME", "USB_UART_TXD_U6RX")
    Database.setSymbolValue("core", "BSP_PIN_23_FUNCTION_TYPE", "U6RX")
    
    Database.setSymbolValue("core", "BSP_PIN_36_FUNCTION_NAME", "USB_UART_RXD_U6TX")
    Database.setSymbolValue("core", "BSP_PIN_36_FUNCTION_TYPE", "U6TX")

    Database.setSymbolValue("core", "BSP_PIN_46_FUNCTION_NAME", "PWRGD")
    Database.setSymbolValue("core", "BSP_PIN_46_FUNCTION_TYPE", "GPIO")
    Database.setSymbolValue("core", "BSP_PIN_46_MODE", "DIGITAL")
    Database.setSymbolValue("core", "BSP_PIN_46_DIR", "In")

    Database.setSymbolValue("core", "BSP_PIN_57_FUNCTION_NAME", "SDCARD_CLK_SCK5")
    Database.setSymbolValue("core", "BSP_PIN_57_FUNCTION_TYPE", "SCK5")

    Database.setSymbolValue("core", "BSP_PIN_59_FUNCTION_NAME", "USB_UART_SUSPRND")
    Database.setSymbolValue("core", "BSP_PIN_59_FUNCTION_TYPE", "GPIO")
    Database.setSymbolValue("core", "BSP_PIN_59_MODE", "DIGITAL")
    Database.setSymbolValue("core", "BSP_PIN_59_DIR", "Out")

    Database.setSymbolValue("core", "BSP_PIN_60_FUNCTION_NAME", "USB_UART_WAKEUP")
    Database.setSymbolValue("core", "BSP_PIN_60_FUNCTION_TYPE", "GPIO")
    Database.setSymbolValue("core", "BSP_PIN_60_MODE", "DIGITAL")
    Database.setSymbolValue("core", "BSP_PIN_60_DIR", "Out")

    Database.setSymbolValue("core", "BSP_PIN_82_FUNCTION_NAME", "USB_5V_OUT")
    Database.setSymbolValue("core", "BSP_PIN_82_FUNCTION_TYPE", "GPIO")
    Database.setSymbolValue("core", "BSP_PIN_82_MODE", "DIGITAL")
    Database.setSymbolValue("core", "BSP_PIN_82_DIR", "Out")
    Database.setSymbolValue("core", "BSP_PIN_82_PD", "True")

    Database.setSymbolValue("core", "BSP_PIN_90_FUNCTION_NAME", "SDCARD_DO_SDI5")
    Database.setSymbolValue("core", "BSP_PIN_90_FUNCTION_TYPE", "SDI5")

    Database.setSymbolValue("core", "BSP_PIN_91_FUNCTION_NAME", "SDCARD_DI_SDO5")
    Database.setSymbolValue("core", "BSP_PIN_91_FUNCTION_TYPE", "SDO5")

    Database.setSymbolValue("core", "BSP_PIN_97_FUNCTION_NAME", "GPIO_RD9_SD_CS")
    Database.setSymbolValue("core", "BSP_PIN_97_FUNCTION_TYPE", "GPIO")
    Database.setSymbolValue("core", "BSP_PIN_97_MODE", "DIGITAL")
    Database.setSymbolValue("core", "BSP_PIN_97_DIR", "Out")

    Database.setSymbolValue("core", "BSP_PIN_119_FUNCTION_NAME", "FLASH_CS_SQIC1")
    Database.setSymbolValue("core", "BSP_PIN_119_FUNCTION_TYPE", "SQIC1")

    Database.setSymbolValue("core", "BSP_PIN_129_FUNCTION_NAME", "FLASH_SCK_SQICLK")
    Database.setSymbolValue("core", "BSP_PIN_129_FUNCTION_TYPE", "SQICLK")

    Database.setSymbolValue("core", "BSP_PIN_130_FUNCTION_NAME", "FLASH_DATA3_SQID3")
    Database.setSymbolValue("core", "BSP_PIN_130_FUNCTION_TYPE", "SQID3")

    Database.setSymbolValue("core", "BSP_PIN_139_FUNCTION_NAME", "FLASH_DATA2_SQID2")
    Database.setSymbolValue("core", "BSP_PIN_139_FUNCTION_TYPE", "SQID2")

    Database.setSymbolValue("core", "BSP_PIN_140_FUNCTION_NAME", "FLASH_DATA1_SQID1")
    Database.setSymbolValue("core", "BSP_PIN_140_FUNCTION_TYPE", "SQID1")

    Database.setSymbolValue("core", "BSP_PIN_141_FUNCTION_NAME", "FLASH_DATA0_SQID10")
    Database.setSymbolValue("core", "BSP_PIN_141_FUNCTION_TYPE", "SQID0")

    # DEVCFG0<ICESEL> In-Circuit Emulator/Debugger Communication Channel Select bits
    Database.setSymbolValue("core", "CONFIG_ICESEL", "ICS_PGx2")

    BSP_NAME = "pic32mz_ef_horizon_main"

    pinAttributes = [{"attrib":"type", "symbol":"BSP_CUSTOM_TYPE", "label":"Type Name"},
        {"attrib":"mode", "symbol":"BSP_CUSTOM_MODE", "label":"Mode"},
        {"attrib":"dir", "symbol":"BSP_CUSTOM_DIR", "label":"Direction"},
        {"attrib":"lat", "symbol":"BSP_CUSTOM_LAT", "label":"Initial Latch Value"},
        {"attrib":"od", "symbol":"BSP_CUSTOM_OD", "label":"Open Drain"},
        {"attrib":"cn", "symbol":"BSP_CUSTOM_CN", "label":"Change Notice"},
        {"attrib":"pu", "symbol":"BSP_CUSTOM_PU", "label":"Pull Up"},
        {"attrib":"pd", "symbol":"BSP_CUSTOM_PD", "label":"Pull Down"}]
    
    pinTypes = []

    execfile(Variables.get("__BSP_DIR") + "/boards/config/bsp_common.py")
