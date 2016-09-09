/**
  ******************************************************************************
  * @file       KNX_Ph.c
  * @author     MA Dingjie
  * @version    V1.0.0
  * @date       6-September-2016
  * @brief      KNX Physical Layer supervisor.
  *             This file provides functions to manage following functionalities:
  *              + Initialization functions
  *              + Send and Receive messages functions
  *              + Several basic services
  *              + State functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "KNX_Ph.h"
#include "KNX_Aux.h"
#include "KNX_Ph_TPUart.h"
#include "KNX_def.h"
#include "cola.h"
#include "debug.h"

/** @addtogroup KNX_Lib
  * @{
  */

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
static unsigned char KNX_PH_STATE_DEBUGMSG[] = "[KNX PH]KNX_PH_STATE changed to XX.\r\n";
/** \brief ::KNX_PH_STATE_DEBUGMSG digits indice. */
#define KNX_PH_STATE_DEBUGMSG_INDICE ((uint8_t)32)
/** \brief Sending Debug message sent in \ref Cola_Debug. */
static unsigned char KNX_PH_SEND_DEBUGMSG[] = "[KNX PH]Data sent: XX\r\n";
/** \brief ::KNX_PH_SEND_DEBUGMSG digits indice. */
#define KNX_PH_SEND_DEBUGMSG_INDICE ((uint8_t)19)
/** \brief Receiving Debug message sent in \ref Cola_Debug. */
static unsigned char KNX_PH_RECEIVE_DEBUGMSG[] = "[KNX PH]Data received: XX\r\n";
/** \brief ::KNX_PH_RECEIVE_DEBUGMSG digits indice. */
#define KNX_PH_RECEIVE_DEBUGMSG_INDICE ((uint8_t)23)
/** \brief Error Debug message sent in \ref Cola_Debug. */
static unsigned char KNX_PH_ERROR_DEBUGMSG[] = "[KNX PH]Error code: XX\r\n";
/** \brief ::KNX_PH_ERROR_DEBUGMSG digits indice. */
#define KNX_PH_ERROR_DEBUGMSG_INDICE ((uint8_t)20)

/** \brief The current tick of timer. */
TickType_t currentTick;

/** \brief Cola defined in \ref Debug */
t_cola colaDebug;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup KNX_PH_Sup_Private_Functions KNX_Ph_Sup Private Functions
  * @{
  */
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
  KNX_StartTimer();

  /** Initialize TPUart. */
  if(KNX_PH_TPUart_init() == TPUart_ERROR)
  {
    KNX_Ph_DebugMessage(PH_ERROR_INIT, ERROR_DEBUG);
    
    /** \b If TPUart initialization failed, return ::PH_ERROR_INIT */
    return PH_ERROR_INIT;
  }
      
  return PH_ERROR_NONE;
}

/**
  * @}
  */

/** @defgroup KNX_PH_Sup_Exported_Functions_Group2 Send/Receive Functions
  * @{
  */

/**
  * @brief      Send a data.
  * @param      data: a \c uint8_t data.
  * @param      timeout: timeout duration.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_SendData(uint8_t data, uint32_t timeout)
{  
  /** Try to send the data */
  currentTick = KNX_GetTick();
  while(!KNX_CheckForTimeOut(&currentTick, &timeout))
  {
    if(KNX_PH_TPUart_Send(&data, 1) == TPUart_OK)
    {
      KNX_Ph_DebugMessage(data, SEND_DEBUG);
      
      /** \b If succeeded, return ::PH_ERROR_NONE. */
      return PH_ERROR_NONE;
    }
  }
  
  KNX_Ph_DebugMessage(PH_ERROR_TIMEOUT, ERROR_DEBUG);

  /** \b If timeout, return ::PH_ERROR_TIMEOUT. */
  return PH_ERROR_TIMEOUT;
}

/**
  * @brief      Receive a data.
  * @param      data: a \c uint8_t data.
  * @param      timeout: timeout duration.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_RecData(uint8_t *data, uint32_t timeout)
{        
  /** Try to receive the data */
  currentTick = KNX_GetTick();
  while(!KNX_CheckForTimeOut(&currentTick, &timeout))
  {
    if(KNX_PH_TPUart_Receive(data, 1) == TPUart_OK)
    {      
      KNX_Ph_DebugMessage(*data, RECEIVE_DEBUG);

      /** \b If data received is the response expected. Return ::PH_ERROR_NONE. */
      return PH_ERROR_NONE;
    }
  }
  
  KNX_Ph_DebugMessage(PH_ERROR_TIMEOUT, ERROR_DEBUG);

  /** \b If timeout, return ::PH_ERROR_TIMEOUT. */
  return PH_ERROR_TIMEOUT;
}

/**
  * @brief      Wait for a response with timeout.
  * @param      res: response got.
  * @param      timeout: timeout duration.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_WaitFor(uint8_t res, uint32_t timeout)
{
  uint8_t data;
  
  /** Try to send the data */
  currentTick = KNX_GetTick();
  while(!KNX_CheckForTimeOut(&currentTick, &timeout))
  {
    if(KNX_PH_TPUart_Receive(&data, 1) == TPUart_OK)
    {      
      KNX_Ph_DebugMessage(data, RECEIVE_DEBUG);

      /** \b If data received is the response expected. */
      if(data == res)
      {
        /** Return ::PH_ERROR_NONE. */
        return PH_ERROR_NONE;
      }
    }
  }
  
  KNX_Ph_DebugMessage(PH_ERROR_TIMEOUT, ERROR_DEBUG);

  /** \b If timeout, return ::PH_ERROR_TIMEOUT. */
  return PH_ERROR_RESPONSE;
}

/**
  * @brief      Wait for a type of response with mask and timeout.
  * @param      res: response got.
  * @param      resMask: mask of the response expected.
  * @param      timeout: timeout duration.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_WaitForWithMask(uint8_t *res, uint8_t resMask, uint32_t timeout)
{
  uint8_t data;
  
  /** Try to send the data */
  currentTick = KNX_GetTick();
  while(!KNX_CheckForTimeOut(&currentTick, &timeout))
  {
    if(KNX_PH_TPUart_Receive(&data, 1) == TPUart_OK)
    {      
      KNX_Ph_DebugMessage(data, RECEIVE_DEBUG);

      /** \b If data received is the type of response expected. */
      if((data & resMask) == resMask)
      {
        *res = data;
        /** Return ::PH_ERROR_NONE. */
        return PH_ERROR_NONE;
      }
    }
  }
  
  KNX_Ph_DebugMessage(PH_ERROR_TIMEOUT, ERROR_DEBUG);

  /** \b If timeout, return ::PH_ERROR_TIMEOUT. */
  return PH_ERROR_RESPONSE;
}
/**
  * @}
  */

/** @defgroup KNX_PH_Sup_Exported_Functions_Group3 KNX PH Services
* @{
*/

/**
  * @brief      Request to reset the \ref KNX_PH module and waiting for the
  *             ::Reset_indication.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_Reset(void)
{
  uint8_t ret;
  
  /** Send ::Ph_Reset request. */
  if(KNX_Ph_GetState() != PH_RESET)
  {
    KNX_Ph_SetState(PH_RESET);
  }
  
  ret = KNX_Ph_SendData(U_Reset_request, KNX_DEFAULT_TIMEOUT);
  if(ret != PH_ERROR_NONE)
  {
    /** \b If encounter a problem, return ::PH_ERROR_REQUEST  */
    return PH_ERROR_REQUEST;
  }
    
  /** Waiting for the ::Reset_indication. */
  ret = KNX_Ph_WaitFor(Reset_indication, KNX_DEFAULT_TIMEOUT);
  if(ret == PH_ERROR_NONE)
  {
    /** \b If receive the response, set state to ::PH_NORMAL. */
    KNX_Ph_SetState(PH_NORMAL);

    /** Return ::PH_ERROR_NONE. */
    return PH_ERROR_NONE;
  }
  else
  {
    /** Else return ::PH_ERROR_RESPONSE. */
    return PH_ERROR_TIMEOUT;
  }
}

/**
  * @brief      Requests the internal communication state from the TP-UART-IC
  *             and waiting for the ::State_indication.
  * @param      res: response received.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_State(uint8_t *res)
{
  uint8_t ret;
  
  /** Send ::Ph_Reset request. */
  ret = KNX_Ph_SendData(U_State_request, KNX_DEFAULT_TIMEOUT);
  if(ret != PH_ERROR_NONE)
  {
    /** \b If encounter a problem, return ::PH_ERROR_REQUEST  */
    return PH_ERROR_REQUEST;
  }
    
  /** Waiting for the ::State_indication. */
  ret = KNX_Ph_WaitForWithMask(res, State_indication_mask, KNX_DEFAULT_TIMEOUT);
  if(ret == PH_ERROR_NONE)
  {
    /** Return ::PH_ERROR_STATE. */
    return PH_ERROR_NONE;
  }
  else
  {
    *res = NULL;
    
    /** Else return ::PH_ERROR_TIMEOUT. */
    return PH_ERROR_TIMEOUT;
  }
}

/**
  * @brief      Send datas.
  * @param      frame: frame to be sent.
  * @param      length: number of octets in frame.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_Data_req(uint8_t *frame, uint16_t length)
{
  uint8_t res;
  uint16_t i;
  
  /** Send U_L_DataStart byte and CTRL byte. */
  if(KNX_Ph_SendData(U_L_DataStart, KNX_DEFAULT_TIMEOUT) != PH_ERROR_NONE)
  {
    /** \b If encounter a problem, return ::PH_ERROR_REQUEST  */
    return PH_ERROR_REQUEST;
  }
  
  if(KNX_Ph_SendData(frame[0], KNX_DEFAULT_TIMEOUT) != PH_ERROR_NONE)
  {
    /** \b If encounter a problem, return ::PH_ERROR_TIMEOUT  */
    return PH_ERROR_TIMEOUT;
  }
  
  /** Send frame. */
  for(i=1; i<length-1; i++)
  {
    /** Send U_L_DataContinue byte and the frame. */
    if(KNX_Ph_SendData((U_L_DataContinue | i), KNX_DEFAULT_TIMEOUT) != PH_ERROR_NONE)
    {
      /** \b If encounter a problem, return ::PH_ERROR_TIMEOUT  */
      return PH_ERROR_TIMEOUT;
    }
    
    if(KNX_Ph_SendData(frame[i], KNX_DEFAULT_TIMEOUT) != PH_ERROR_NONE)
    {
      /** \b If encounter a problem, return ::PH_ERROR_TIMEOUT  */
      return PH_ERROR_TIMEOUT;
    }
  }
  
  /** Send U_L_DataEnd byte and CheckSum. */
  if(KNX_Ph_SendData((U_L_DataEnd | (uint8_t)length), KNX_DEFAULT_TIMEOUT) != PH_ERROR_NONE)
  {
    /** \b If encounter a problem, return ::PH_ERROR_TIMEOUT  */
    return PH_ERROR_TIMEOUT;
  }
  
  if(KNX_Ph_SendData(frame[length-1], KNX_DEFAULT_TIMEOUT) != PH_ERROR_NONE)
  {
    /** \b If encounter a problem, return ::PH_ERROR_TIMEOUT  */
    return PH_ERROR_TIMEOUT;
  }
  
  /** Waiting for the ::L_Data_confirm_success. */
  if(KNX_Ph_WaitForWithMask(&res, L_Data_confirm_mask, KNX_DEFAULT_TIMEOUT) == PH_ERROR_NONE)
  {
    if(res == L_Data_confirm_success)
    {
      /** Return ::PH_ERROR_NONE. */
      return PH_ERROR_NONE;
    }
    else
    {      
      /** Return ::PH_ERROR_DATA_CON_FAIL. */
      return PH_ERROR_DATA_CON_FAIL;
    }
  }
  else
  {
    /** Else return ::PH_ERROR_TIMEOUT. */
    return PH_ERROR_TIMEOUT;
  }
}

/**
  * @brief      Receive datas.
  * @param      frame: frame received.
  * @param      length: number of octets in frame.
  * @retval     Error code, See \ref PH_Error_Code.
  */
uint8_t KNX_Ph_Data_rec(uint8_t *frame, uint16_t *length)
{
  uint8_t ret;
  
  /** Receive frame. */
  for(*length=0; *length<FRAME_SIZE; *length++)
  {
    ret = KNX_Ph_RecData(&frame[*length], KNX_DEFAULT_TIMEOUT);
    if(ret != PH_ERROR_NONE)
    {
      return PH_ERROR_TIMEOUT;
    }
  }
  
  return PH_ERROR_NONE;
}
/**
  * @}
  */

/** @defgroup KNX_PH_Sup_Exported_Functions_Group4 KNX PH Supervisor State Function
  * @{
  */

/**
 *  @brief      Getter of the status of the physical layer. 
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
 *  @param      data: the data or error code.
 *  @param      type: the type of the message, see ::DEBUG_Type_t.
 */
static void KNX_Ph_DebugMessage(uint8_t data, DEBUG_Type_t type)
{ 
  switch(type)
  {
    case STATE_DEBUG:
      int2text(data, &KNX_PH_STATE_DEBUGMSG[KNX_PH_STATE_DEBUGMSG_INDICE]);
      cola_guardar(&colaDebug, KNX_PH_STATE_DEBUGMSG);
      break;
    case SEND_DEBUG:
      int2text(data, &KNX_PH_SEND_DEBUGMSG[KNX_PH_SEND_DEBUGMSG_INDICE]);
      cola_guardar(&colaDebug, KNX_PH_SEND_DEBUGMSG);
      break;
    case RECEIVE_DEBUG:
      int2text(data, &KNX_PH_RECEIVE_DEBUGMSG[KNX_PH_RECEIVE_DEBUGMSG_INDICE]);
      cola_guardar(&colaDebug, KNX_PH_RECEIVE_DEBUGMSG);
      break;
    default:
      int2text(data, &KNX_PH_ERROR_DEBUGMSG[KNX_PH_ERROR_DEBUGMSG_INDICE]);
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

/**
  * @}
  */