/**
  ******************************************************************************
  * @file    BspGpio.c
  * @author  
  * @version V1.0
  * @date    
  * @brief   GPIO PB8 LED����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:stm32f103c8t6��Сϵͳ��
  *
  ******************************************************************************
  */

#include "BspGpio.h"

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LedGpioConfig(void){
	/*����GPIO_InitTypeDef �ṹ��*/
	GPIO_InitTypeDef MyGpioInit;
	/*��GPIOB����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*����GPIO_InitTypeDef �ṹ��*/
	MyGpioInit.GPIO_Pin = GPIO_Pin_8;
	MyGpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	MyGpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	/*����GPIOB*/
	GPIO_Init(GPIOB, &MyGpioInit);
}
