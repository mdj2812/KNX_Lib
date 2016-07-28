/**
  ******************************************************************************
  * @file       KNX_TPUart.h
  * @author     MA Dingjie
  * @version    V0.0.5
  * @date       20-July-2016
  * @brief      This file contains definitions and prototypes of functions for
  *             TP-UART.
  ******************************************************************************
  */

#ifndef __KNX_TPUART
#define __KNX_TPUART

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup KNX_PH
  * @{
  */

/** @addtogroup KNX_PH_TPUart
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup KNX_PH_TPUart_Exported_Constants TPUART Exported Constants
  * @{
  */

/** @defgroup UART_Control_To Field of Services to UART
  * @brief    Services to UART
  * @{
  */
#define U_None                          ((uint8_t)0x00U)    /*!< None request   */
#define U_Reset_request                 ((uint8_t)0x01U)    /*!< Reset request  */
#define U_State_request                 ((uint8_t)0x02U)    /*!< State request  */
#define U_ActivateBusmon                ((uint8_t)0x05U)    /*!< Activate monitor mode  */
#define U_AckInformation_Addressed      ((uint8_t)0x11U)    /*!< Acknowledgment addressed  */
#define U_AckInformation_NotAddressed   ((uint8_t)0x11U)    /*!< Acknowledgment not addressed  */

#define U_ProductID_request             ((uint8_t)0x20U)    /*!< Product ID request  */
#define U_ActivateBusyMode              ((uint8_t)0x21U)    /*!< Activate busy mode  */
#define U_ResetBusyMode                 ((uint8_t)0x22U)    /*!< Reset busy mode  */
#define U_MxRstCnt                      ((uint8_t)0x24U)    /*!< Adjust the max number of repetition  */
#define U_ActivateCRC                   ((uint8_t)0x25U)    /*!< Activate CRC  */
#define U_SetAddress                    ((uint8_t)0x28U)    /*!< Set address  */

#define U_L_DataStart                   ((uint8_t)0x80U)    /*!< Activate CRC  */
/**
  * @}
  */

/** @defgroup UART_Control_From Field of Services from UART
  * @brief    Services from UART
  * @{
  */
#define Reset_indication                ((uint8_t)0x03U)    /*!< Reset indication   */
#define State_indication                ((uint8_t)0x07U)    /*!< State indication  */
#define State_indication_mask           ((uint8_t)0x07U)    /*!< State indication mask  */
#define L_Data_confirm_success          ((uint8_t)0x8BU)    /*!< Transmission succeed   */
#define L_Data_confirm_failed           ((uint8_t)0x0BU)    /*!< Transmission failed    */
/**
  * @}
  */

/** @defgroup UART_Control_Mask Mask for STATE INDICATION service
  * @{
  */
#define TPUART_STATE_INDICATION_SLAVE_COLLISION_MASK  0x80  /*!< Slave Collision   */
#define TPUART_STATE_INDICATION_RECEIVE_ERROR_MASK    0x40  /*!< Receive Error   */
#define TPUART_STATE_INDICATION_TRANSMIT_ERROR_MASK   0x20  /*!< Transmit Error   */
#define TPUART_STATE_INDICATION_PROTOCOL_ERROR_MASK   0x10  /*!< Protocol Error   */
#define TPUART_STATE_INDICATION_TEMP_WARNING_MASK     0x08  /*!< Temp Warning   */
/**
  * @}
  */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup KNX_TPUart_Exported_Types KNX TPUart Exported Types
  * @brief    TPUart Status Enumeration
  * @{
  */

/** 
  * @brief  TPUart Status structures definition  
  */  
typedef enum 
{
  TPUart_OK       = 0x00U,      /*!< OK                                       */
  TPUart_ERROR    = 0x01U,      /*!< Error                                    */
  TPUart_BUSY     = 0x02U,      /*!< Busy                                     */
} TPUart_Status_t;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup KNX_PH_TPUart_Exported_Functions
  * @{
  */

/** @addtogroup KNX_PH_TPUart_Exported_Functions_Group1
  * @{
  */

/* Initialization functions ***************************************************/
uint8_t KNX_PH_TPUart_init(void);
/**
  * @}
  */

/** @addtogroup KNX_PH_TPUart_Exported_Functions_Group2
  * @{
  */

/* Send/Receive functions  ***************************************************/
uint8_t KNX_PH_TPUart_Send(uint8_t *data, uint16_t size);
uint8_t KNX_PH_TPUart_Receive(uint8_t *data, uint16_t size);
/**
  * @}
  */

/** @addtogroup KNX_PH_TPUart_Exported_Functions_Group3
  * @{
  */
/* UART Interrupt function  ***************************************************/
void TPUart_isr(void);
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

#ifdef __cplusplus
}
#endif

#endif /* __KNX_TPUART */
