/**
  *****************************************************************************
  * @file       debug_uart.c 
  * @author     
  * @version    V3.1.1 
  * @date       29-July-2016
  * @brief      Debug UART adaptation module
  *             This file provides functions to manage following functionalities:
  *              + Initialization ports to adapt to the board
  *              + Perform UART send/receive functions
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

/* Private variables --------------------------------------------------------*/
/** @defgroup Debug_UART_External_Variables Debug UART External Variables
  * @{
  */
/** \brief UART Handler */
static UART_HandleTypeDef debug_huart;
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
  * @brief      Initialization of the uart handler ::debug_huart. 
  * @retval     1 for success, 0 for fail. 
  */
uint8_t debug_uart_init(void)
{
  debug_huart.Instance = USART2;
  debug_huart.Init.BaudRate = 9600;
  debug_huart.Init.WordLength = UART_WORDLENGTH_8B;
  debug_huart.Init.StopBits = UART_STOPBITS_1;
  debug_huart.Init.Parity = UART_PARITY_NONE;
  debug_huart.Init.Mode = UART_MODE_TX_RX;
  debug_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  debug_huart.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&debug_huart) != HAL_OK)
  {
    puts("***ERROR*** debug_uart failed to initiate \r\n");
    return 0;
  }
  
  __HAL_UART_ENABLE_IT(&debug_huart, UART_IT_RXNE);/** Activate Flag Receptie */
  __HAL_UART_ENABLE_IT(&debug_huart, UART_IT_TC);  /** Activate Flag TX       */
  
  return 1;
}
/**
  * @}
  */

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
Debug_Uart_Status_t debug_uart_send (uint8_t *data, uint16_t size)
{
  if((data == NULL ) || (size == 0U)) 
  {
    return Debug_Uart_ERROR;
  }
  
  /* Process Locked */
  if((&debug_huart)->Lock == HAL_LOCKED)
  {
    return Debug_Uart_BUSY;
  }
  else
  {
    (&debug_huart)->Lock = HAL_LOCKED;
  }
  
  (&debug_huart)->pTxBuffPtr = data;
  (&debug_huart)->TxXferSize = size;
  (&debug_huart)->TxXferCount = size;

  (&debug_huart)->ErrorCode = HAL_UART_ERROR_NONE;
  (&debug_huart)->gState = HAL_UART_STATE_BUSY_TX;
  
  /* Process Unlocked */
  (&debug_huart)->Lock = HAL_UNLOCKED;

  /* Enable the UART Transmit data register empty Interrupt */
  SET_BIT((&debug_huart)->Instance->CR1, USART_CR1_TXEIE);

  return Debug_Uart_OK;
}

/**
  * @brief      Receive the data through UART.
  * @param      data:  pointer to the data buffer.
  * @param      size:  lenghth of the buffer.
  */
Debug_Uart_Status_t debug_uart_receive (uint8_t *data, uint16_t size)
{
  /* Check that a Rx process is not already ongoing */ 
  if((&debug_huart)->RxState == HAL_UART_STATE_READY)
  {
    if((data == NULL ) || (size == 0U)) 
    {
      return Debug_Uart_ERROR;
    }
    
    /* Process Locked */
    if((&debug_huart)->Lock == HAL_LOCKED)
    {
      return Debug_Uart_BUSY;
    }
    else
    {
      (&debug_huart)->Lock = HAL_LOCKED;
    }
    
    (&debug_huart)->pRxBuffPtr = data;
    (&debug_huart)->RxXferSize = size;
    (&debug_huart)->RxXferCount = size;

    (&debug_huart)->ErrorCode = HAL_UART_ERROR_NONE;
    (&debug_huart)->RxState = HAL_UART_STATE_BUSY_RX;
    
    /* Process Unlocked */
    (&debug_huart)->Lock = HAL_UNLOCKED;
  
    /* Enable the UART Parity Error Interrupt */
    SET_BIT((&debug_huart)->Instance->CR1, USART_CR1_PEIE);

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT((&debug_huart)->Instance->CR3, USART_CR3_EIE);

    /* Enable the UART Data Register not empty Interrupt */
    SET_BIT((&debug_huart)->Instance->CR1, USART_CR1_RXNEIE);
    
    return Debug_Uart_OK;
  }
  else
  {
    return Debug_Uart_BUSY; 
  }
}

/**
  * @brief      UART interrupt routines.
  */
void debug_uart_isr(void)
{  
  /* UART IRQ Handler function provided by driver. */
  HAL_UART_IRQHandler(&debug_huart);
  
  debug_uart_isr_begin ();
    
  /* UART in mode Receiver ---------------------------------------------------*/
  if((HAL_UART_GetState(&debug_huart) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX)
  {
    debug_uart_isr_rx();
  }
  
  /* UART in mode Transmitter ------------------------------------------------*/
  if((HAL_UART_GetState(&debug_huart) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX)
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
