/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "DataConfig.h"
//#include "BspGpio.h"
#include "motor.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

extern __IO uint32_t TimingDelay;
void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles USART2 interrupt request.
  * @param  None
  * @retval None
  */

//extern u8 Uart2_Get_Flag;
//extern u8 Uart2_Get_Data[];
extern unsigned char  GetUserOrder;
//extern u8 counter;
extern UserOrder MyUserOrder;


/*
CarBEFOR          			= 0x06,
CarSTOP          			= 0x00,
CarBACK          			= 0x09,
CarTurnBeforeRight          	= 0x04,
CarTurnBeforeLift          	= 0x02,
CarTurnBackRight          	= 0x08,
CarTurnBackLift          	= 0x01,
*/
void USART2_IRQHandler(void)
{
	//接收中断
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		GetUserOrder = USART_ReceiveData(USART2);

		switch (GetUserOrder){
		case 0xF0:
			MyUserOrder = ReadHDT11Data;
			break;
		/*小车*/
		case 0x06:
			MyUserOrder = CarBEFOR;
			break;
		case 0x00:
			MyUserOrder = CarSTOP;
			break;
		case 0x09:
			MyUserOrder = CarBACK;
			break;
		case 0x04:
			MyUserOrder = CarTurnBeforeRight;
			break;
		case 0x02:
			MyUserOrder = CarTurnBeforeLift;
			break;
		case 0x08:
			MyUserOrder = CarTurnBackRight;
			break;
		case 0x01:
			MyUserOrder = CarTurnBackLift;
			break;


		/* MQ2烟雾 */
		case 0xF1:
			MyUserOrder = MQ2;
			break;

		/* 人体感应 */
		case 0xF2:
			MyUserOrder = ME003;
			break;
		//测试
		case 0x55:
			MyUserOrder = Test;
			break;
		/////////////////////////////////////////////////
		default: break;
			}
	}

	
	/*
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Uart2_Get_Data[counter++] = USART_ReceiveData(USART2);
		Uart2_Get_Data[counter]  = '\0';
		if(counter >= 50)
			counter = 0;
		Uart2_Get_Flag  = 1;
	}
	*/
//USART_IT_TXE // 发送中断
// if( USART_GetITStatus(USART2, USART_IT_TXE) != RESET  ) //一个是TXE=发送数据寄存器空，
// {
    //USART_SendData(USART2, 0x23);
//    USART_ClearITPendingBit(USART2,USART_IT_TXE);
//    LED(ON);
// }
// else
// { 
  // 因为是 发送寄存器空 的中断，所以发完字符串后必须关掉，
   // 否则只要空了，就会进中断
//   USART_ITConfig(USART2, USART_IT_TXE, DISABLE);                                        
//  }
 
	//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE);	//读SR
		USART_ReceiveData(USART2);				//读DR
	}
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
