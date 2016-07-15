/**
  ******************************************************************************
  * @file       KNX_Ph.h
  * @author     MA Dingjie
  * @version    V0.0.2
  * @date       15-July-2016
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
#include "debug.h"

/** @addtogroup KNX_PH
  * @{
  */

/** @addtogroup KNX_PH_Sup
  * @{
  */

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

/* Exported types ------------------------------------------------------------*/
/** @defgroup KNX_PH_Sup_Exported_Types KNX_PH_Sup Exported Types
  * @brief    PH Status Enumeration
  * @{
  */

/**
  * @brief  PH Status Enumeration definition. State machine:
  *         \image html KNX_Ph_state_machine.jpg
  */
typedef enum
{
  PH_NOINIT     = 0x00U,        /*!< Not Initialized State                    */
  PH_RESET      = 0x01U,        /*!< Reset State                              */
  PH_NORMAL     = 0x02U,        /*!< Normal Mode                              */
  PH_MONITOR    = 0x03U,        /*!< Monitor Mode                             */
  PH_STOP       = 0x04U         /*!< Stop Mode                                */
} PH_Status_t;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup KNX_PH_Sup_Exported_Functions
  * @{
  */

/** @addtogroup KNX_PH_Sup_Exported_Functions_Group1
  * @{
  */

/* Initialization functions  **************************************************/
uint32_t KNX_Ph_Init(void);
/**
  * @}
  */

/** @addtogroup KNX_PH_Sup_Exported_Functions_Group2
* @{
*/

/* Send/Receive functions  ***************************************************/
uint32_t KNX_Ph_Send(uint8_t data);
uint32_t KNX_Ph_Receive(uint8_t *data);
/**
  * @}
  */

/** @addtogroup KNX_PH_Sup_Exported_Functions_Group3
  * @{
  */

/* State functions  **********************************************************/
PH_Status_t KNX_Ph_GetState(void);
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

#endif /* __KNX_Ph */
