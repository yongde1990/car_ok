/*
 **********************************************
 *�������ܣ�ME003ģ��IO�����ã�PA6���ME�ߵ�ƽ
 **********************************************
 */
 #include "ME003.h"

void MEGpioConfig(void)
{
	GPIO_InitTypeDef MyGpioInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	MyGpioInit.GPIO_Pin=GPIO_Pin_6;
	MyGpioInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
	
	GPIO_Init(GPIOA,&MyGpioInit);
}

