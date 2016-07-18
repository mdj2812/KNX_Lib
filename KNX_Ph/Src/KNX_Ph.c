/**
  ******************************************************************************
  * @file       KNX_Ph.c
  * @author     MA Dingjie
  * @version    V0.0.3
  * @date       18-July-2016
  * @brief      KNX Physical Layer supervisor.
  *             This file provides functions to manage following functionalities:
  *              + Initialization functions
  *              + Send and Receive messages functions
  *              + Timeout detection function
  *              + State functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "KNX_Ph.h"
#include "KNX_Aux.h"
#include "KNX_TPUart.h"
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
/** \brief Debug message sent in \ref Cola_Debug. */
static char KNX_PH_STATE_DEBUGMSG[] = "KNX_PH_STATE changed to XX\n";
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup KNX_PH_Sup_Private_Functions KNX_Ph_Sup Private Functions
  * @{
  */
static void     KNX_Ph_SetState(PH_Status_t state);
static uint32_t KNX_Ph_GetTick(void);
static uint32_t KNX_Ph_WaitOnUntilTimeOut(uint32_t Tickstart, uint32_t Timeout);
static uint32_t KNX_Ph_DebugMessage(uint8_t data);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/**
  * @}
  */

/**
  * @}
  */
