#ifndef _CONFIG_H_
#define _CONFIG_H_

typedef struct SeniorData{
	unsigned char FireDataHigh;
	unsigned char FireDataLow;
	
	unsigned char HumidityHigh;
	unsigned char HumidityLow;
	unsigned char TemperatureHigh;
	unsigned char TemperatureLow;
	
	unsigned char HumanInduction;
	
	unsigned char DistanceMeasurement;
}Stm32SeniorData, *PStm32SeniorData;

#define CmdStm32ReadData 0xF0

#endif
