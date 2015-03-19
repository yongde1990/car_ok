#ifndef __CONVERT_H
#define __CONVERT_H
/**
  *
  */

#include "DataConfig.h"

void convert(unsigned char * string, PT_AllSensorData SensorData);
void convert_DHT(unsigned char * string, DHT11Data SensorData);

#endif

