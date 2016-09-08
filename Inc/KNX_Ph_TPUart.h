/**
  ******************************************************************************
  * @file       KNX_PH_TPUart.h
  * @author     MA Dingjie
  * @version    V1.0.0
  * @date       6-September-2016
  * @brief      This file contains definitions and prototypes of functions for
  *             TP-UART.
  ******************************************************************************
  */

#ifndef __KNX_Ph_TPUART
#define __KNX_Ph_TPUART

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup KNX_PH
  * @{
  */

/** @addtogroup KNX_PH_TPUart
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup KNX_TPUart_Exported_Types KNX TPUart Exported Types
  * @brief    TPUart Status Enumeration
  * @{
  */

/** 
  * @brief  TPUart Status structures definition  
  */  
typedef enum 
{
  TPUart_OK       = 0x00U,      /*!< OK                                       */
  TPUart_ERROR    = 0x01U,      /*!< Error                                    */
  TPUart_BUSY     = 0x02U,      /*!< Busy                                     */
} TPUart_Status_t;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup KNX_PH_TPUart_Exported_Functions
  * @{
  */

/** @addtogroup KNX_PH_TPUart_Exported_Functions_Group1
  * @{
  */

/* Initialization functions ***************************************************/
uint8_t KNX_PH_TPUart_init(void);
/**
  * @}
  */

/** @addtogroup KNX_PH_TPUart_Exported_Functions_Group2
  * @{
  */

/* Send/Receive functions  ***************************************************/
uint8_t KNX_PH_TPUart_Send(uint8_t *data, uint16_t size);
uint8_t KNX_PH_TPUart_Receive(uint8_t *data, uint16_t size);
/**
  * @}
  */

/** @addtogroup KNX_PH_TPUart_Exported_Functions_Group3
  * @{
  */
/* UART Interrupt function  ***************************************************/
void TPUart_isr(void);
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

#endif /* __KNX_Ph_TPUART */
