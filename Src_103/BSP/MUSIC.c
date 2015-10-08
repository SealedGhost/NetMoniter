#include "MUSIC.h"
#include "STM32F10x_GPIO.H"
static void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //????
      while(i--) ;    
   }
}

static void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //????
      while(i--) ;    
   }
}
void Music_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; /* Configure USART1 Rx (PA.8) as “Ù¿÷–≈∫≈1 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  /* Configure USART1 Tx (PA.09) as “Ù¿÷–≈∫≈2 */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_9);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  /* Configure USART1 Tx (PA.10) as “Ù¿÷–≈∫≈3 */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_10);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  /* Configure USART1 Tx (PA.11) as “Ù¿÷–≈∫≈4 */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_11);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  /* Configure USART1 Tx (PA.12) as “Ù¿÷–≈∫≈5*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_12);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  /* Configure USART1 Tx (PC.13) as “Ù¿÷–≈∫≈6*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC,GPIO_Pin_13);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;  /* Configure USART1 Tx (PC.14) as “Ù¿÷–≈∫≈-*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_SetBits(GPIOC,GPIO_Pin_14);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  /* Configure USART1 Tx (PC.15) as “Ù¿÷–≈∫≈+ */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOC,GPIO_Pin_15);
}
void Sound_D(void)
{
	u32 i=0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);
	for(i=0;i<1000000;i++);
	GPIO_SetBits(GPIOC,GPIO_Pin_14);
	for(i=0;i<1000000;i++);
}
void Sound_U(void)
{
	u32 i=0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_15);
	for(i=0;i<1000000;i++);
	GPIO_SetBits(GPIOC,GPIO_Pin_15);
	for(i=0;i<1000000;i++);
}
void Send_Music_Data(u8 data)
{
	u8 i=0;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	delay_ms(5);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	for(i=0;i<8;i++)
	{

		if(data&1)
		{	
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			delay_us(600);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			delay_us(200);

    }
		else 
		{	
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			delay_us(200);
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			delay_us(600);

    }
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
		data>>=1;
  }
	//GPIO_SetBits(GPIOB,GPIO_Pin_8);	
}

















