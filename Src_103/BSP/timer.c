#include "timer.h"
#include "stm32f10x_tim.h"
#include <stm32f10x.h>
#include "UART.h"
#include "misc.h"
#include "IWDG.h"
//定时器4中断服务程序	 
void TIM4_IRQHandler(void)
{ 		 
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  ); 
		//printf("%c",'a');			
			IWDG_Feed();	
  }   

		
}
// void TIM1_UP_IRQHandler(void)
// { 		 
// 	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
// 	{
// 		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  ); 
// 		printf("%c",'B');			    				   				     	    	
//  	}				    	    
// }
//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM4_Int_Init(void)
{	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  NVIC_InitTypeDef         NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		
    TIM_TimeBaseStructure.TIM_Period=4999;//设置自动重装载寄存器周期值		 								
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;//设置时钟频率除数预分频值10KHz计数，计数到5000为500ms				    
    //Tout=((4999+1)*(7199+1))/72=500000us=500ms
	  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
//		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //TIM3
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_Cmd(TIM4, ENABLE);	
}
// void TIM1_Int_Init(void)
// {	
// 		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	  NVIC_InitTypeDef         NVIC_InitStructure;
// 		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
// 		
//     TIM_TimeBaseStructure.TIM_Period=7199;		 								
//     TIM_TimeBaseStructure.TIM_Prescaler= 4999;			    
//     TIM_TimeBaseStructure.TIM_ClockDivision=0; 		
//     TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
// 		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//     TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
// 	  TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );
// 	  TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM1中断,允许更新中断

// 		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM3
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
// 		NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
// 		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
// 		NVIC_Init(&NVIC_InitStructure);
// 		
// 		TIM_Cmd(TIM1, ENABLE);	
// }














