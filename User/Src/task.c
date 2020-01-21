#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "AD7606.h"
#include "spi.h"
#include "arm_math.h"
#include "BC26.h"

uint32_t i = 0;
uint8_t firstFlag = 0;
uint8_t firstFlagFFT = 0;

uint16_t buf[16] = { 0 };

float out[4096] = { 0 };
arm_rfft_fast_instance_f32 S;
//arm_fir_decimate_instance_f32 S_fir50, S_fir100, S_fir150;
arm_status stat;

extern uint16_t transcplt;

extern float data[4096];

extern osSemaphoreId_t adSampleHandle;
extern osSemaphoreId_t adSampleCpltHandle;
extern osSemaphoreId_t FFTStartHandle;

void tBlinkLED(void *arg) {

	for (;;) {
		osDelay(1000);
		i += 1;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
	}
}

void tADCSample(void *arg) {
	for (;;) {
		osDelay(1000);
//		if (!firstFlag) {
//			osSemaphoreAcquire(adSampleCpltHandle, osWaitForever);
//			firstFlag = 1;
//		} else {
//			osSemaphoreAcquire(adSampleCpltHandle, osWaitForever);
//			AD7606_CS_H;
//			AD7606_STARTCONV();
//			while (AD7606_BUSY)
//				AD7606_CS_L;
//			HAL_SPI_Receive_IT(&hspi1, (uint8_t*) buf, 8);
//		}
	}
}

void tCalculateFFT(void *arg) {
	for (;;) {
		osDelay(1000);
//		if (!firstFlagFFT) {
//			osSemaphoreAcquire(FFTStartHandle, osWaitForever);
//			firstFlagFFT = 1;
//		}
//		osSemaphoreAcquire(FFTStartHandle, osWaitForever);
//		arm_rfft_fast_f32(&S, data, out, 0);
//		transcplt = 1;
	}
}
