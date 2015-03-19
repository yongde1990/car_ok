#ifndef __DHT11_H
#define __DHT11_H
/**
  *
  */
 
#include "stm32f10x.h"
#include "delay.h"

/**
  * �ܽ�����ߵ͵�ƽ
  */

#define HIGH 1
#define LOW 0

#define DHT11Out(x) if(x) \
	GPIO_SetBits(GPIOA, GPIO_Pin_4); \
	else \
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);


/**
  *DHT11���ݽṹ
  *DHT11���ص�����˳��Ϊ:
  *8bitʪ����������+ 8bitʪ��С������+ 8bit�¶���������+ 8bit�¶�С������+ 8bitУ���
  *У���= "8bitʪ����������+ 8bitʪ��С������+ 8bit�¶���������+ 8bit�¶�С������" ���ý����ĩ8λ
  */
typedef struct DHTData{
	unsigned char HumidityDataHigh;
	unsigned char HumidityDataLow;
	unsigned char TemperatureDataHigh;
	unsigned char TemperatureDataLow;
	//unsigned char CheckData;
} DHT11Data, *pDHT11Data;

/**
  *DHT11���ݴ���˿����ú��� 
  */
void DHT11GpioOutputConfig(void);
void DHT11GpioInputConfig(void);

/**
  *��ȡ8bit����
  *����: ��ȡ8bit ���ݽ��
  */
unsigned char GetByteData(void);
/**
  * ��ʼ��DHT11ͨѶ����
  */
void StartDHT(void);
/**
  * ���DHT11�Ƿ�ʼ������
  */
void CheckDHT(void);
/**
  *���DHT11Data������
  */
int GetDHT11Data(DHT11Data* pGetAllDHT11Data);

#endif
