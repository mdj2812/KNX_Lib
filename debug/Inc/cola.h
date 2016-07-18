/**
  ****************************************************************************
  * @file       cola.h
  * @author
  * @version    V3.0.1 
  * @date       18-July-2016
  * @brief      Cola module.
  ******************************************************************************
  */

#ifndef cola_H
#define cola_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"

/** @addtogroup Cola_Debug
  * @{
  */

/** @addtogroup Cola
  * @{
  */

/* Private define ------------------------------------------------------------*/
/** @defgroup Cola_Private_Define Cola Private Define
  * @{
  */
/** \brief Max size of the t_cola.datos */
#define COLA_SIZE 4*1024
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup Cola_Exported_Types Cola Exported Types
  * @{
  */

/**
  * @brief       Cola Structure definition
  */
typedef struct
{
  /*huecos vacios de tamaño COLA_SIZE, indice de lectura, indice de escritura */
  uint32_t huecos;              /*!< Free places of #datos                    */
  uint32_t cabeza;              /*!< The beginning indice of the new message  */
  uint32_t cola;                /*!< The end indice of the new message        */
  uint32_t items;               /*!< Items in the #datos                      */

  SemaphoreHandle_t mutex;      /*!< The mutex to prevent the colision of
                                      #cola_leer and #cola_guardar            */

  uint8_t datos[COLA_SIZE];     /*!< The buffer who stroes all messages       */
}t_cola;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Cola_Exported_Functions
  * @{
  */

/** @addtogroup Cola_Exported_Functions_Group1
  * @{
  */
/* Initialization functions ***************************************************/
void cola_init(t_cola *p);
/**
  * @}
  */

/** @addtogroup Cola_Exported_Functions_Group2
  * @{
  */
/* Save/Read cola functions ***************************************************/
int16_t cola_guardar (t_cola *p, unsigned char *msg);
int16_t cola_leer (t_cola *p, unsigned char *msg, uint32_t l);
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

#endif
