#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpdma.h"
#include "lpc177x_8x_uart.h"
#include "timer.h"

void lpc1788_TIMER_Init(void)//TIMER定时器初始化函数
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_MATCHCFG_Type MatchConfigStruct;

	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL; 	/* 初始化timer2, 预分频计数时间为1000000uS = 1S */
	TIM_ConfigStruct.PrescaleValue	=400; //设置计数时钟分频，0.4毫秒计数一次
	TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &TIM_ConfigStruct);

	MatchConfigStruct.MatchChannel = 0; //使用通道0
	MatchConfigStruct.IntOnMatch = ENABLE; //MR0与TC值匹配时产生中断
	MatchConfigStruct.ResetOnMatch = ENABLE; //MR0与TC值的匹配时TC复位
	MatchConfigStruct.StopOnMatch = DISABLE;
	MatchConfigStruct.ExtMatchOutputType = 0;
	MatchConfigStruct.MatchValue =1; //0.4毫秒匹配中断
	TIM_ConfigMatch(LPC_TIM2, &MatchConfigStruct);

	TIM_ResetCounter(LPC_TIM2);	/* 定时器计数复位 */

	NVIC_SetPriority(TIMER2_IRQn, ((0x01<<3)|0x01));/* 设置抢占优先级preemption为1, 子优先级sub-priority为1 */
	NVIC_EnableIRQ(TIMER2_IRQn);	/* 使能TIMER2中断 */
	TIM_Cmd(LPC_TIM2, ENABLE);/* 启动定时器 */
}

