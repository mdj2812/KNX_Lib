/**
  ******************************************************************************
  * @file       KNX_Aux.h
  * @author     MA Dingjie
  * @version    V0.0.4
  * @date       19-July-2016
  * @brief      This file contains auxiliary functions for the project.
  ******************************************************************************
  */

#ifndef __KNX_Ph
#define __KNX_Ph

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup KNX_PH
  * @{
  */

/** @addtogroup KNX_PH_Aux
  * @{
  */
	
/* Exported constants --------------------------------------------------------*/
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

/* Exported functions --------------------------------------------------------*/
/** @addtogroup KNX_Aux_Exported_Functions
  * @{
  */
uint8_t int2text(uint8_t value, char *msg);
uint8_t text2int(char *msg, uint8_t *value);
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
