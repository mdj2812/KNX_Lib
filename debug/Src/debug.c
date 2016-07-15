/*******************************************************************************
  * @file       debug.c
  * @author  
  * @version    V3.0.0 
  * @date       14-July-2016
  * @brief      Debug tasks
  *             This file provides functions to manage following functionalities:
  *              + Initialization functions and tasks
  *              + Perform DebugTask
  *              + Perform 2 testing tasks based on time        
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cola.h"
#include "debug.h"
#include "debug_UART.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

/** @defgroup Cola_Debug Cola Debug
  * @{
  */

/** @defgroup Debug Debug
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup Debug_Private_Variables Debug Private Variables
  * @{
  */
/** \brief Error message put in ::DebugTask */
static const char *msg_error = "***ERROR*** cola_leer() = -1 ????? \r\n" ;

/** \brief The ::buffer to store the message */
static unsigned char buffer[BUFFER_SIZE];
/** \brief The indice of the ::buffer */
__IO uint16_t buffer_indice;
/** \brief The maximum value of the ::buffer_indice */
__IO uint16_t buffer_indice_max;

/** \brief Handler of the ::DebugTask */
static TaskHandle_t xDebugTaskHandle;
/** \brief Handler of the ::DebugGuardar1sTask */
static TaskHandle_t xGuardar1sTaskHandle;
/** \brief Handler of the ::DebugGuardar2sTask */
static TaskHandle_t xGuardar2sTaskHandle;
/** \brief Semaphore to permit the new cycle of ::DebugTask */
static SemaphoreHandle_t semaforo_debug_isruart;
/** \brief Semaphore to permit the write in of ::DebugGuardar1sTask */
static SemaphoreHandle_t semaforo_debug_isrsystick_1s;
/** \brief Semaphore to permit the write in of ::DebugGuardar2sTask */
static SemaphoreHandle_t semaforo_debug_isrsystick_2s;
/** \brief Used for yield the UART interrupt */
static BaseType_t xHigherPriorityTaskWoken;
/** \brief Used for yield the Systick interrupt */
static BaseType_t xGuardarSomeTaskWoken;

/** \brief The counter of ::DebugGuardar1sTask */
__IO uint16_t counter_1s = 1;
/** \brief The counter of ::DebugGuardar2sTask */
__IO uint16_t counter_2s = 1001;
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
  * @brief      UART RX interrupt routine. 
  */
void debug_uart_isr_rx(void)
{
	// No hacer nada
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
void DebugInit(void)
{  
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

  /* definition and creation of semaforo_debug_isrsystick */
  semaforo_debug_isrsystick_1s = xSemaphoreCreateBinary();
  semaforo_debug_isrsystick_2s = xSemaphoreCreateBinary();

  /* definition and creation of DebugGuardarTask */
  xTaskCreate(
                DebugGuardar1sTask,       /* Function that implements the task. */
                "Guardar_1s",          /* Text name for the task. */
                configMINIMAL_STACK_SIZE + 16, /* Stack size in words, not bytes. */
                ( void * ) 0,    /* Parameter passed into the task. */
                tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
                &xGuardar1sTaskHandle );      /* Used to pass out the created task's handle. */

  xTaskCreate(
                DebugGuardar2sTask,       /* Function that implements the task. */
                "Guardar_2s",          /* Text name for the task. */
                configMINIMAL_STACK_SIZE + 16, /* Stack size in words, not bytes. */
                ( void * ) 0,    /* Parameter passed into the task. */
                tskIDLE_PRIORITY+2,/* Priority at which the task is created. */
                &xGuardar2sTaskHandle );      /* Used to pass out the created task's handle. */

  //inicializar la UART de depuracion
  debug_uart_init();
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
  * @brief      Testing task. Store a buffer into ::colaDebug every 1 second.
  * @param      argument:  argument of the task.
  */
void DebugGuardar1sTask(void * argument)
{
  unsigned char buffer_1s[20], Task_name[20];
  
  strcpy((char*) Task_name, pcTaskGetName(xGuardar1sTaskHandle));
  for(;;)
  {
    if(xSemaphoreTake(semaforo_debug_isrsystick_1s, portMAX_DELAY))
    {
      sprintf((char*) buffer_1s, "[%d\t]%s\t: %d\r\n", xTaskGetTickCount(), Task_name, counter_1s++);
      cola_guardar(&colaDebug, buffer_1s);
    }
  }
}

/**
  * @brief      Testing task. Store a buffer into ::colaDebug every 2 second.
  * @param      argument:  argument of the task.
  */
void DebugGuardar2sTask(void * argument)
{
  unsigned char buffer_2s[20], Task_name[20];
  
  strcpy((char*) Task_name, pcTaskGetName(xGuardar2sTaskHandle));
  for(;;)
  {
    if(xSemaphoreTake(semaforo_debug_isrsystick_2s, portMAX_DELAY))
    {
      sprintf((char*) buffer_2s, "[%d\t]%s\t: %d\r\n", xTaskGetTickCount(), Task_name, counter_2s++);
      cola_guardar(&colaDebug, buffer_2s);
    }
  }
}
/**
  * @}
  */

/** @defgroup Debug_Exported_Functions_Group3 Debug Systic Test ISR Functions
  * @{
  */

/**
  * @brief      At the begin of interrupt, set ::xGuardarSomeTaskWoken to pdFalse. 
  */
void debug_systick_isr_begin(void)
{
  xGuardarSomeTaskWoken = pdFALSE;
}

/**
  * @brief      Every 1 second, give the semaphore to give ::DebugGuardar1sTask to
  *             store a message in ::colaDebug. 
  */
void debug_systick_isr_1s(void)
{
  xSemaphoreGiveFromISR( semaforo_debug_isrsystick_1s, &xGuardarSomeTaskWoken );
}

/**
  * @brief      Every 2 second, give the semaphore to give ::DebugGuardar2sTask to
  *             store a message in ::colaDebug. 
  */
void debug_systick_isr_2s(void)
{
  xSemaphoreGiveFromISR( semaforo_debug_isrsystick_2s, &xGuardarSomeTaskWoken );
}

/**
  * @brief      At the end of interrupt, yield from the Systick interrupt. 
  */
void debug_systick_isr_end(void)
{
  portYIELD_FROM_ISR( xGuardarSomeTaskWoken );
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