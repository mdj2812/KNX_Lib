/**
  ******************************************************************************
  * @file       KNX_Ph.c
  * @author     MA Dingjie
  * @version    V0.0.1
  * @date       13-July-2016
  * @brief      KNX Physical Layer communication via TP-UART.
  *             This file provides functions to manage following functionalities:
  *              + Initialization functions
  *              + Send and Receive messages functions
  *              + Auxiliary functions to convert messages to binary format or
  *                ASCII-hex format
  *              + State functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "KNX_Ph.h"
#include "stm32f4xx_hal.h"

/* Private variables ---------------------------------------------------------*/
static char KNX_PH_STATE_DEBUGMSG[] = "KNX_PH_STATE changed to XX\n";

/* Private function prototypes -----------------------------------------------*/
static uint32_t KNX_Ph_Debug_Send(uint8_tdata);
static void KNX_Ph_SetState(PH_StatusTypeDef state);
