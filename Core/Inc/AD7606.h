#ifndef __AD7606_H
#define __AD7606_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stdint.h"
#define AD7606OS0_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define AD7606OS0_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define AD7606OS1_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define AD7606OS1_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)
#define AD7606OS2_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)
#define AD7606OS2_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)

#define AD7606_CONVST_A_H HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET)
#define AD7606_CONVST_A_L HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET)
#define AD7606_CONVST_B_H HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)
#define AD7606_CONVST_B_L HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)

#define AD7606_SCLK_H HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_SET)
#define AD7606_SCLK_L HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13, GPIO_PIN_RESET)
#define AD7606_RESET_H HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5, GPIO_PIN_SET)
#define AD7606_RESET_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5, GPIO_PIN_RESET)
#define AD7606_CS_H HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_SET)
#define AD7606_CS_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_RESET)

#define AD7606_BUSY HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)
#define AD7606_DOUTA HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
// #define AD7606_DOUTB HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1)

#define DATA_LENGTH 4096

void AD7606_init(void);
void ADC_Delay(uint32_t nCount);
void ADC1_Configuration(void);
void AD7606_SETOS(uint8_t osv);
void AD7606_RESET(void);
uint16_t AD7606_ReadBytes(void);
void AD7606_STARTCONV(void);
uint8_t AD7606_Sample(uint8_t ch);
// void Data_Acquire(uint8_t ch);
// void Data_Acquire2(uint8_t ch);
// void Data_Acquire3(uint8_t ch);
#ifdef __cplusplus
}
#endif

#endif 

