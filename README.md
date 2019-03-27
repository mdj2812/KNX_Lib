# KNX_Lib
KNX library

The purpose of this project is to build a library using FreeRTOS tasks and synchronization mechanisms to ease the development of embedded applications using the KNX communication protocol.
The library is based on the stm32f4discovery board plus a base board populated with a KNX UART.
The stm32f4discovery board is built around the STM32F407VGT6, a modern 32-bit ARM Cortex M4F microcontroller.
The program to develop on the stm32f4discovery must employ a modular and layered architecture; each layer of the KNX communications protocol stack (physical, link, network, application) must be developed as a separate, interchangeable library of one or more modules. All data exchange between such modules must employ FreeRTOS synchronization mechanisms in order to maximize module independence.
The development environment to use is the IAR Embedded Workbench for ARM and the STM32 CubeMX software to automatically generate code to configure and use the microcontroller hardware peripherals.
