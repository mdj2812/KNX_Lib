/**
  ******************************************************************************
  * @file       KNX_def.h
  * @author     MA Dingjie
  * @version    V0.0.1
  * @date       6-September-2016
  * @brief      This file contains KNX common defines,
  *             enumerations, macros, structures definitions, extern variables.
  ******************************************************************************
  */

#ifndef __KNX_DEF
#define __KNX_DEF

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

/** @addtogroup KNX_Lib
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup KNX_Lib_Constants KNX Library Constants
  * @brief    KNX Library Constants
  * @{
  */

/** @defgroup 8_Bits_Masks 8 bits masks
  * @{
  */
#define BIT0                            ((uint8_t)0x01U)    /*!< BIT0   */
#define BIT1                            ((uint8_t)0x02U)    /*!< BIT1   */
#define BIT2                            ((uint8_t)0x04U)    /*!< BIT2   */
#define BIT3                            ((uint8_t)0x08U)    /*!< BIT3   */
#define BIT4                            ((uint8_t)0x10U)    /*!< BIT4   */
#define BIT5                            ((uint8_t)0x20U)    /*!< BIT5   */
#define BIT6                            ((uint8_t)0x40U)    /*!< BIT6   */
#define BIT7                            ((uint8_t)0x80U)    /*!< BIT7   */
/**
  * @}
  */

/** @defgroup UART_Control_To Field of Services to UART
  * @brief    Services to UART
  * @{
  */
#define U_None                          ((uint8_t)0x00U)    /*!< None request   */
#define U_Reset_request                 ((uint8_t)0x01U)    /*!< Reset request  */
#define U_State_request                 ((uint8_t)0x02U)    /*!< State request  */
#define U_ActivateBusmon                ((uint8_t)0x05U)    /*!< Activate monitor mode  */
    
#define U_AckInformation_ACK            ((uint8_t)0x11U)    /*!< Acknowledgment addressed  */
#define U_AckInformation_Busy           ((uint8_t)0x12U)    /*!< Acknowledgment busy  */
#define U_AckInformation_Nack           ((uint8_t)0x14U)    /*!< Acknowledgment NACK  */

#define U_ProductID_request             ((uint8_t)0x20U)    /*!< Product ID request  */
#define U_ActivateBusyMode              ((uint8_t)0x21U)    /*!< Activate busy mode  */
#define U_ResetBusyMode                 ((uint8_t)0x22U)    /*!< Reset busy mode  */
#define U_MxRstCnt                      ((uint8_t)0x24U)    /*!< Adjust the max number of repetition  */
#define U_ActivateCRC                   ((uint8_t)0x25U)    /*!< Activate CRC  */
#define U_SetAddress                    ((uint8_t)0x28U)    /*!< Set address  */

#define U_L_DataStart                   ((uint8_t)0x80U)    /*!< Data Start Byte  */
#define U_L_DataContinue                ((uint8_t)0x80U)    /*!< Data Continue Byte  */
#define U_L_DataEnd                     ((uint8_t)0x80U)    /*!< Data End Byte  */
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
#define L_Data_confirm_mask             ((uint8_t)0x0BU)    /*!< Transmission failed    */
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
   
/** @defgroup KNX_Timeout Timeout Definition
  * @brief    Timeout Definition
  * @{
  */
#define KNX_DEFAULT_TIMEOUT     ((TickType_t)2000)               /*!< Default Timeout 500 ms*/
#define KNX_MAX_DELAY           ((TickType_t)0xFFFFFFFFU)       /*!< Max Delay: Infinity   */
/**
  * @}
  */
    
/** @defgroup KNX_Frame KNX Frame Definition
  * @brief    KNX Frame Definition
  * @{
  */
/** \brief Max size of the frame */
#define FRAME_SIZE 22
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

#endif /* __KNX_DEF */
