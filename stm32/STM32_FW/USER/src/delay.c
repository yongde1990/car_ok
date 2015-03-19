#include "delay.h"

__IO uint32_t TimingDelay;

/*---------------------------------
函数名：ms延时函数
描 述：参数1即为1ms，1000即为1s;只有几
us的误差;
-----------------------------------*/
void DelayMs(__IO uint32_t nTime)
{
	while(SysTick_Config(SystemCoreClock/1000));
	TimingDelay = nTime;
	while(TimingDelay != 0);
	SysTick->CTRL=0x00; //关闭计数器
	SysTick->VAL =0X00; //清空计数器
}
/*---------------------------------
函数名：us延时函数 SysTick_Handler
描 述：参数1即为1us，1000即为1ms;只有几
us的误差;
-----------------------------------*/
void DelayUs(__IO uint32_t nTime)
{
	while(SysTick_Config(SystemCoreClock/1000000));
	TimingDelay = nTime;
	while(TimingDelay != 0);
	SysTick->CTRL=0x00; //关闭计数器
	SysTick->VAL =0X00; //清空计数器
}

