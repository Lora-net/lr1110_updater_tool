
# LR1110 Updater tool

## Introduction

This project gives an implementation example of the flash memory update mechanism available in the LR1110 radio.

The project is developed for the following configuration:

-   NUCLEO-L476RG development board
-   LR1110 shield
-   Touchscreen (optional)

Nevertheless, the function `lr1110_update_firmware` can be reused to port this feature on any platform.

More information and pre-compiled binaries are available [the Wiki](https://github.com/Lora-net/lr1110_updater_tool/wiki/home)

## Compilation

The user can chose the firmware to be uploaded thanks to this tool by defining the header file to be included in main.c file before launching the compilation.

For instance:

-   `#include "lr1110_trx_0303.h"` for transceiver firmware 0x0303
-   `#include "lr1110_modem_1.0.7.h"` for modem firmware v1.0.7

Firmware images can be downloaded from the [**dedicated Github repository**](https://github.com/Lora-net/radio_firmware_images/tree/master/lr1110)

There are 2 ways to get the binary file:

-   a makefile
-   a Keil project (available in keil folder )

## Operations

To use this tool, the user can download the generated binary file through :

-   The drag-and-drop flash programming (if enabled)
-   A tool like STM32 ST-Link Utility

As soon as the binary is downloaded, the update process begins.

The user can get information about the update status through the following interfaces:

-   LEDs: an orange LED is on during the update and a green LED indicates that the update is successful (or a red LED if something went wrong)
-   COM port: if there is a terminal connected to the COM port exposed by the NUCLEO board, information can be read (bitrate set to 921600 bps)
-   Touchscreen (if connected): the status is displayed on the screen
