/**
  *****************************************************************************
  * @file       cola.c
  * @author
  * @version    V3.0.1 
  * @date       18-July-2016
  * @brief      Cola module.
  *             This file provides functions to manage following functionalities:
  *              + Initialization function
  *              + Save and read cola
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"

/* Demo application includes. */
#include "cola.h"
#include "stdio.h"

/** @addtogroup KNX_Lib
  * @{
  */

/** @addtogroup Cola_Debug
  * @{
  */

/** @defgroup Cola Cola
  * @{
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup Cola_Exported_Functions Cola Exported Functions
  * @{
  */

/** @defgroup Cola_Exported_Functions_Group1 Cola Initialization Functions
  * @{
  */

/**
  * @brief      Initialize the value of p.
  * @param      p: pointer to a #t_cola that contains information of cola and
  *             which will be used to store and read information.
  */
void cola_init(t_cola *p){
  p->cabeza = 0;
  p->cola = 0;
  p->items = 0;
  p->huecos = COLA_SIZE;
  p->mutex = xSemaphoreCreateMutex();
}
/**
  * @}
  */

/** @defgroup Cola_Exported_Functions_Group2 Save and Read Cola Functions
  * @{
  */

/**
  * @brief      Save the message into the end of cola's buffer. If at the end,
  *             restart form and cover the head of it.
  * @param      p: pointer to a t_cola in which we will stroe the message.
  * @param      msg: pointer to the message buffer.
  * @retval     1 for success, 0 for error
  */
int16_t cola_guardar (t_cola *p, unsigned char *msg){
  int16_t i,l,res = 1;

  /*Calcular la longitud del mensaje en l*/
  for (l = 0; msg[l] != '\n'; l++)
  {
  }
  l++;
  xSemaphoreTake(p->mutex, portMAX_DELAY /* (TickType_t)10 */ );

  if(l < p->huecos)
  {
    for(i = 0; i<l; i++)
    {
      /*guardar msg[i] en la cola p*/
      p->datos [ p->cola ] = msg[i];
      p->cola++;

      if(p->cola >= COLA_SIZE)
      {
       p->cola = 0;
      }

      p->huecos--;
      p->items++;
    }
  }
  else
  {
    res = 0;
  }

  xSemaphoreGive(p->mutex);

  return res;
}

/**
  * @brief      Read the message from the cola.
  * @param      p: pointer to the t_cola from where we will read the message.
  * @param      msg: pointer to the buffer to take the message.
  * @param      l: the max size of the buffer msg.
  * @retval     -1: if the message length from p depass the max size of msg.
  *             0: if p doesn't contain any message.
  *             >0: the length of the message successfully stored into msg.
  */
/*l es el tamaño del array msg,
para saber si el mensaje que se lee cabe o no cabe en msg.
El valor de retorno sera:
-1 si el mensaje de la cola p no cabe en msg.
0 si no hay mensaje.
1,2 o mas si cabe; l_de_p serEel valor de la longitud del mensaje*/
int16_t cola_leer (t_cola *p, unsigned char *msg, uint32_t l){

  int16_t i, res, l_de_p, Ndatos;

  xSemaphoreTake(p->mutex, portMAX_DELAY /* (TickType_t)10 */ );

  Ndatos = COLA_SIZE - p->huecos;

  /*Calcular la longitud del mensaje que se va a sacar de la cola p en l*/
  for(l_de_p = 0, i = p->cabeza; l_de_p <= Ndatos; l_de_p++)
  {
    if(p->datos[i] == '\n')
    {
        break;
    }
    i++;
    if(i == COLA_SIZE)
    {
       i = 0;
    }
  }

  if((Ndatos == 0 )||( p->datos[i] != '\n'))
  {
    res = 0;
  }
  else
  {
    l_de_p++;   // Contabilizar \n
    if(l_de_p > l)
    {
      res = -1;
    }
    else
    {
      res = l_de_p;
      for(i = 0; i<l_de_p; i++)
      {
        /*copiar msg[i] de la cola p al mensaje*/
        msg[i] = p->datos [ p->cabeza ];
        p->cabeza++;
        if(p->cabeza >= COLA_SIZE)
        {
          p->cabeza = 0;
        }
        p->huecos++;
      }
    }
  }

  xSemaphoreGive(p->mutex);

  return res;
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

/**
  * @}
  */
