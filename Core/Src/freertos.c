/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "pacommon.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* USER CODE END Variables */
osThreadId_t BlinkLEDHandle;
osThreadId_t ADCSampleHandle;
osThreadId_t CalulateFFTHandle;
osSemaphoreId_t adSampleHandle;
osSemaphoreId_t adSampleCpltHandle;
osSemaphoreId_t FFTStartHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void tBlinkLED(void *argument);
void tADCSample(void *argument);
void tCalculateFFT(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  PAGlobalInit();
       
  /* USER CODE END Init */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of adSample */
  const osSemaphoreAttr_t adSample_attributes = {
    .name = "adSample"
  };
  adSampleHandle = osSemaphoreNew(1, 1, &adSample_attributes);

  /* definition and creation of adSampleCplt */
  const osSemaphoreAttr_t adSampleCplt_attributes = {
    .name = "adSampleCplt"
  };
  adSampleCpltHandle = osSemaphoreNew(1, 1, &adSampleCplt_attributes);

  /* definition and creation of FFTStart */
  const osSemaphoreAttr_t FFTStart_attributes = {
    .name = "FFTStart"
  };
  FFTStartHandle = osSemaphoreNew(1, 1, &FFTStart_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of BlinkLED */
  const osThreadAttr_t BlinkLED_attributes = {
    .name = "BlinkLED",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  BlinkLEDHandle = osThreadNew(tBlinkLED, NULL, &BlinkLED_attributes);

  /* definition and creation of ADCSample */
  const osThreadAttr_t ADCSample_attributes = {
    .name = "ADCSample",
    .priority = (osPriority_t) osPriorityAboveNormal,
    .stack_size = 128
  };
  ADCSampleHandle = osThreadNew(tADCSample, NULL, &ADCSample_attributes);

  /* definition and creation of CalulateFFT */
  const osThreadAttr_t CalulateFFT_attributes = {
    .name = "CalulateFFT",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  CalulateFFTHandle = osThreadNew(tCalculateFFT, NULL, &CalulateFFT_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_tBlinkLED */
/**
  * @brief  Function implementing the BlinkLED thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_tBlinkLED */
__weak void tBlinkLED(void *argument)
{
  /* USER CODE BEGIN tBlinkLED */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END tBlinkLED */
}

/* USER CODE BEGIN Header_tADCSample */
/**
* @brief Function implementing the ADCSample thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tADCSample */
__weak void tADCSample(void *argument)
{
  /* USER CODE BEGIN tADCSample */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END tADCSample */
}

/* USER CODE BEGIN Header_tCalculateFFT */
/**
* @brief Function implementing the CalulateFFT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_tCalculateFFT */
__weak void tCalculateFFT(void *argument)
{
  /* USER CODE BEGIN tCalculateFFT */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END tCalculateFFT */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
