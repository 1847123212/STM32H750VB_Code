#include "pacommon.h"
#include "AD7606.h"
#include "spi.h"
#include "arm_math.h"
#include "tim.h"
#include "BC26.h"

uint8_t BC26Heap[10240];
extern arm_status stat;
extern arm_rfft_fast_instance_f32 S;

void PAGlobalInit() {
	AD7606_init();
	HAL_TIM_Base_Start_IT(&htim2);
	stat = arm_rfft_fast_init_f32(&S, 4096);
	construct(BC26Heap, 10240);
}
