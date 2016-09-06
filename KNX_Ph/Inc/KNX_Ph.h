/**
  ******************************************************************************
  * @file       KNX_Ph.h
  * @author     MA Dingjie
  * @version    V1.0.0
  * @date       6-September-2016
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
/** @defgroup PH_Exported_Constants Physical Layer Exported Constants
  * @brief    PH Exported Constants
  * @{
  */

/** @defgroup PH_Error_Code Physical Layer Error Code
  * @brief    PH Error Code
  * @{
  */
#define PH_ERROR_NONE           ((uint8_t)0x00U)   /*!< No error              */
#define PH_ERROR_INIT           ((uint8_t)0x01U)   /*!< Initialization error  */
#define PH_ERROR_TIMEOUT        ((uint8_t)0x02U)   /*!< Timeout error         */
#define PH_ERROR_REQUEST        ((uint8_t)0x03U)   /*!< Invalid request error */
#define PH_ERROR_RESPONSE       ((uint8_t)0x04U)   /*!< No response error     */
#define PH_ERROR_STATE          ((uint8_t)0x05U)   /*!< State error           */
/**
  * @}
  */

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
  * @brief  PH Request Enumeration definition.
  */
typedef enum
{
  Ph_Reset              = 0x00U,        /*!< Reset Request                    */
  Ph_State              = 0x01U,        /*!< State Request                    */
  Ph_ActivateBusmon     = 0x02U,        /*!< Activate the Busmonitormode      */
  Ph_ProductID          = 0x03U,        /*!< Product ID Request               */
  Ph_ActivateBusyMode   = 0x04U,        /*!< Activate the Busy Mode           */
  Ph_ResetBusyMode      = 0x05U,        /*!< Deactivate the Busy Mode         */
  Ph_SetAddress         = 0x06U,        /*!< Set Address Request              */
  Ph_AckInformation     = 0x07U,        /*!< Acknowledgement Information Request:
                                              Nack, Busy, Addressed           */
  Ph_None               = 0xffU         /*!< None request                     */
} PH_Request_t;
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
uint8_t KNX_Ph_Init(void);
/**
  * @}
  */

/** @addtogroup KNX_PH_Sup_Exported_Functions_Group2
* @{
*/

/* Send/Receive functions  ***************************************************/
uint8_t KNX_Ph_Send(PH_Request_t request, uint32_t timeout);
uint8_t KNX_Ph_WaitFor(uint8_t res, uint32_t timeout);
uint8_t KNX_Ph_WaitForWithMask(uint8_t *res, uint8_t resMask, uint32_t timeout);
/**
  * @}
  */

/** @addtogroup KNX_PH_Sup_Exported_Functions_Group3
* @{
*/

/* Services functions  ********************************************************/
uint8_t KNX_Ph_Reset_request(void);
uint8_t KNX_Ph_State_request(void);
/**
  * @}
  */

/** @addtogroup KNX_PH_Sup_Exported_Functions_Group4
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
