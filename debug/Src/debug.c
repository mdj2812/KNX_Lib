/**
  *****************************************************************************
  * @file       debug.c
  * @author
  * @version    V3.1.0
  * @date       26-July-2016
  * @brief      Debug tasks.
  *             This file provides functions to manage following functionalities:
  *              + Initialization functions and tasks
  *              + Perform DebugTask
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cola.h"
#include "debug.h"
#include "debug_UART.h"
#include "KNX_TPUart.h"
#include "KNX_Aux.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/** @defgroup   Cola_Debug Cola Debug
  * @brief      The Debug module to send messages to the monitor.
  * @{
  */

/** @defgroup Debug Debug
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup Debug_Private_Variables Debug Private Variables
  * @{
  */
/** \brief Current state of debug RX. */
static RX_DEBUG_Status_t KNX_PH_STATE;

/** \brief Error message put in ::DebugTask */
static const char *msg_error = "***ERROR*** cola_leer() = -1 ????? \r\n" ;

/** \brief The buffer to store the message */
static unsigned char buffer[BUFFER_SIZE];
/** \brief The indice of the ::buffer */
__IO uint16_t buffer_indice;
/** \brief The maximum value of the ::buffer_indice */
__IO uint16_t buffer_indice_max;

/** \brief The buffer to store the message received */
static unsigned char RX_buffer[RX_BUFFER_SIZE];
/** \brief The indice of the ::RX_buffer */
__IO uint16_t   RX_buffer_indice = 0;
/** \brief The temporary \c unsigned \c char to store the message received */
static unsigned char temp;

/** \brief Handler of the ::DebugTask */
static TaskHandle_t xDebugTaskHandle;
/** \brief Semaphore to permit the new cycle of ::DebugTask */
static SemaphoreHandle_t semaforo_debug_isruart;
/** \brief Used for yield the UART interrupt */
static BaseType_t xHigherPriorityTaskWoken;
/**
  * @}
  */

/* External functions --------------------------------------------------------*/
/** @defgroup Debug_External_Functions Debug External UART ISR Functions
  * @brief      External functions from \ref Debug_Uart module
  * @{
  */

/**
  * @brief      At the begin of interrupt, set ::xHigherPriorityTaskWoken to pdFalse.
  */
void debug_uart_isr_begin (void){
  xHigherPriorityTaskWoken = pdFALSE;
}

/**
  * @brief      At the end of interrupt, yield from the UART interrupt.
  */
void debug_uart_isr_end (void)
{
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
  * @brief      Send the buffer character by character. If all sent, give the
  *             semaphore in order to enter the next cycle of ::DebugTask.
  */
void debug_uart_isr_tx(void)
{
  buffer_indice++;
  if(buffer_indice > buffer_indice_max)
  {
    xSemaphoreGiveFromISR( semaforo_debug_isruart, &xHigherPriorityTaskWoken );
  }
  else
  {
    debug_uart_send(&buffer[buffer_indice], 1);
  }
}

/**
  * @brief      Receive the buffer character by character. If the buffer fits in
  *             the format as "/XX/" (XX: two digits of hexadecimal), then transmit
  *             it to TPUart.
  */
void debug_uart_isr_rx(void)
{
  uint8_t byte;

  if(temp == '/')
  {
    if(RX_buffer_indice != 2)
    {
      RX_buffer_indice = 0;
      
      RX_buffer[RX_buffer_indice] = temp;
      RX_buffer_indice++;
    }
    else
    {
      RX_buffer[RX_buffer_indice] = temp;
      KNX_PH_TPUart_Send(&byte, 1);
      RX_buffer_indice = 0;
    }
  }
  else
  {
    if(RX_buffer_indice == 1)
    {
      RX_buffer[RX_buffer_indice] = temp;
      RX_buffer_indice++;
    }
    else if(RX_buffer_indice == 2)
    {
      RX_buffer[RX_buffer_indice] = temp;
      RX_buffer_indice++;
      
      if(text2int(&RX_buffer[1], &byte) == AUX_ERROR_MSG)
      {
        RX_buffer_indice=0;
      }
    }
  }
}
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup Debug_Exported_Functions Debug Exported Functions
  * @{
  */

/** @defgroup Debug_Exported_Functions_Group1 Debug Initialization Functions
  * @{
  */

/**
  * @brief      Initialize the \ref Debug module.
  */
uint32_t DebugInit(void)
{
  KNX_PH_STATE = RX_DEBUG_IDLE;
  
  //inicializar cola+mutex para almacenar mensajes
  cola_init(&colaDebug);
  //inicializar semaforo compartido entre tarea debuj y la isr de la UART
  semaforo_debug_isruart = xSemaphoreCreateBinary();
  //crear tarea de depuracion
  xTaskCreate(
                DebugTask,       /* Function that implements the task. */
                "debug",          /* Text name for the task. */
                configMINIMAL_STACK_SIZE + 16, /* Stack size in words, not bytes. */
                ( void * ) 0,    /* Parameter passed into the task. */
                tskIDLE_PRIORITY,/* Priority at which the task is created. */
                &xDebugTaskHandle );      /* Used to pass out the created task's handle. */

  xTaskCreate(
                DebugRXTask,       /* Function that implements the task. */
                "debugRX",          /* Text name for the task. */
                configMINIMAL_STACK_SIZE + 16, /* Stack size in words, not bytes. */
                ( void * ) 0,    /* Parameter passed into the task. */
                tskIDLE_PRIORITY,/* Priority at which the task is created. */
                &xDebugTaskHandle );      /* Used to pass out the created task's handle. */
                
  //inicializar la UART de depuracion
  if(debug_uart_init())
  {
    KNX_PH_STATE = RX_DEBUG_KNX;
    return PH_Debug_ERROR_NONE;
  }
  else
  {
    KNX_PH_STATE = RX_DEBUG_IDLE;
    return PH_Debug_ERROR_INIT;
  }
}
/**
  * @}
  */

/** @defgroup Debug_Exported_Functions_Group2 Debug tasks functions
  * @{
  */

/**
  * @brief      Debug task. Check whether new messages are stored in ::colaDebug.
  *             If yes, send the new message through UART.
  * @param      argument:  argument of the task.
  */
void DebugTask(void * argument)
{
  /* USER CODE BEGIN DebugTask */
  int i, res_leer;

  /* Infinite loop */
  cola_guardar(&colaDebug, "\r\n");
  //cola_guardar(&colaDebug, "Mensaje desde DebugTask ... inicializando\r\n");
  for(;;)
  {
    res_leer = cola_leer(&colaDebug, buffer, BUFFER_SIZE);
    while (res_leer == 0) {
    vTaskDelay( 5 );
    res_leer = cola_leer(&colaDebug, buffer, BUFFER_SIZE);
    }
    if (res_leer == -1){
      //copiar en buffer mensaje de error
      for(i=0; msg_error[i] != '\n'; i++){
          buffer[i] = msg_error[i];
      }
      res_leer = i;
    }

    //Activar transmisión de la UART para transmitir el mensaje
    //almacenado en buffer que es de res_leer caracteres
    buffer_indice=0;
    buffer_indice_max = res_leer-1;
    debug_uart_send(&buffer[buffer_indice], 1);

    //Esperar a que la UART nos de permiso para continuar ..
    //Significa que el mensaje del buffer ya se ha enviado completamente
    xSemaphoreTake( semaforo_debug_isruart, portMAX_DELAY /* (TickType_t)10 */ );
  }
}

/**
  * @brief      Debug RX task. Transmit what received from debug uart towards
  *             KNX uart.
  * @param      argument:  argument of the task.
  */
void DebugRXTask(void * argument)
{  
  vTaskDelay( 200 );

  for(;;)
  {  
    debug_uart_receive (&temp, 1);
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
