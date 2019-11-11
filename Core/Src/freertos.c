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
uint8_t firstFlag =0;

uint16_t buf[16] = {0};
//q15_t fftBuf[4096];
//q15_t output[4096];
float out[4096] = {0};
arm_rfft_fast_instance_f32 S;
arm_status stat;

extern uint16_t transcplt;

extern float data[4096];
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId adStartSample_tHandle;
osThreadId FFTHandle;
osSemaphoreId adSampleHandle;
osSemaphoreId adSampleCpltHandle;
osSemaphoreId FFTStartHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void adStartSample(void const * argument);
void FFT_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
//	stat = arm_rfft_init_q15(&S, 4096, 0, 0);
	stat = arm_rfft_4096_fast_init_f32(&S);
//  for(int i = 0;i<4096; i++)
//  {
//    fftBuf[i] = arm_sin_q15(0xff*i);
//  }
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of adSample */
  osSemaphoreDef(adSample);
  adSampleHandle = osSemaphoreCreate(osSemaphore(adSample), 1);

  /* definition and creation of adSampleCplt */
  osSemaphoreDef(adSampleCplt);
  adSampleCpltHandle = osSemaphoreCreate(osSemaphore(adSampleCplt), 1);

  /* definition and creation of FFTStart */
  osSemaphoreDef(FFTStart);
  FFTStartHandle = osSemaphoreCreate(osSemaphore(FFTStart), 1);

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of adStartSample_t */
  osThreadDef(adStartSample_t, adStartSample, osPriorityLow, 0, 128);
  adStartSample_tHandle = osThreadCreate(osThread(adStartSample_t), NULL);

  /* definition and creation of FFT */
  osThreadDef(FFT, FFT_task, osPriorityLow, 0, 128);
  FFTHandle = osThreadCreate(osThread(FFT), NULL);

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
void StartDefaultTask(void const * argument)
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
void adStartSample(void const * argument)
{
  /* USER CODE BEGIN adStartSample */
  /* Infinite loop */
  for(;;)
  {
    if(!firstFlag)
		{
			osSemaphoreWait(adSampleCpltHandle, osWaitForever);
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
void FFT_task(void const * argument)
{
  /* USER CODE BEGIN FFT_task */
  /* Infinite loop */
  for(;;)
  {
    osSemaphoreWait(FFTStartHandle, osWaitForever);
//    arm_rfft_q15(&S, data, out);
		//arm_rfft_fast_f32(&S, data, out, 0);
		transcplt = 1;
  }
  /* USER CODE END FFT_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
