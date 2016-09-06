/**
  ******************************************************************************
  * @file       KNX_DL.c
  * @author     MA Dingjie
  * @version    V0.0.1
  * @date       6-September-2016
  * @brief      KNX Data Link Layer.
  *             This file provides functions to manage following functionalities:
  *              + Initialization functions
  *              + State functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "KNX_DL.h"
#include "KNX_Ph.h"

/** @defgroup KNX_DL KNX Data Link Layer
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup KNX_DL_Private_Variables KNX Data Link Layer Private Variables
  * @{
  */
/** \brief Current state of KNX Data Link Layer. */
static DL_Status_t KNX_DL_STATE;

/** \brief The buffer to store the data before transmitted */
uint8_t Tx_LPDU_Datas[DL_TX_DATA_SIZE];
/** \brief The indice of the ::Tx_LPDU_Datas */
uint8_t Tx_LPDU_Datas_indice;
/** \brief The buffer to store the data received */
uint8_t Rx_LPDU_Datas[DL_RX_DATA_SIZE];
/** \brief The indice of the ::Rx_LPDU_Datas */
uint8_t Rx_LPDU_Datas_indice;
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup KNX_DL_Private_Functions KNX Data Link Layer Private Functions
  * @{
  */
static void     KNX_DL_SetState(DL_Status_t state);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup KNX_DL_Exported_Functions KNX Data Link Layer Exported Functions
  * @{
  */

/** @defgroup KNX_DL_Exported_Functions_Group1 Initialization Functions
  * @{
  */

/**
  * @brief      Initialize the \ref KNX_DL module.
  * @retval     Error code, See \ref DL_Error_Code.
  */
uint8_t KNX_DL_Init(void)
{  
  /** Set state to ::DL_NOINIT */
  KNX_DL_SetState(DL_NOINIT);
  
  /** TODO Initialize buffers */
  
  /** Send reset quest */
  KNX_DL_SetState(DL_RESET);
  if(KNX_Ph_Reset_request() == PH_ERROR_NONE)
  {
    if(KNX_Ph_Reset_request() == PH_ERROR_NONE)
    {
   	    KNX_DL_SetState(DL_NORMAL);

		return DL_ERROR_NONE;
    }
  }
   
  KNX_DL_SetState(DL_NOINIT);
   
  return DL_ERROR_INIT;
}

/**
  * @}
  */

/** @defgroup KNX_DL_Exported_Functions_Group2 KNX Data Link Layer State Function
  * @{
  */

/**
 *  @brief      Getter of the status of the Data Link Layer. 
 *  @retval     Physical Layer's status: ::DL_Status_t.
 */
DL_Status_t KNX_DL_GetState(void)
{
  return KNX_DL_STATE;
}
/**
  * @}
  */

/**
  * @}
  */

/* Private function ----------------------------------------------------------*/
/** @addtogroup KNX_DL_Private_Functions
  * @{
  */

/**
 *  @brief      Set the status of Data Link Layer. 
 *  @param      state: the state.
 */
static void     KNX_DL_SetState(DL_Status_t state)
{
  /** Change the ::KNX_DL_STATE to \b state */
  KNX_DL_STATE = state;
}

/**
  * @}
  */

/**
  * @}
  */