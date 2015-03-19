#include "DHT11.h"
#include "DataConfig.h"

/**
  * @brief  配置GPIO PA4
  * @param  无
  * @retval 无
  */


extern unsigned char UCTestData;

void DHT11GpioPA4InputConfig(void){
	/*分配GPIO_InitTypeDef 结构体*/
	GPIO_InitTypeDef MyDHT11GpioInit;
	/*打开GPIOA外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*设置GPIO_InitTypeDef 结构体*/
	MyDHT11GpioInit.GPIO_Pin = GPIO_Pin_4;
	MyDHT11GpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	MyDHT11GpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	/*配置GPIOA*/
	GPIO_Init(GPIOA, &MyDHT11GpioInit);
}

void DHT11GpioPA4OutputConfig(void){
	/*分配GPIO_InitTypeDef 结构体*/
	GPIO_InitTypeDef MyDHT11GpioInit;
	/*打开GPIOA外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*设置GPIO_InitTypeDef 结构体*/
	MyDHT11GpioInit.GPIO_Pin = GPIO_Pin_4;
	MyDHT11GpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	MyDHT11GpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	/*配置GPIOA*/
	GPIO_Init(GPIOA, &MyDHT11GpioInit);
}

void StartDHT(void){
	/* 1. 拉低至少18ms  */
	DHT11GpioPA4OutputConfig();
	DHT11Out(LOW);
	DelayMs(20);
	/* 2. 拉高20 ~ 40us  */
	DHT11Out(HIGH);
	DelayUs(40);
}

void CheckDHT(void){
	unsigned int FlagOutTime = 2;
	/* PA4 配置为输入模式 */
	//DHT11GpioPA4InputConfig();
	FlagOutTime = 2;
	while((!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))&&FlagOutTime++);
	FlagOutTime = 2;
	while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))&&FlagOutTime++);
	/* 准备接数据 */
}	
unsigned char GetByteData(void){
	unsigned char OneByteData = 0x00;
	unsigned char  HighOrLow = 0;
	unsigned char i = 0;
	unsigned int FlagOutTime = 0;
	/* 根据协议开始读一个字节的数据 */
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
  *函数功能: 得到DHT11 的温度、湿度值
  *返回值: 0 得到温湿度值, 1没有得到温湿度值
  */
int GetDHT11Data(DHT11Data* pGetAllDHT11Data){
	unsigned char CheckDataTemp = 0;
	unsigned char HumidityDataHigh = 0;
	unsigned char HumidityDataLow = 0;
	unsigned char TemperatureDataHigh = 0;
	unsigned char TemperatureDataLow = 0;
	unsigned char CheckData = 0;
	
	/* 1. 开始与DHT11 通讯 */
	StartDHT();
	
	DHT11GpioPA4InputConfig();
	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)){
		
	/* 2. 检测DHT11 是否要发送数据 */
		CheckDHT();
		
	/* 3. 接收数据 */
		HumidityDataHigh = GetByteData();
		HumidityDataLow = GetByteData();
		TemperatureDataHigh = GetByteData();
		TemperatureDataLow = GetByteData();
		CheckData = GetByteData();
		
	}
	
	DHT11GpioPA4InputConfig();
	/* 检测数据是否正确 */
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

