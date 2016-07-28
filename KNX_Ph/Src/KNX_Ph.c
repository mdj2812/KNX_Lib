/**
  ******************************************************************************
  * @file       KNX_Ph.c
  * @author     MA Dingjie
  * @version    V0.0.5
  * @date       20-July-2016
  * @brief      KNX Physical Layer supervisor.
  *             This file provides functions to manage following functionalities:
  *              + Initialization functions
  *              + Send and Receive messages functions
  *              + State functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "KNX_Ph.h"
#include "KNX_Aux.h"
#include "KNX_TPUart.h"
#include "cola.h"
#include "debug.h"

/** @defgroup KNX_PH KNX Physical Layer
  * @{
  */

/** @defgroup KNX_PH_Sup KNX Physical Layer Supervisor
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup KNX_PH_Sup_Private_Variables KNX_Ph_Sup Private Variables
  * @{
  */
/** \brief Current state of KNX physical layer. */
static PH_Status_t KNX_PH_STATE;
/** \brief State Debug message sent in \ref Cola_Debug. */
static unsigned char KNX_PH_STATE_DEBUGMSG[] = "KNX_PH_STATE changed to XX.\r\n";
/** \brief Sending Debug message sent in \ref Cola_Debug. */
static unsigned char KNX_PH_SEND_DEBUGMSG[] = "Message sent: /XX/\r\n";
/** \brief Receiving Debug message sent in \ref Cola_Debug. */
static unsigned char KNX_PH_RECEIVE_DEBUGMSG[] = "Message received: [XX]\r\n";
/** \brief Error Debug message sent in \ref Cola_Debug. */
static unsigned char KNX_PH_ERROR_DEBUGMSG[] = "Error!\r\n";

/** \brief Cola defined in \ref Debug */
t_cola colaDebug;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup KNX_PH_Sup_Private_Functions KNX_Ph_Sup Private Functions
  * @{
  */
static uint8_t  request2data(PH_Request_t request);
static void     KNX_Ph_SetState(PH_Status_t state);
static void     KNX_Ph_DebugMessage(uint8_t data, DEBUG_Type_t type);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup KNX_PH_Sup_Exported_Functions KNX Physical Layer Supervisor Exported Functions
  * @{
  */

/** @defgroup KNX_PH_Sup_Exported_Functions_Group1 Initialization Functions
  * @{
  */

/**
  * @brief      Initialize the \ref KNX_PH module.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_Init(void)
{  
  /** Set state to ::PH_NOINIT */
  KNX_Ph_SetState(PH_NOINIT);
  
  /** Initialize the timer. */
  KNX_InitTimer();
  /** Initialize TPUart. */
  if(KNX_PH_TPUart_init() == TPUart_ERROR)
  {
    /** \b If TPUart initialization failed, return ::PH_ERROR_INIT */
    return PH_ERROR_INIT;
  }
  return PH_ERROR_NONE;
}

/**
  * @brief      Reset the \ref KNX_PH module.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_Reset(void)
{
  /** Set state to ::PH_RESET. */
  KNX_Ph_SetState(PH_RESET);

  /** Send ::Ph_Reset request. */
  if(KNX_Ph_Send(Ph_Reset, KNX_DEFAULT_TIMEOUT) != PH_ERROR_NONE)
  {
    /** \b If encounter a problem, return ::PH_ERROR_REQUEST  */
    return PH_ERROR_REQUEST;
  }
    
  /** Waiting for the ::Reset_indication. */
  if(KNX_Ph_Receive(Reset_indication, KNX_DEFAULT_TIMEOUT) == PH_ERROR_NONE)
  {
    /** \b If receive the response, set state to ::PH_NORMAL. */
    KNX_Ph_SetState(PH_NORMAL);

    /** Return ::PH_ERROR_NONE. */
    return PH_ERROR_NONE;
  }
  else
  {
    /** \b If times out, set state to ::PH_NOINIT. */
    KNX_Ph_SetState(PH_NOINIT);

    /** Return ::PH_ERROR_TIMEOUT. */
    return PH_ERROR_TIMEOUT;
  }
}
/**
  * @}
  */

/** @defgroup KNX_PH_Sup_Exported_Functions_Group2 Send/Receive Functions
  * @{
  */

/**
  * @brief      Send a request.
  * @param      request: a request, see ::PH_Request_t.
  * @param      timeout: timeout duration.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_Send(PH_Request_t request, uint32_t timeout)
{
  uint8_t data;
  
  /** Convert the request to \c uint8_t data. */
  data = request2data(request);
  
  /** \b If the request is not valid, return ::PH_ERROR_REQUEST. */
  if(data == U_None)
  {
    return PH_ERROR_REQUEST;
  }
    
  KNX_StartTimer(timeout);
  /** Try to send the data */
  while(KNX_GetTimerState() != TIMER_TIMEOUT)
  {
    if(KNX_PH_TPUart_Send(&data, 1) == TPUart_OK)
    {
      KNX_Ph_DebugMessage(data, SEND_DEBUG);
      
      KNX_ResetTimer();
      /** \b If succeeded, return ::PH_ERROR_NONE. */
      return PH_ERROR_NONE;
    }
  }
  
  KNX_ResetTimer();
  /** \b If timeout, return ::PH_ERROR_TIMEOUT. */
  return PH_ERROR_TIMEOUT;
}

/**
  * @brief      Receive a response.
  * @param      response: response expected.
  * @param      timeout: timeout duration.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_Receive(uint8_t response, uint32_t timeout)
{
  uint8_t *data;
  
  KNX_StartTimer(timeout);
  /** Try to receive the data */
  while(KNX_GetTimerState() != TIMER_TIMEOUT)
  {
    if(KNX_PH_TPUart_Receive(data, 1) == TPUart_OK)
    {
      KNX_Ph_DebugMessage(*data, RECEIVE_DEBUG);
      
      /** \b If data received is the response expected. */
      if(*data == response)
      {
       /** Return ::PH_ERROR_NONE. */
       return PH_ERROR_NONE;
      }
    }
  }
  
  KNX_ResetTimer();
  /** \b If timeout, return ::PH_ERROR_TIMEOUT. */
  return PH_ERROR_TIMEOUT;

}
/**
  * @}
  */

/** @defgroup KNX_PH_Sup_Exported_Functions_Group3 KNX PH Supervisor State Function
  * @{
  */

/**
 *  @brief      Get the status. 
 *  @retval     Physical Layer's status: ::PH_Status_t.
 */
PH_Status_t KNX_Ph_GetState(void)
{
  return KNX_PH_STATE;
}
/**
  * @}
  */

/**
  * @}
  */

/* Private function ----------------------------------------------------------*/
/** @addtogroup KNX_PH_Sup_Private_Functions
  * @{
  */

/**
 *  @brief      Convert a request to a data. 
 *  @param      request: the request in ::PH_Request_t.
 *  @retval     The 8 bits data.
 */
static uint8_t  request2data(PH_Request_t request)
{
  switch(request)
  {
    case Ph_Reset:
      return 0x01U;
    case Ph_State:
      return 0x02U;
    case Ph_ActivateBusmon:
      return 0x05U;
    case Ph_ProductID:
      return 0x20U;
    case Ph_ActivateBusyMode:
      return 0x21U;
    case Ph_ResetBusyMode:
      return 0x22U;
    case Ph_SetAddress:
      return 0x28U;
    default:
      return 0x00U;
  }
}

/**
 *  @brief      Set the status of Physical layer. 
 *  @param      state: the state.
 */
static void     KNX_Ph_SetState(PH_Status_t state)
{
  /** Change the ::KNX_PH_STATE to \b state */
  KNX_PH_STATE = state;
  /** Send the debug message that the status has changed */
  KNX_Ph_DebugMessage(state, STATE_DEBUG);
}

/**
 *  @brief      Send the debug message to indicate the change of the state. 
 *  @param      data: the data.
 *  @param      type: the type of the message, see ::DEBUG_Type_t.
 */
static void KNX_Ph_DebugMessage(uint8_t data, DEBUG_Type_t type)
{ 
  switch(type)
  {
    case STATE_DEBUG:
      int2text(data, &KNX_PH_STATE_DEBUGMSG[24]);
      cola_guardar(&colaDebug, KNX_PH_STATE_DEBUGMSG);
      break;
    case SEND_DEBUG:
      int2text(data, &KNX_PH_SEND_DEBUGMSG[15]);
      cola_guardar(&colaDebug, KNX_PH_SEND_DEBUGMSG);
      break;
    case RECEIVE_DEBUG:
      int2text(data, &KNX_PH_SEND_DEBUGMSG[15]);
      cola_guardar(&colaDebug, KNX_PH_RECEIVE_DEBUGMSG);
      break;
    default:
      cola_guardar(&colaDebug, KNX_PH_ERROR_DEBUGMSG);
  }
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