/**
  ******************************************************************************
  * @file       KNX_Aux.h
  * @author     MA Dingjie
  * @version    V0.0.3
  * @date       18-July-2016
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
