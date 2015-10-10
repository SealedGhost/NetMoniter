#include "includes.h"
#include "stm32f10x_it.h"
#include "UART.h"
#include "Translate.h"
#include "key.h" 
#include "IWDG.h"
#include "timer.h"
#include "MUSIC.h"
#include "SPI.h"
void BSP_Init(void)
{
	unsigned short int i;
	SystemInit(); /* 配置系统时钟为72M */
	SysTick_init(); /* 初始化并使能SysTick定时器 */
	
	GPIO_Configuration();   
  DMA_Configuration();
	UART_Configuration();
	NVIC_Configuration();   
  USART_Cmd(USART1, ENABLE);	
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
	IWDG_Init(4,625);//与分频数为64,重载值为625,溢出时间为1s
	TIM4_Int_Init();  
	SPI2_Init();
  Music_Init();
	Key_Init();//打开IO时钟
}
 
void SysTick_init(void)
{
	SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);
}















