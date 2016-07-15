/*******************************************************************************
  * @file       debug_uart 
  * @author     
  * @version    V3.0.0 
  * @date       14-July-2016
  * @brief      Functions perform the debug uart services.
  ******************************************************************************
  */

#ifndef debug_UART_H
#define debug_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup Cola_Debug
  * @{
  */

/** @addtogroup Debug_Uart
  * @{
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Debug_Uart_Exported_Functions
  * @{
  */

/** @addtogroup Debug_Uart_Exported_Functions_Group1
  * @{
  */

/* Initialization functions ***************************************************/
void debug_uart_init(void);
/**
  * @}
  */

/** @addtogroup Debug_Uart_Exported_Functions_Group2
  * @{
  */

/* UART utilities functions ***************************************************/
void debug_uart_send (uint8_t *data, uint16_t size);
void debug_uart_isr(void);
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

#ifdef __cplusplus
}
#endif

#endif /* debug_UART_H */

