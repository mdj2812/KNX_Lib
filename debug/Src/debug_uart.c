/*******************************************************************************
  * @file       debug_uart 
  * @author     
  * @version    V3.0.0 
  * @date       14-July-2016
  * @brief      Debug UART adaptation module
  *             This file provides functions to manage following functionalities:
  *              + Initialization ports to adapt to the board
  *              + Perform UART send functions
  *              + UART interrupt routine functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "debug_uart.h"

/** @addtogroup Cola_Debug
  * @{
  */

/** @defgroup Debug_Uart Debug UART
  * @{
  */

/* External variables --------------------------------------------------------*/
/** @defgroup Debug_UART_External_Variables Debug UART External Variables
  * @{
  */
/** \brief UART Handler */
extern UART_HandleTypeDef huartx;
/**
  * @}
  */

/* External functions --------------------------------------------------------*/
/** @defgroup Debug_UART_External_Functions Debug UART External Functions
  * @brief      External functions to be used in \ref Debug module
  * @{
  */
extern void debug_uart_isr_rx (void);
extern void debug_uart_isr_tx (void);
extern void debug_uart_isr_begin (void);
extern void debug_uart_isr_end (void);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup Debug_Uart_Exported_Functions Debug UART Exported Functions
  * @{
  */

/** @defgroup Debug_Uart_Exported_Functions_Group1 Debug UART Initi Functions
  * @{
  */

/**
  * @brief      Initialization the uart handler ::huartx. 
  */
void debug_uart_init(void) {
  huartx.Instance = USART2;
  huartx.Init.BaudRate = 9600;
  huartx.Init.WordLength = UART_WORDLENGTH_8B;
  huartx.Init.StopBits = UART_STOPBITS_1;
  huartx.Init.Parity = UART_PARITY_NONE;
  huartx.Init.Mode = UART_MODE_TX_RX;
  huartx.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huartx.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huartx) != HAL_OK)
  {
    puts("***ERROR*** uart failed to initiate \r\n");
  }
}
/**
  * @}
  */

/** @defgroup Debug_Uart_Exported_Functions_Group2 Debug UART utilities functions
  * @{
  */

/**
  * @brief      Send the data through UART.
  * @param      data:  pointer to the data buffer.
  * @param      size:  lenghth of the buffer.
  */
void debug_uart_send (uint8_t *data, uint16_t size) {
  /* Process Locked */
  (&huartx)->pTxBuffPtr = data;
  (&huartx)->TxXferSize = size;
  (&huartx)->TxXferCount = size;

  (&huartx)->ErrorCode = HAL_UART_ERROR_NONE;
  (&huartx)->gState = HAL_UART_STATE_BUSY_TX;
  
  /* Enable the UART Transmit data register empty Interrupt */
  SET_BIT((&huartx)->Instance->CR1, USART_CR1_TXEIE);
}

/**
  * @brief      UART interrupt routines.
  */
void debug_uart_isr(void)
{  
  debug_uart_isr_begin ();
    
  /* UART in mode Receiver -------------------------------------------------*/
  if(HAL_UART_GetState(&huartx) == HAL_UART_STATE_BUSY_RX)
  {
    debug_uart_isr_rx();
  }
  
  /* UART in mode Transmitter ------------------------------------------------*/
  if(HAL_UART_GetState(&huartx) == HAL_UART_STATE_BUSY_TX)
  {
    debug_uart_isr_tx();
  }
  
  debug_uart_isr_end();
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
