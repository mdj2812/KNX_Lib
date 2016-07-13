/**
  ******************************************************************************
  * @file       KNX_Ph.h
  * @author     MA Dingjie
  * @version    V0.0.1
  * @date       13-July-2016
  * @brief      This file contains KNX physical layer common defines,
  *             enumerations, macros, structures definitions and all the
  *             functions prototypes.
  ******************************************************************************
  */

#ifndef __KNX_Ph
#define __KNX_Ph

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  PH Status structures definition
  */
typedef enum
{
  PH_NOINIT     = 0x00U,
  PH_RESET      = 0x01U,
  PH_NORMAL     = 0x02U,
  PH_MONITOR    = 0x03U,
  PH_STOP       = 0x04U
} PH_StatusTypeDef;

/**
  * @brief  RX Debug Status structures definition
  */
typedef enum
{
  RX_DEBUG_IDLE = 0x00U,
  RX_DEBUG_KNX  = 0x01U,
  RX_DEBUG_OTHER= 0x02U
} RX_DEBUG_StatusTypeDef;

/* Exported constants --------------------------------------------------------*/
/** @defgroup PH_Error_Code Physical Layer Error Code
  * @brief    PH Error Code
  * @{
  */
#define PH_ERROR_NONE           ((uint32_t)0x00000000U)   /*!< No error       */
#define PH_ERROR_FORMAT         ((uint32_t)0x00000001U)   /*!< Format error   */
#define PH_ERROR_TIMEOUT        ((uint32_t)0x00000002U)   /*!< Timeout error  */
#define PH_ERROR_INIT           ((uint32_t)0x00000004U)   /*!< Initial error  */
/**
  * @}
  */

/** @defgroup PH_Debug_Error_Code Physical Layer Debug Error Code
  * @brief    PH_Debug Error Code
  * @{
  */
#define PH_Debug_ERROR_NONE     ((uint32_t)0x00000000U)   /*!< No error       */
#define PH_Debug_ERROR_TIMEOUT  ((uint32_t)0x00000002U)   /*!< Timeout error  */
#define PH_Debug_ERROR_INIT     ((uint32_t)0x00000004U)   /*!< Initial error  */
/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
uint32_t KNX_Ph_Init(void);
uint32_t KNX_Ph_Send(uint8_t data);
uint32_t KNX_Ph_Receive(uint8_t *data);

/* Auxiliary functions  *******************************************************/
uint32_t int2text(uint8_t value, char *msg);
uint32_t text2int(char *msg, uint8_t *value);

/* Peripheral State functions  ************************************************/
PH_StatusTypeDef KNX_Ph_GetState(void);

#ifdef __cplusplus
}
#endif

#endif /* __KNX_Ph */
