#include "key.h" 
#include "UART.h"
static GPIO_InitTypeDef GPIO_InitStructure;//由于太多地方使用了这个，只好使用全局了

void Key_Init(void)//配置按键用到的I/O口
{ 	/*开启按键端口（PB0）的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);														 
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**第一行是否有按键按下判断**/
uint32_t Key_1Line_Scan(void)
{
	uint32_t Key_IO = 0x0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推免输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//输出为0
	
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5));
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))<<1;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))<<2;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))<<3;
	return Key_IO;
}
 
/**第二行是否有按键按下判断**/
uint32_t Key_2Line_Scan(void)
{
	uint32_t Key_IO = 0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))<<4;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))<<5;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))<<6;
  Key_IO |= (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))<<7;
	return Key_IO;
}

/**第三行是否有按键按下判断**/
uint32_t Key_3Line_Scan(void)
{
	uint32_t Key_IO = 0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))<<8;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))<<9;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))<<10;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))<<11;
	return Key_IO;
}

/**第四行是否有按键按下判断**/
uint32_t Key_4Line_Scan(void)
{
	uint32_t Key_IO = 0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))<<12;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))<<13;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))<<14;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))<<15;
	return Key_IO;
}

/**第五行是否有按键按下判断**/
uint32_t Key_5Line_Scan(void)
{
	uint32_t Key_IO = 0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))<<16;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))<<17;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))<<18;
	Key_IO |= (!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))<<19;
	return Key_IO;
}

uint32_t Get_KeyValue(void)
{
	u32 Key_L=0;//,Key_L1=0,Key_L2=0,Key_L3=0,Key_L4=0,Key_L5=0;	
	Key_L |= Key_1Line_Scan();	//普通键扫描
	Key_L |= Key_2Line_Scan();
	Key_L |= Key_3Line_Scan();
	Key_L |= Key_4Line_Scan();
	Key_L |= Key_5Line_Scan();
	return Key_L;
}


