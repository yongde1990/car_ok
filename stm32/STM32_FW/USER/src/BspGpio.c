/**
  ******************************************************************************
  * @file    BspGpio.c
  * @author  
  * @version V1.0
  * @date    
  * @brief   GPIO PB8 LED测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:stm32f103c8t6最小系统板
  *
  ******************************************************************************
  */

#include "BspGpio.h"

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LedGpioConfig(void){
	/*分配GPIO_InitTypeDef 结构体*/
	GPIO_InitTypeDef MyGpioInit;
	/*打开GPIOB外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*设置GPIO_InitTypeDef 结构体*/
	MyGpioInit.GPIO_Pin = GPIO_Pin_8;
	MyGpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	MyGpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	/*配置GPIOB*/
	GPIO_Init(GPIOB, &MyGpioInit);
}
