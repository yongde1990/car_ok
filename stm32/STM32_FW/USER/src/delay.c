#include "delay.h"

__IO uint32_t TimingDelay;

/*---------------------------------
��������ms��ʱ����
�� ��������1��Ϊ1ms��1000��Ϊ1s;ֻ�м�
us�����;
-----------------------------------*/
void DelayMs(__IO uint32_t nTime)
{
	while(SysTick_Config(SystemCoreClock/1000));
	TimingDelay = nTime;
	while(TimingDelay != 0);
	SysTick->CTRL=0x00; //�رռ�����
	SysTick->VAL =0X00; //��ռ�����
}
/*---------------------------------
��������us��ʱ���� SysTick_Handler
�� ��������1��Ϊ1us��1000��Ϊ1ms;ֻ�м�
us�����;
-----------------------------------*/
void DelayUs(__IO uint32_t nTime)
{
	while(SysTick_Config(SystemCoreClock/1000000));
	TimingDelay = nTime;
	while(TimingDelay != 0);
	SysTick->CTRL=0x00; //�رռ�����
	SysTick->VAL =0X00; //��ռ�����
}

