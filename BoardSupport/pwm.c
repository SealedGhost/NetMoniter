#include "lpc177x_8x_pwm.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_adc.h"
#include "lpc177x_8x_pinsel.h"
#include "pwm.h"

#define _BACK_LIGHT_BASE_CLK (1000/4)
	uint32_t pclk;
 
 
void lpc1788_PWM_Init(void)//PWM初始化函数
{
//	uint32_t pclk;
  PWM_TIMERCFG_Type PWMCfgDat;
  PWM_MATCHCFG_Type PWMMatchCfgDat;
	
  PWMCfgDat.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;/* 直接使用分频值(另外还可以使用微秒值) */
  PWMCfgDat.PrescaleValue = 1;  //频率=fpclk
  PWM_Init(1, PWM_MODE_TIMER, (void *) &PWMCfgDat);/* PWM1，使用定时模式（另外还可以使用计数模式） */		
	PINSEL_ConfigPin(2,1,1);/*配置PWM引脚 P2.1: PWM1_2*/
	PWM_ChannelConfig(1, 2, PWM_CHANNEL_SINGLE_EDGE);	/* 配置PWM1通道2输出，单边沿控制模式 */

  pclk = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER);	/* 配置PWM1通道0，控制周期 */
  PWM_MatchUpdate(1, 0, pclk/_BACK_LIGHT_BASE_CLK, PWM_MATCH_UPDATE_NOW);
  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 0;
  PWMMatchCfgDat.ResetOnMatch = ENABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;
  PWM_ConfigMatch(1, &PWMMatchCfgDat);

  PWM_MatchUpdate(1, 2, (pclk/_BACK_LIGHT_BASE_CLK)/2, PWM_MATCH_UPDATE_NOW);	/* 配置PWM1通道2，控制占空比, 这里设为50% */
  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 2;
  PWMMatchCfgDat.ResetOnMatch = DISABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;
  PWM_ConfigMatch(1, &PWMMatchCfgDat);

  PWM_ChannelCmd(1, 2, ENABLE);  /* 使能PWM1通道2输出 */
  PWM_ResetCounter(1);/* 复位PWM1计数器 */
  PWM_CounterCmd(1, ENABLE);/* PWM1计数使能 */
  PWM_Cmd(1, ENABLE);  /* PWM模式使能 */
}
void SetBackLight(uint32_t level)//设置PWM占空比，调节LCD背光亮度
{ 
  PWM_MATCHCFG_Type PWMMatchCfgDat;
  PWM_MatchUpdate(1, 2, level, PWM_MATCH_UPDATE_NOW);	/* 配置PWM1通道2，控制占空比 */
  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 2;
  PWMMatchCfgDat.ResetOnMatch = DISABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;
  PWM_ConfigMatch(1, &PWMMatchCfgDat);
}

void PWM_SET(uint8_t PWM)//PWM_test
{
	  uint32_t BACK_LIGHT;
	  BACK_LIGHT=pclk/_BACK_LIGHT_BASE_CLK/100;
//    printf("\r\nBACK_LIGHT:%d\n\r",PWM);
		SetBackLight(BACK_LIGHT*PWM);
  switch(PWM)
  {
     case 1:
          SetBackLight(BACK_LIGHT*2);
          break;
     case 2:
          SetBackLight(BACK_LIGHT*20);
          break;
     case 3:
          SetBackLight(BACK_LIGHT*30);
          break;
     case 4:
          SetBackLight(BACK_LIGHT*50);
          break;
     case 5:
          SetBackLight(BACK_LIGHT*70);
          break;
     case 6:
          SetBackLight(BACK_LIGHT*90);
          break;
  }
}



/************************************* End *************************************/
