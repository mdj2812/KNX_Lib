/**
  ******************************************************************************
  * @file       KNX_Aux.c
  * @author     MA Dingjie
  * @version    V0.0.4
  * @date       19-July-2016
  * @brief      Auxiliary functions for KNX Physical Layer.
  *             This file provides functions to manage following functionalities:
  *              + Conversion functions from int to text and from text to int
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "KNX_Aux.h"

/** @addtogroup KNX_PH KNX Physical Layer
  * @{
  */

/** @defgroup KNX_Aux KNX Physical Layer Auxiliary
  * @{
  */
	
/* Private constants ---------------------------------------------------------*/
/** @defgroup KNX_Aux_Private_Consts Auxiliary Private constants
  * @{
  */
/** \brief Table of hexadecimal numbers */
static const char hex_num[] = {'0', '1', '2', '3', '4', '5', '6', '7' , '8', '9',
                                'A', 'B', 'C', 'D', 'E', 'F'};
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup KNX_Aux_Exported_Functions KNX Physical Layer Auxiliary Exported Functions
  * @{
  */

/**
 *  @brief      Convert an uint8_t value into a char message
 *  @param      value: an 8 bits unsigned value.
 *  @param      msg: pointer to a buffer.
 *  @return     Error code, see \ref KNX_Aux_Error_Code
 */
uint8_t int2text(uint8_t value, char *msg)
{
  if(msg == NULL)
  {
    return AUX_ERROR_MSG;
  }
		
  *(msg+1) = hex_num[value%16]; /* get the value on digit 0 */
  *msg = hex_num[value/16];     /* get the value on digit 1 */
  
  return AUX_ERROR_NONE;
}

/**
 *  @brief      Convert a char message into an uint8_t value 
 *  @param      msg: pointer to a buffer of 2 digits.
 *  @param      value: pointer to ouput as an 8 bits unsigned value.
 *  @return     Error code, see \ref KNX_Aux_Error_Code
 */
uint8_t text2int(char *msg, uint8_t *value)
{
  uint8_t i;	/* indice */
  uint8_t value_0, value_1;	/* values on 2 digits */
	
  for(i=0; i<16; i++)
  {
    if(*msg == hex_num[i])
    {
      value_1 = i;
      break;
    }
    
    if(i == 15)                 /* didn't found the corresponding digit */
    {
      return AUX_ERROR_MSG;
    }
  }
  
  for(i=0; i<16; i++)
  {
    if(*(msg+1) == hex_num[i])
    {
      value_0 = i;
      break;
    }
    
    if(i == 15)                 /* didn't found the corresponding digit */
    {
      return AUX_ERROR_MSG;
    }
  }
  
  *value = value_0 + value_1*16;/* output value = d0*16^0+d1*16^1 */
  
  return AUX_ERROR_NONE;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
