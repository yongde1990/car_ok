#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

/***********************************************
 *�� ��������1��Ϊ1ms��1000��Ϊ1s;ֻ�м�
 *us�����;
 ***********************************************
 */
void DelayMs(__IO uint32_t nTime);

/***********************************************
 *�� ��������1��Ϊ1us��1000��Ϊ1ms;ֻ�м�
 *us�����;
 ***********************************************
 */
void DelayUs(__IO uint32_t nTime);


#endif