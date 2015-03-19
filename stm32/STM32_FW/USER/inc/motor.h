#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
//#include "delay.h"

/**
  *               Befor:前进Stop:停止 Back:后退TurnBeforeRight:右前转TurnBeforeLift: 左前转TurnBackRight:右后转TurnBackLift: 左后转
  *
  *
  * N1N2N3N4: 0110            0000           1001                   0100                                   0010                             1000                                        0001
  */
#define BEFOR  			0x0110
#define STOP    			0x0000
#define BACK	   			0x1001
#define TurnBeforeRight  	0x0100
#define TurnBeforeLift		0x0010
#define TurnBackRight	0x1000
#define TurnBackLift  		0x0001

typedef unsigned int CarControlData;
void MoveCar(CarControlData data);

void CarGpioConfig(void);




#endif
