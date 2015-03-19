#ifndef __DATACONFIG_H
#define __DATACONFIG_H

#include "DHT11.h"
//#include "myuart.h"

#define  AllSenorDataSize DHT11DATASIZE+1
#define DHT11DATASIZE 4

/* 调试 */
#if 0
	#define Debug_Uart  USART2_Puts("uart ok!")
	#define Debug_FF  USART2_Putc(0xff)
	#define Debug_DHT  USART2_Puts(str)
	#define Debug_Get  USART2_Putc(MyUserOrder)
	#define Debug_OneUCData(x)  USART2_Putc(x)
#else
	#define Debug_Uart 
#endif

/* 错误类型数据 */
typedef enum ERRORDATA{
	HDT11DataError = 100
} ReturnError;

/* 命令类型数据 */
typedef enum USERORDER{
	ReadHDT11Data 			= 0xF0,
	
	CarBEFOR          			= 0x06,
	CarSTOP          			= 0x00,
	CarBACK          			= 0x09,
	CarTurnBeforeRight          	= 0x04,
	CarTurnBeforeLift          	= 0x02,
	CarTurnBackRight          	= 0x08,
	CarTurnBackLift          	= 0x01,

	MQ2					= 0xF1,

	ME003					= 0xF2,
	
	NoOrder 				= 0xFF,

	//测试
	Test = 0x55
} UserOrder;


typedef struct Data{
	DHT11Data HTData; //从DHT11 得到的温度湿度数据
} T_AllSensorData, *PT_AllSensorData;



typedef struct SeniorData{
	unsigned char FireDataHigh;
	unsigned char FireDataLow;
	
	unsigned char HumidityHigh;
	unsigned char HumidityLow;
	unsigned char TemperatureHigh;
	unsigned char TemperatureLow;
	
	unsigned char HumanInduction;
	
	unsigned char DistanceMeasurement;
}Stm32SeniorData;

typedef struct CheckData{
	unsigned char FireDataHigh;
	unsigned char FireDataLow;
	
	unsigned char FlagH;
	unsigned char FlagHH;
	unsigned char FlagL;
	unsigned char FlagLL;
	
	unsigned char HumanInduction;
	
	unsigned char DistanceMeasurement;
}Stm32CheckData;

#endif

