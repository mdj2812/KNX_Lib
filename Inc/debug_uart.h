/**
  *****************************************************************************
  * @file       debug_uart.h 
  * @author     
  * @version    V3.1.0 
  * @date       26-July-2016
  * @brief      Functions perform the debug uart services.
  ******************************************************************************
  */

#ifndef debug_UART_H
#define debug_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"

/** @addtogroup KNX_Lib
  * @{
  */

   /** @addtogroup Cola_Debug
  * @{
  */

/** @addtogroup Debug_Uart
  * @{
  */
   
/* Exported types ------------------------------------------------------------*/
/** @defgroup Debug_Uart_Exported_Types Debug Uart Exported Types
  * @brief    Debug Uart Status Enumeration
  * @{
  */

/** 
  * @brief  Debug Uart Status structures definition  
  */  
typedef enum 
{
  Debug_Uart_OK       = 0x00U,  /*!< OK                                       */
  Debug_Uart_ERROR    = 0x01U,  /*!< Error                                    */
  Debug_Uart_BUSY     = 0x02U,  /*!< Busy                                     */
} Debug_Uart_Status_t;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Debug_Uart_Exported_Functions
  * @{
  */

/** @addtogroup Debug_Uart_Exported_Functions_Group1
  * @{
  */

/* Initialization functions ***************************************************/
uint8_t debug_uart_init(void);
/**
  * @}
  */

/** @addtogroup Debug_Uart_Exported_Functions_Group2
  * @{
  */

/* UART utilities functions ***************************************************/
Debug_Uart_Status_t debug_uart_send (uint8_t *data, uint16_t size);
Debug_Uart_Status_t debug_uart_receive (uint8_t *data, uint16_t size);
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

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* debug_UART_H */

