/**
  ******************************************************************************
  * @file       KNX_Aux.h
  * @author     MA Dingjie
  * @version    V0.0.5
  * @date       20-July-2016
  * @brief      This file contains auxiliary functions for the project.
  ******************************************************************************
  */

#ifndef __KNX_Aux
#define __KNX_Aux

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup KNX_PH
  * @{
  */

/** @addtogroup KNX_Aux
  * @{
  */
	
/* Exported constants --------------------------------------------------------*/
/** @defgroup KNX_Aux_Exported_Consts Auxiliary Exported Functions
  * @brief    Auxiliary Exported Functions
  * @{
  */

/** @defgroup KNX_Aux_Error_Code Auxiliary Functions' Error Code
  * @brief    Auxiliary Functions' Error Code
  * @{
  */
#define AUX_ERROR_NONE          ((uint8_t)0x00U)   /*!< No error              */
#define AUX_ERROR_BIN           ((uint8_t)0x01U)   /*!< Binary value error    */
#define AUX_ERROR_MSG           ((uint8_t)0x02U)   /*!< Message buffer error  */
/**
  * @}
  */
    
/** @defgroup KNX_Timeout Timeout Definition
  * @brief    Timeout Definition
  * @{
  */
#define KNX_DEFAULT_TIMEOUT     200                /*!< Default Timeout 200 ms*/
#define KNX_MAX_DELAY           0xFFFFFFFFU        /*!< Max Delay: Infinity   */
/**
  * @}
  */

/**
  * @}
  */
   
/* Exported types ------------------------------------------------------------*/
/** @defgroup KNX_Aux_Exported_Types Auxiliary Exported Types
  * @brief    Timer Status Enumeration
  * @{
  */

/**
  * @brief  Timer Status Enumeration definition.
  */
typedef enum
{
  TIMER_RESET           = 0x00U,        /*!< Reset State                      */
  TIMER_RUNNING         = 0x01U,        /*!< Timer is running                 */
  TIMER_PAUSE           = 0x02U,        /*!< Timer is paused                  */
  TIMER_TIMEOUT         = 0x03U         /*!< Timeout                          */
} TIMER_Status_t;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup KNX_Aux_Exported_Functions
  * @{
  */
    
/** @addtogroup KNX_Aux_Exported_Functions_Group1
  * @{
  */
uint8_t int2text(uint8_t value, unsigned char *msg);
uint8_t text2int(unsigned char *msg, uint8_t *value);
/**
  * @}
  */

/** @addtogroup KNX_Aux_Exported_Functions_Group2
  * @{
  */
void     KNX_InitTimer(void);
void     KNX_StartTimer(uint32_t Timeout);
uint32_t KNX_GetTime(void);
TIMER_Status_t KNX_GetTimerState(void);
uint32_t KNX_StopTimer(void);
uint32_t KNX_ResetTimer(void);
void     KNX_systick_isr(void);
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

#endif /* __KNX_Aux */
