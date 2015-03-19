#include "motor.h"

/**
  * PB12->N1    PB13->N2  PB14->N3    PB15->N4
  *  蓝              绿             黄             橙
  */
void CarGpioConfig(void){
	/*分配GPIO_InitTypeDef 结构体*/
	GPIO_InitTypeDef CarGpioInit;
	/*打开GPIOB外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*设置GPIO_InitTypeDef 结构体*/
	CarGpioInit.GPIO_Pin = GPIO_Pin_12;
	CarGpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	CarGpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &CarGpioInit);
	CarGpioInit.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &CarGpioInit);
	CarGpioInit.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &CarGpioInit);
	CarGpioInit.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &CarGpioInit);
}

void MoveCar(CarControlData data){
	//data = data & 0xFF;
	switch (data){
		case BEFOR:
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
			GPIO_SetBits(GPIOB, GPIO_Pin_13); 
			GPIO_SetBits(GPIOB, GPIO_Pin_14); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_15); 
			break;
		case STOP:
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_15); 
			break;
		case BACK:
			GPIO_SetBits(GPIOB, GPIO_Pin_12); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
			GPIO_SetBits(GPIOB, GPIO_Pin_15); 
			break;
		case TurnBeforeRight:
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
			GPIO_SetBits(GPIOB, GPIO_Pin_13); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_15); 
			break;
		case TurnBeforeLift:
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
			GPIO_SetBits(GPIOB, GPIO_Pin_14); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_15); 
			break;
		case TurnBackRight:
			GPIO_SetBits(GPIOB, GPIO_Pin_12); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_15); 
			break;
		case TurnBackLift:
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
			GPIO_SetBits(GPIOB, GPIO_Pin_15); 
			break;
		default: 
			GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
			GPIO_ResetBits(GPIOB, GPIO_Pin_15); 
			break;
	}
}

