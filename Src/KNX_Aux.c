/**
  ******************************************************************************
  * @file       KNX_Aux.c
  * @author     MA Dingjie
  * @version    V1.0.0
  * @date       6-September-2016
  * @brief      Auxiliary functions for KNX Library.
  *             This file provides functions to manage following functionalities:
  *              + Conversion functions from int to text and from text to int
  *              + A basic timer
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "KNX_Aux.h"
#include "KNX_def.h"
#include "debug.h"
#include "stm32f4xx_hal.h"

/** @addtogroup KNX_Lib
  * @{
  */

/** @defgroup KNX_Aux KNX Auxiliary
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

/* Private variables ---------------------------------------------------------*/
/** @defgroup KNX_Aux_Private_Variables Auxiliary Private Variables
  * @{
  */
/** \brief Status of the timer */
volatile TIMER_Status_t timer_state;
/** \brief Counter of the timer, the unit is ms */
volatile uint32_t timer_tick;

/** \brief Aux Error message. */
static unsigned char Aux_Err_Msg[] = "[Aux]Error Code: XX\r\n";
/** \brief ::Aux_Err_Msg digits indice. */
#define AUX_ERROR_MSG_INDICE ((uint8_t)17)
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup KNX_Aux_Exported_Functions Auxiliary Exported Functions
  * @{
  */

/** @defgroup KNX_Aux_Exported_Functions_Group1 KNX int/text conversion functions
  * @{
  */

/**
 *  @brief      Convert an \c uint8_t value into a \c char message
 *  @param      value: an 8 bits unsigned value.
 *  @param      msg: pointer to a buffer.
 *  @return     Error code, see \ref KNX_Aux_Error_Code
 */
uint8_t int2text(uint8_t value, unsigned char *msg)
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
 *  @brief      Convert a \c char message into an \c uint8_t value 
 *  @param      msg: pointer to a buffer of 2 digits.
 *  @param      value: pointer to ouput as an 8 bits unsigned value.
 *  @return     Error code, see \ref KNX_Aux_Error_Code
 */
uint8_t text2int(unsigned char *msg, uint8_t *value)
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
      int2text(AUX_ERROR_BIN, &Aux_Err_Msg[AUX_ERROR_MSG_INDICE]);
      cola_guardar(&colaDebug, Aux_Err_Msg);

      return AUX_ERROR_BIN;
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
      int2text(AUX_ERROR_BIN, &Aux_Err_Msg[AUX_ERROR_MSG_INDICE]);
      cola_guardar(&colaDebug, Aux_Err_Msg);
      
      return AUX_ERROR_BIN;
    }
  }
  
  *value = value_0 + value_1*16;/* output value = d0*16^0+d1*16^1 */
  
  return AUX_ERROR_NONE;
}
/**
  * @}
  */

/** @defgroup KNX_Aux_Exported_Functions_Group2 KNX Auxiliary Timer Functions
  * @{
  */

/**
 *  @brief      Initilize the timer. 
 */
void KNX_InitTimer(void)
{
  timer_state = TIMER_RESET;
}

/**
 *  @brief      Start the timer.
 */
void KNX_StartTimer(void)
{
  /** Set timer state to running. */
  timer_state = TIMER_RUNNING;
  
  /** Set timer counter to 0. */
  timer_tick = 0;
}

/**
 *  @brief      Get the timer. 
 *  @retval     Value of the timer.
 */
uint32_t KNX_GetTick(void)
{
  return timer_tick;
}

/**
 *  @brief      Stop the timer. 
 *  @retval     Value of the timer.
 */
uint32_t KNX_StopTimer(void)
{
  /** Set timer state to pause. */
  timer_state = TIMER_PAUSE;
  
  /** return ::timer_tick. */
  return timer_tick;
}

/**
 *  @brief      Reset the timer. 
 *  @retval     Value of the timer.
 */
uint32_t KNX_ResetTimer(void)
{
  uint32_t ret;
    
  /** Set timer state to reset. */
  timer_state = TIMER_RESET;
  
  /** return ::timer_tick and set it back to 0. */
  ret = timer_tick;
  timer_tick = 0;
  
  return ret;
}

/**
 *  @brief      Get the timer's status. 
 *  @retval     Timer's status: ::TIMER_Status_t.
 */
TIMER_Status_t KNX_GetTimerState(void)
{
  return timer_state;
}

/**
 *  @brief      Check for time out or not. 
 *  @param      timeOnEntering: Time on entering this function
 *  @param      ticksToWait: ticks to wait before time out
 *  @retval     0 for false, 1 for true.
 */
uint8_t KNX_CheckForTimeOut(uint32_t * const timeOnEntering, uint32_t * const ticksToWait)
{
    /* Minor optimisation.  The tick count cannot change in this block. */
    const uint32_t xConstTickCount = KNX_GetTick();

    if( *ticksToWait == KNX_MAX_DELAY )
    {
      return 0;
    }
    else if(xConstTickCount - *timeOnEntering < *ticksToWait )
    {
      /* Not a genuine timeout. Adjust parameters for time remaining. */
      *ticksToWait -= (xConstTickCount - *timeOnEntering );
      *timeOnEntering = KNX_GetTick();
      return 0;
    }
    else
    {
      return 1;
    }
}

/**
 *  @brief      Systick interrupt routines. 
 */
void KNX_systick_isr(void)
{
  if(timer_state == TIMER_RUNNING)
  {
    timer_tick++;
  }
}

/**
  * @}
  */

/** @defgroup KNX_Aux_Exported_Functions_Group3 KNX Auxiliary Vertical Parity Checker
  * @{
  */

/**
 *  @brief      Vertical parity checker.
 *  @param      *datas: the trame containing \c uint8_t datas.
 *  @param      length: the length of the trame
 *  @retval     Vertical parity of datas: 0 for even, 1 for odd
 */
uint8_t KNX_VerticalParity(uint8_t *datas, uint16_t length)
{
  uint8_t parity = datas[0];
  uint16_t l;

  for (l = 1; l < length; l++)
  {
    parity ^= datas[l];
  }

  return parity;
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

/**
  * @}
  */