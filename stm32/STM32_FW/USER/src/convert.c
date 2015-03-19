
#include "convert.h"
#include "DataConfig.h"

extern unsigned char UCTestData;

/**
  *	unsigned char HumidityDataHigh;
	unsigned char HumidityDataLow;
	unsigned char TemperatureDataHigh;
	unsigned char TemperatureDataLow;
  */

void convert_DHT(unsigned char * string, DHT11Data SensorData){
	
	/* 1. �����ʪ������ */
	string[0] = SensorData.HumidityDataHigh;
	
	string[1] = SensorData.HumidityDataLow;
	
	string[2] = SensorData.TemperatureDataHigh;
	
	string[3] = SensorData.TemperatureDataLow;
	
	/*���һ���ֽڲ�����λ*/
	
	string[4] = '\0';
}



