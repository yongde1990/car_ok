#ifndef __DHT11_H
#define __DHT11_H
/**
  *
  */
 
#include "stm32f10x.h"
#include "delay.h"

/**
  * 管脚输出高低电平
  */

#define HIGH 1
#define LOW 0

#define DHT11Out(x) if(x) \
	GPIO_SetBits(GPIOA, GPIO_Pin_4); \
	else \
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);


/**
  *DHT11数据结构
  *DHT11返回的数据顺序为:
  *8bit湿度整数数据+ 8bit湿度小数数据+ 8bit温度整数数据+ 8bit温度小数数据+ 8bit校验和
  *校验和= "8bit湿度整数数据+ 8bit湿度小数数据+ 8bit温度整数数据+ 8bit温度小数数据" 所得结果的末8位
  */
typedef struct DHTData{
	unsigned char HumidityDataHigh;
	unsigned char HumidityDataLow;
	unsigned char TemperatureDataHigh;
	unsigned char TemperatureDataLow;
	//unsigned char CheckData;
} DHT11Data, *pDHT11Data;

/**
  *DHT11数据传输端口配置函数 
  */
void DHT11GpioOutputConfig(void);
void DHT11GpioInputConfig(void);

/**
  *读取8bit数据
  *返回: 读取8bit 数据结果
  */
unsigned char GetByteData(void);
/**
  * 开始与DHT11通讯函数
  */
void StartDHT(void);
/**
  * 检测DHT11是否开始读数据
  */
void CheckDHT(void);
/**
  *获得DHT11Data的数据
  */
int GetDHT11Data(DHT11Data* pGetAllDHT11Data);

#endif
