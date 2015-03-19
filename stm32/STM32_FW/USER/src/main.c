/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    
  * @brief   ���Կ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:stm32f103c8t6��Сϵͳ��
  * 
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include <string.h>
#include "BspGpio.h"
#include "delay.h"
#include "DHT11.h"
#include "myuart.h"
#include "DataConfig.h"
#include "testdata.h"
#include "convert.h"
#include "motor.h"

#include "mq2.h"
#include "ME003.h"

UserOrder MyUserOrder = NoOrder;
ReturnError RETERR;

//extern DHT11Data TestDHTData;
extern __IO uint16_t ADC_ConvertedValue;
//float ADC_ConvertedValueLocal;

/* �����Ӧ :����Ϊ0xff, ����Ϊ0x00  . ����ʱ�˿ڼ��Ϊ�ߵ�ƽ*/
unsigned char CheckPeople;

int main(void){
	Stm32SeniorData SendARM9Data;
	Stm32CheckData CheckDataArm;
	DHT11Data MyDHTData;
	CarControlData MyCarData;
	
//	unsigned char str[AllSenorDataSize];
//	unsigned char string[4] = {0xAA, 0x55, 0x00, 0xFF};
	int timemotor = 1000;
/**
  *���ô��� 
  */
	USART_Configuration();
	NVIC_Configuration();

	//�����־λ�������1λ���ݻᶪʧ
	USART_ClearFlag(USART2,USART_FLAG_TC);

/**
  * С������
  */
//	CarGpioConfig();

/* ������������ */
	ADC1_Init();


/* �����Ӧ */
	LedGpioConfig();
	MEGpioConfig();
	
	memset(&SendARM9Data, 0x0, sizeof(struct SeniorData));
/**
  *���ݽ��յ��û����������Ӧ����Ϊ
  */

for(;;){
	switch(MyUserOrder){
		/* �����Ӧ */
		case ME003:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)){
		    		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
				CheckPeople = 0xFF;
				USART2_Putc(CheckPeople);
				MyUserOrder = NoOrder;}
			else{
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
				CheckPeople = 0x00;
				USART2_Putc(CheckPeople);
				MyUserOrder = NoOrder;
				}
			break;
		/* MQ2 �������� */
		case MQ2:
			//ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3;
			SendARM9Data.FireDataHigh = ADC_ConvertedValue>>8;
			SendARM9Data.FireDataLow = ADC_ConvertedValue & 0x00ff;
			USART2SendtoArm(SendARM9Data);
			MyUserOrder = NoOrder;
			break;
			
		/* ��ʪ������ */
		case ReadHDT11Data:
			if(GetDHT11Data(&MyDHTData)){
				RETERR = HDT11DataError;
				return RETERR;
			};
				SendARM9Data.HumidityHigh = MyDHTData.HumidityDataHigh;
				SendARM9Data.HumidityLow	= MyDHTData.HumidityDataLow;
				SendARM9Data.TemperatureHigh	= MyDHTData.TemperatureDataHigh;
				SendARM9Data.TemperatureLow		= MyDHTData.TemperatureDataLow;
				USART2SendtoArm(SendARM9Data);
				
			#if 0
			convert_DHT(str, MyDHTData);
			USART2_Puts(str);
			#endif
				MyUserOrder = NoOrder;
		break;

		/* С��������� */
		case CarBEFOR:
			MyCarData = BEFOR;
			MoveCar(MyCarData);
			DelayMs(timemotor);
			MoveCar(STOP);
			MyUserOrder = NoOrder;
			break;
		case CarSTOP:
			MyCarData = STOP;
			MoveCar(MyCarData);
			break;
		case CarBACK:
			MyCarData = BACK;
			MoveCar(MyCarData);
			DelayMs(timemotor);
			MoveCar(STOP);
			MyUserOrder = NoOrder;
			break;
		case CarTurnBeforeRight:
			MyCarData = TurnBeforeRight;
			MoveCar(MyCarData);
			DelayMs(timemotor);
			MoveCar(STOP);
			MyUserOrder = NoOrder;
			break;
		case CarTurnBeforeLift:
			MyCarData = TurnBeforeLift;
			MoveCar(MyCarData);
			DelayMs(timemotor);
			MoveCar(STOP);
			MyUserOrder = NoOrder;
			break;
		case CarTurnBackRight:
			MyCarData = TurnBackRight;
			MoveCar(MyCarData);
			DelayMs(timemotor);
			MoveCar(STOP);
			MyUserOrder = NoOrder;
			break;
		case CarTurnBackLift:
			MyCarData = TurnBackLift;
			MoveCar(MyCarData);
			DelayMs(timemotor);
			MoveCar(STOP);
			MyUserOrder = NoOrder;
			break;
		case NoOrder:
			/* ����Ƿ��н���������� 
			 * 1 �Ƿ����ˣ�����
			 * 2 С���Ƿ�Ҫ��ײ������
			 * 3 �Ƿ��ж�����й¶
			 */
			memset(&CheckDataArm, 0x0, sizeof(struct CheckData));
			#if 0
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)){
		    		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
				CheckPeople = 0xFF;
				CheckDataArm.HumanInduction = 0xFF;
			#endif
				
			if((ADC_ConvertedValue > 2048) || (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)) ){
				CheckDataArm.FireDataHigh = ADC_ConvertedValue>>8;
				CheckDataArm.FireDataLow = ADC_ConvertedValue & 0x00ff;
				
				CheckDataArm.HumanInduction = 0xFF;
				/* �������� */
			  USART2ChecktoArm(CheckDataArm);
			}
			break;
		default : 
				MyUserOrder = NoOrder;
			break;
	}

}
}


/*******************************
  * ���Դ���
  */
 /*
for(;;){
	switch(MyUserOrder){
		case 0x55:
			USART2_Puts(string);
			MyUserOrder = NoOrder;
		default : break;
		}
}
*/
