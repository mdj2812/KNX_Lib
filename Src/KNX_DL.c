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

/** \brief The buffer to store the data before transmitted */
uint8_t Tx_LPDU_Datas[FRAME_SIZE];
/** \brief The length of the ::Tx_LPDU_Datas */
uint16_t Tx_LPDU_Datas_length;
/** \brief The buffer to store the data received */
uint8_t Rx_LPDU_Datas[FRAME_SIZE];
/** \brief The length of the ::Rx_LPDU_Datas */
uint16_t Rx_LPDU_Datas_length;

/** \brief Current address of the device. */
static const uint16_t KNX_DL_SA = 0x0000;
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
  
  /** Initialize buffers */
  for(i=0; i<FRAME_SIZE; i++)
  {
    Tx_LPDU_Datas[i] = '\0';
    Tx_LPDU_Datas_length = 0;
    
    Rx_LPDU_Datas[i] = '\0';
    Rx_LPDU_Datas_length = 0;
  }
    
  /** Set state to ::DL_RESET */
  KNX_DL_SetState(DL_RESET);

  /** Send reset quest until reset indication is received */
  for(i=0; i<retryTimes; i++)
  {
    if(KNX_Ph_Reset() == PH_ERROR_NONE)
    {
      /** Send state quest to confirm the state */
      if(KNX_Ph_State(&res) == PH_ERROR_NONE && res == State_indication)
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
  uint8_t Tx_CTRL, Tx_ChkOct, i, ret;
  uint16_t Tx_SA = KNX_DL_SA;
  
  Tx_LPDU_Datas_length = 8 + Tx_LG;
  Tx_CTRL = ((Tx_FT << 7) | BIT4 | (Tx_Pri << 2));
  Tx_ChkOct = KNX_VerticalParity(Tx_LSDU, Tx_LG);
  
  Tx_LPDU_Datas[0] = (uint8_t)Tx_CTRL;
  Tx_LPDU_Datas[1] = (uint8_t)(Tx_SA >> 8);
  Tx_LPDU_Datas[2] = (uint8_t)(Tx_SA & (0xFF));
  Tx_LPDU_Datas[3] = (uint8_t)(Tx_DA >> 8);
  Tx_LPDU_Datas[4] = (uint8_t)(Tx_DA & (0xFF));
  Tx_LPDU_Datas[5] = (uint8_t)((Tx_AT << 7) | (0/*3 bits LSDU?*/) | (Tx_LG & (0x0F)));
  Tx_LPDU_Datas[6] = (uint8_t)Tx_LSDU[0];
  for(i=0; i<Tx_LG; i++)
  {
    Tx_LPDU_Datas[7+i] = (uint8_t)Tx_LSDU[i];
  }
  Tx_LPDU_Datas[7+Tx_LG] = (uint8_t)Tx_ChkOct;
  
  ret = KNX_Ph_Data_req(Tx_LPDU_Datas, Tx_LPDU_Datas_length);
  if(ret == PH_ERROR_NONE)
  {
    return DL_ERROR_NONE;
  }
  else if(ret == PH_ERROR_DATA_CON_FAIL)
  {
    return DL_ERROR_DATA_CON_FAIL;
  }
  else
  {
    return DL_ERROR_TIMEOUT;
  }
}

/**
 *  @brief      Receive a frame from the KNX bus. 
 *  @retval     Error code, See \ref DL_Error_Code.
 */
uint8_t KNX_DL_Data_rec(uint8_t *Rx_FT, uint8_t *Rx_AT, uint16_t *Rx_SA, uint8_t *Rx_Pri, uint8_t *Rx_LSDU, uint8_t *Rx_LG)
{
  uint8_t Rx_CTRL, i;
  uint16_t Rx_DA;
  
  if(KNX_Ph_Data_rec(Rx_LPDU_Datas, &Rx_LPDU_Datas_length) == PH_ERROR_NONE)
  {
    if(Rx_LPDU_Datas_length < 7)
    {
      return DL_ERROR_FRAME;
    }
    
    Rx_CTRL = Rx_LPDU_Datas[0];
    
    if(((Rx_CTRL & BIT4) != BIT4) || ((Rx_CTRL & ~(0x03)) != Rx_CTRL))
    {
      return DL_ERROR_FRAME;
    }
    
    *Rx_FT = (Rx_CTRL >> 6);
    *Rx_Pri = (Rx_CTRL >> 2) & (0x03);
    
    /** If Data Fomat is standard */
    if(*Rx_FT == 0x10)
    {
      *Rx_SA = (Rx_LPDU_Datas[1] << 8) | Rx_LPDU_Datas[2];
      Rx_DA = (Rx_LPDU_Datas[3] << 8) | Rx_LPDU_Datas[4];
      
      if(Rx_DA != KNX_DL_SA)
      {
        return DL_ERROR_ADDRESS;
      }
      
      /** If Checksum is incorrect */
      if(KNX_VerticalParity(Rx_LPDU_Datas, Rx_LPDU_Datas_length-1) != Rx_LPDU_Datas[Rx_LPDU_Datas_length-1])
      {
        /** Send NACK */
        KNX_Ph_SendData(U_AckInformation_Nack, KNX_DEFAULT_TIMEOUT);
        return DL_ERROR_FRAME;
      }
      
      *Rx_AT = Tx_LPDU_Datas[5] >> 7;
      *Rx_LG = Tx_LPDU_Datas[5] & (0x0F);
      /** If length is incorrect */
      if(Rx_LPDU_Datas_length != *Rx_LG + 8)
      {
        /** Send NACK */
        KNX_Ph_SendData(U_AckInformation_Nack, KNX_DEFAULT_TIMEOUT);
        return DL_ERROR_FRAME;
      }
      
      for(i=0; i<*Rx_LG; i++)
      {
        Rx_LSDU[i] = Rx_LPDU_Datas[7+i];
      }
      
      /** If in busy mode */
      if(KNX_DL_STATE == DL_BUSY)
      {
        /** Send BUSY */
        KNX_Ph_SendData(U_AckInformation_Busy, KNX_DEFAULT_TIMEOUT);
        return DL_ERROR_BUSY;        
      }

      /** Send ACK */
      KNX_Ph_SendData(U_AckInformation_ACK, KNX_DEFAULT_TIMEOUT);
      return DL_ERROR_NONE;        
    }
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