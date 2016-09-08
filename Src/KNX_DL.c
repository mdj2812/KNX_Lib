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
#include "KNX_def.h"
#include "KNX_Aux.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

/** @addtogroup KNX_Lib
  * @{
  */

/** @defgroup KNX_DL KNX Data Link Layer
  * @{
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup KNX_DL_Private_Consts KNX Data Link Layer Private Constants
  * @{
  */
/** \brief Maximal times to retry the request in case of failure. */
static const uint16_t retryTimes = 10;

/** \brief Current address of the device. */
static const uint16_t KNX_DL_SA = 0x00;
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup KNX_DL_Private_Variables KNX Data Link Layer Private Variables
  * @{
  */
/** \brief Current state of KNX Data Link Layer. */
static DL_Status_t KNX_DL_STATE;

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
  uint8_t res;
  uint16_t i;
  
  /** Set state to ::DL_POWER_ON */
  KNX_DL_SetState(DL_POWER_ON);
    
  /** Set state to ::DL_POWER_ON */
  KNX_DL_SetState(DL_RESET);

  /** Send reset quest until reset indication is received */
  for(i=0; i<retryTimes; i++)
  {
    if(KNX_Ph_Reset() == PH_ERROR_NONE)
    {
      /** Send state quest to confirm the state */
      if(KNX_Ph_State(&res) == PH_ERROR_NONE && res == Reset_indication)
      {
        HAL_GPIO_TogglePin(GPIOD, LD4_Pin);
        
        KNX_DL_SetState(DL_NORMAL);

        return DL_ERROR_NONE;
      }
    }
  }
   
  KNX_DL_SetState(DL_POWER_ON);
   
  return DL_ERROR_INIT;
}

/**
  * @}
  */

/* Services functions  ********************************************************/
/** @defgroup KNX_DL_Exported_Functions_Group2 KNX Data Link Layer Services functions
  * @{
  */

/**
 *  @brief      Send a frame to the KNX bus. 
 *  @param      Tx_FT: Frame Type
 *                      - 0: L_Data_Extended Frame
 *                      - 1: L_Data_Standard Frame
 *  @param      Tx_AT: Adrress Type
 *                      - 0: Individual
 *                      - 1: Group
 *  @param      Tx_DA: Destination Address
 *  @param      Tx_Pri: Priority of the data 
 *                      - 00: system priority
 *                      - 10: urgent priority
 *                      - 01: normal priority
 *                      - 11: low priority
 *  @param      Tx_LSDU: Datas of user Link Layer
 *  @param      Tx_LG: Length of LSDU
 *  @retval     Error code, See \ref DL_Error_Code.
 */
uint8_t KNX_DL_Data_req(uint8_t Tx_FT, uint8_t Tx_AT, uint16_t Tx_DA, uint8_t Tx_Pri, uint8_t *Tx_LSDU, uint8_t Tx_LG)
{
  uint8_t Tx_CTRL, TX_SA = KNX_DL_SA, Tx_ChkOct, i;
  uint8_t Frame[DL_FRAME_SIZE];
  
  Tx_CTRL = ((Tx_FT << 7) | BIT4 | (Tx_FT << 2));
  Tx_ChkOct = KNX_VerticalParity(Tx_LSDU, Tx_LG);
  
  Frame[0] = (uint8_t)Tx_CTRL;
  Frame[1] = (uint8_t)(TX_SA >> 4);
  Frame[2] = (uint8_t)(TX_SA & (0xFF));
  Frame[3] = (uint8_t)(Tx_DA >> 4);
  Frame[4] = (uint8_t)(Tx_DA & (0xFF));
  Frame[5] = (uint8_t)((Tx_AT << 7) | (0/*3 bits LSDU?*/) | (Tx_LG & (0x0F)));
  Frame[6] = (uint8_t)Tx_LSDU[0];
  for(i=0; i<Tx_LG; i++)
  {
    Frame[7+i] = (uint8_t)Tx_LSDU[i];
  }
  Frame[7+Tx_LG] = (uint8_t)Tx_ChkOct;
  
  if(KNX_Ph_Data(Frame, 8+Tx_LG) == PH_ERROR_NONE)
  {
    return DL_ERROR_NONE;
  }
  else
  {
    return DL_ERROR_TIMEOUT;
  }
}
/**
  * @}
  */

/** @defgroup KNX_DL_Exported_Functions_Group3 KNX Data Link Layer State Function
  * @{
  */

/**
 *  @brief      Getter of the status of the Data Link Layer. 
 *  @retval     Data Link Layer's status: ::DL_Status_t.
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

/**
  * @}
  */