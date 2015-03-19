/**
  ******************************************************************************
  * @file    myuart.c
  * @author  
  * @version V1.0
  * @date    
  * @brief   ���ڲ�������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:stm32f103c8t6��Сϵͳ��
  * PA2: USART2_TX
  * PA3: USART2_RX
  *
  ******************************************************************************
  */

#include "myuart.h"

u8 Uart2_Get_Flag;	//����2���յ����ݱ�־
u8 Uart2_Get_Data[MaxSize];	//����2���յ�����
unsigned char  GetUserOrder;

void USART_Configuration(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	USART_InitTypeDef 		USART_InitStructure;
	USART_ClockInitTypeDef 	USART_ClockInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	//ʹ�ܴ���2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	// A2 ��T2X
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// A3 ��R2X 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	USART_ClockInit(USART2, &USART_ClockInitStructure);
	
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE);
	//����2ʹ�ý����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/*UART2*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_Putc(u8 c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void USART2_Puts(unsigned char * str)
{
	unsigned char i = 0;
	while(str[i]){
		USART_SendData(USART2, str[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		i++;
	}
	
	/*
	unsigned char i = 0;
	for(i = 0; i < (AllSenorDataSize-1); i++){
		USART_SendData(USART2, str[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
	*/
}
/* �����ض����� */
void USART2SendtoArm(struct SeniorData senddata)
{
	USART_SendData(USART2, senddata.FireDataHigh);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, senddata.FireDataLow);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART2, senddata.HumidityHigh);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, senddata.HumidityLow);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART2, senddata.TemperatureHigh);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, senddata.TemperatureLow);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART2, senddata.HumanInduction);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, senddata.DistanceMeasurement);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/* ���ͽ����¼����� */
void USART2ChecktoArm(struct CheckData checkdata)
{
	USART_SendData(USART2, checkdata.FireDataHigh);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, checkdata.FireDataLow);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, checkdata.FlagH);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, checkdata.FlagHH);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART2, checkdata.FlagL);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, checkdata.FlagLL);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART2, checkdata.HumanInduction);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, checkdata.DistanceMeasurement);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}



