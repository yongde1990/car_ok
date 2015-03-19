#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f10x.h"

#define ON 0
#define OFF 1

#define LED(x) if(x) \
	GPIO_SetBits(GPIOB, GPIO_Pin_8); \
	else \
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);

void LedGpioConfig(void);

#endif
