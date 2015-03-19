
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
	
	/* 1. 获得温湿度数据 */
	string[0] = SensorData.HumidityDataHigh;
	
	string[1] = SensorData.HumidityDataLow;
	
	string[2] = SensorData.TemperatureDataHigh;
	
	string[3] = SensorData.TemperatureDataLow;
	
	/*最后一个字节补结束位*/
	
	string[4] = '\0';
}



