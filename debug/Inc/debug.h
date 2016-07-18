/**
  *****************************************************************************
  * @file       debug.h
  * @author
  * @version    V3.0.1
  * @date       18-July-2016
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
void DebugGuardar1sTask(void * argument);
void DebugGuardar2sTask(void * argument);
/**
  * @}
  */

/** @addtogroup Debug_Exported_Functions_Group3
  * @{
  */

/* Systick test isr functions  ************************************************/
void debug_systick_isr_begin(void);
void debug_systick_isr_1s(void);
void debug_systick_isr_2s(void);
void debug_systick_isr_end(void);
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

