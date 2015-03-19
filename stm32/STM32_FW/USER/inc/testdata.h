#include "stm32f10x.h"
#include "BspGpio.h"
#include "delay.h"
#include "DHT11.h"
#include "myuart.h"
#include "DataConfig.h"

#include "convert.h"

#ifndef __TESTDATA_H
#define __TESTDATA_H

unsigned char UCTestData;
struct DHTData TestDHTData = {
	0xff,0xff,0xff,0xff
};

#endif
