#ifndef __MYUART_H
#define __MYUART_H


#include "stm32f10x.h"
#include "DataConfig.h"

/*  串口接收到最大数据 */
#define MaxSize 50

extern u8 Uart2_Get_Flag;
extern u8 Uart2_Get_Data[];

void USART_Configuration(void);
void NVIC_Configuration(void);
void USART2_Putc(u8 c);
void USART2_Puts(unsigned char * str);

void USART2SendtoArm(struct SeniorData senddata);
void USART2ChecktoArm(struct CheckData checkdata);

#endif

