#include "DHT11.h"
#include "DataConfig.h"

/**
  * @brief  ����GPIO PA4
  * @param  ��
  * @retval ��
  */


extern unsigned char UCTestData;

void DHT11GpioPA4InputConfig(void){
	/*����GPIO_InitTypeDef �ṹ��*/
	GPIO_InitTypeDef MyDHT11GpioInit;
	/*��GPIOA����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*����GPIO_InitTypeDef �ṹ��*/
	MyDHT11GpioInit.GPIO_Pin = GPIO_Pin_4;
	MyDHT11GpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	MyDHT11GpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	/*����GPIOA*/
	GPIO_Init(GPIOA, &MyDHT11GpioInit);
}

void DHT11GpioPA4OutputConfig(void){
	/*����GPIO_InitTypeDef �ṹ��*/
	GPIO_InitTypeDef MyDHT11GpioInit;
	/*��GPIOA����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*����GPIO_InitTypeDef �ṹ��*/
	MyDHT11GpioInit.GPIO_Pin = GPIO_Pin_4;
	MyDHT11GpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	MyDHT11GpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	/*����GPIOA*/
	GPIO_Init(GPIOA, &MyDHT11GpioInit);
}

void StartDHT(void){
	/* 1. ��������18ms  */
	DHT11GpioPA4OutputConfig();
	DHT11Out(LOW);
	DelayMs(20);
	/* 2. ����20 ~ 40us  */
	DHT11Out(HIGH);
	DelayUs(40);
}

void CheckDHT(void){
	unsigned int FlagOutTime = 2;
	/* PA4 ����Ϊ����ģʽ */
	//DHT11GpioPA4InputConfig();
	FlagOutTime = 2;
	while((!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))&&FlagOutTime++);
	FlagOutTime = 2;
	while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))&&FlagOutTime++);
	/* ׼�������� */
}	
unsigned char GetByteData(void){
	unsigned char OneByteData = 0x00;
	unsigned char  HighOrLow = 0;
	unsigned char i = 0;
	unsigned int FlagOutTime = 0;
	/* ����Э�鿪ʼ��һ���ֽڵ����� */
	for(i=0; i < 8; i++){
		FlagOutTime = 2;
		while((!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))&&(FlagOutTime++));
		DelayUs(30);
		HighOrLow = 0;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
			HighOrLow = 1;
		FlagOutTime = 2;
		while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))&&(FlagOutTime++));
		if(FlagOutTime == 1) break;

		OneByteData<<=1;
		OneByteData |= HighOrLow;		
	}
	return OneByteData;
}
/**
  *��������: �õ�DHT11 ���¶ȡ�ʪ��ֵ
  *����ֵ: 0 �õ���ʪ��ֵ, 1û�еõ���ʪ��ֵ
  */
int GetDHT11Data(DHT11Data* pGetAllDHT11Data){
	unsigned char CheckDataTemp = 0;
	unsigned char HumidityDataHigh = 0;
	unsigned char HumidityDataLow = 0;
	unsigned char TemperatureDataHigh = 0;
	unsigned char TemperatureDataLow = 0;
	unsigned char CheckData = 0;
	
	/* 1. ��ʼ��DHT11 ͨѶ */
	StartDHT();
	
	DHT11GpioPA4InputConfig();
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)){
		
	/* 2. ���DHT11 �Ƿ�Ҫ�������� */
		CheckDHT();
		
	/* 3. �������� */
		HumidityDataHigh = GetByteData();
		HumidityDataLow = GetByteData();
		TemperatureDataHigh = GetByteData();
		TemperatureDataLow = GetByteData();
		CheckData = GetByteData();
		
	}
	
	DHT11GpioPA4InputConfig();
	/* ��������Ƿ���ȷ */
	CheckDataTemp = (HumidityDataHigh + HumidityDataLow + TemperatureDataHigh + TemperatureDataLow);
	
	if(CheckDataTemp == CheckData){
		
		pGetAllDHT11Data->HumidityDataHigh = HumidityDataHigh;
		pGetAllDHT11Data->HumidityDataLow = HumidityDataLow;
		pGetAllDHT11Data->TemperatureDataHigh = TemperatureDataHigh;
		pGetAllDHT11Data->TemperatureDataLow = TemperatureDataLow;
		
		return 0;
	}
	else{	
		return 1;
	}
}

