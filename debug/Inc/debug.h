/**
  *****************************************************************************
  * @file       debug.h
  * @author
  * @version    V3.1.0
  * @date       26-July-2016
  * @brief      Debug tasks.
  ******************************************************************************
  */

#ifndef debug_H
#define debug_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cola.h"

/** @addtogroup Cola_Debug
  * @{
  */

/** @addtogroup Debug
  * @{
  */

/* Defines -------------------------------------------------------------------*/
/** @defgroup Debug_Private_Define Debug Private Define
  * @{
  */
/** \brief Max size of the ::buffer */
#define BUFFER_SIZE 1024
/** \brief Max size of the ::RX_buffer */
#define RX_BUFFER_SIZE 4

/**
  * @}
  */

/** @defgroup PH_Debug_Error_Code Physical Layer Debug Error Code
  * @brief    PH_Debug Error Code
  * @{
  */
#define PH_Debug_ERROR_NONE     ((uint32_t)0x00000000U)    /*!< No error       */
#define PH_Debug_ERROR_TIMEOUT  ((uint32_t)0x00000002U)    /*!< Timeout error  */
#define PH_Debug_ERROR_INIT     ((uint32_t)0x00000004U)    /*!< Initial error  */
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup Debug_Exported_Types Debug Exported Types
  * @{
  */

/**
  * @brief  RX Debug Status Enumeration definition
  */
typedef enum
{
  RX_DEBUG_IDLE = 0x00U,        /*!< Idle State                               */
  RX_DEBUG_KNX  = 0x01U,        /*!< KNX Debug Mode                           */
  RX_DEBUG_OTHER= 0x02U         /*!< Other Modes                              */
} RX_DEBUG_Status_t;

/**
  * @brief  RX Debug Message Type Enumeration definition
  */
typedef enum
{
  STATE_DEBUG   = 0x00U,        /*!< State Debug Message                      */
  SEND_DEBUG    = 0x01U,        /*!< Request Sending Debug Message            */
  RECEIVE_DEBUG = 0x02U,        /*!< Receiving Debug Message                  */
  ERROR_DEBUG   = 0x03U         /*!< Error Debug Message                      */
} DEBUG_Type_t;
/**
  * @}
  */

/* External variables --------------------------------------------------------*/
/** @defgroup Debug_External_Variables Debug External Variables
  * @{
  */
extern t_cola colaDebug;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Debug_Exported_Functions
  * @{
  */

/** @addtogroup Debug_Exported_Functions_Group1
  * @{
  */

/* Initialization functions ***************************************************/
uint32_t DebugInit(void);
/**
  * @}
  */

/** @addtogroup Debug_Exported_Functions_Group2
  * @{
  */

/* Debug tasks functions  *****************************************************/
void DebugTask(void * argument);
void DebugRXTask(void * argument);
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

#endif /* debug_H */

