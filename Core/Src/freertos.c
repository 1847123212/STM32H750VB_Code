/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "AD7606.h"
#include "spi.h"
#include "arm_math.h"
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
uint32_t i =0;
uint8_t firstFlag = 0;
uint8_t firstFlagFFT = 0;

uint16_t buf[16] = {0};

float out[4096] = {0};
arm_rfft_fast_instance_f32 S;
//arm_fir_decimate_instance_f32 S_fir50, S_fir100, S_fir150;
arm_status stat;

extern uint16_t transcplt;

extern float data[4096];
/* USER CODE END Variables */
osThreadId_t defaultTaskHandle;
osThreadId_t adStartSample_tHandle;
osThreadId_t FFTHandle;
osSemaphoreId_t adSampleHandle;
osSemaphoreId_t adSampleCpltHandle;
osSemaphoreId_t FFTStartHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void adStartSample(void *argument);
void FFT_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	stat = arm_rfft_4096_fast_init_f32(&S);
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
  /* definition and creation of defaultTask */
  const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* definition and creation of adStartSample_t */
  const osThreadAttr_t adStartSample_t_attributes = {
    .name = "adStartSample_t",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  adStartSample_tHandle = osThreadNew(adStartSample, NULL, &adStartSample_t_attributes);

  /* definition and creation of FFT */
  const osThreadAttr_t FFT_attributes = {
    .name = "FFT",
    .priority = (osPriority_t) osPriorityLow,
    .stack_size = 128
  };
  FFTHandle = osThreadNew(FFT_task, NULL, &FFT_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
		i+=1;
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_adStartSample */
/**
* @brief Function implementing the adStartSample_t thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adStartSample */
void adStartSample(void *argument)
{
  /* USER CODE BEGIN adStartSample */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
		{
			firstFlag = 1;
		}
		else
		{
			osSemaphoreWait(adSampleCpltHandle, osWaitForever);
			AD7606_CS_H;
			AD7606_STARTCONV();

			while(AD7606_BUSY);
			AD7606_CS_L;

			HAL_SPI_Receive_IT(&hspi1,(uint8_t * )buf,8);
		}
	}
  /* USER CODE END adStartSample */
}

/* USER CODE BEGIN Header_FFT_task */
/**
* @brief Function implementing the FFT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FFT_task */
void FFT_task(void *argument)
{
  /* USER CODE BEGIN FFT_task */
    for(;;)
    {
        if(!firstFlagFFT)
        {
            osSemaphoreWait(FFTStartHandle, osWaitForever);
            firstFlagFFT = 1;
        }
        osSemaphoreWait(FFTStartHandle, osWaitForever);
        //    arm_rfft_q15(&S, data, out);
        arm_rfft_fast_f32(&S, data, out, 0);
        transcplt = 1;
    }
  /* USER CODE END FFT_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
