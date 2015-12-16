#include "UART.h"
#include "Translate.h"
#include "misc.h"
#include "ucos_ii.h"
#include "app.h"

uint8_t Uart_Rx[UART_RX_LEN];//UART2接收


void GPIO_Configuration(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);//打开串口1和对应IO时钟      
  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//UART1重映射到PB6，7脚使能
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; /* Configure USART1 Rx (PB.7) as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  /* Configure USART1 Tx (PB.6) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
////////////////////////////////////////////////////////////////////////////////////////////////	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; /* Configure USART2 Rx (PA.3) as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  /* Configure USART2 Tx (PA.2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
}

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //启动DMA时钟  

    DMA_DeInit(DMA1_Channel6); 	//DMA1通道5配置   
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);//外设地址  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart_Rx;     //内存地址  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma传输方向从外设到内存 
    DMA_InitStructure.DMA_BufferSize = UART_RX_LEN;               //设置DMA在传输时缓冲区的长度  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设地址不变 
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //设置DMA的内存递增模式  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长 8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //内存数据字长8位   
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //设置DMA的传输模式   
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //设置DMA的优先级别 最高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //设置DMA的2个memory中的变量互相访问  
    DMA_Init(DMA1_Channel6,&DMA_InitStructure);  
  
    DMA_Cmd(DMA1_Channel6,ENABLE);     //使能通道5  
}

void UART_Configuration(void)
{
	   USART_InitTypeDef USART_InitStructure;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//开启USART2时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
   
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
   USART_InitStructure.USART_StopBits = USART_StopBits_1;    
   USART_InitStructure.USART_Parity = USART_Parity_No;    
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
//   USART_InitStructure.USART_Mode =  USART_Mode_Rx |USART_Mode_Tx;      
   USART_InitStructure.USART_Mode = USART_Mode_Tx;   
   USART_InitStructure.USART_BaudRate = 115200;   

   USART_Init(USART1,&USART_InitStructure);          

   USART_ITConfig(USART1,USART_IT_TC,DISABLE);    //禁止 
   USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);  //禁止 
   USART_ITConfig(USART1,USART_IT_IDLE,DISABLE);   //


    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode =  USART_Mode_Rx;      
    USART_InitStructure.USART_BaudRate = 38400;   
 
    USART_Init(USART2,&USART_InitStructure);          

    USART_ITConfig(USART2,USART_IT_TC,DISABLE);    //禁止 
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);  //禁止 
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);   //开启

    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);	//使能UART2采用DMA方式发送


}
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //配置UART2中断    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;               //通道设置为串口2中断    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       //中断占先等级0    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //中断响应优先级0    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断    
    NVIC_Init(&NVIC_InitStructure); 

 
}


void USART2_IRQHandler(void)                                 
{  
	uint32_t Length = 0;//,i=0;

	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
	{  
		//flag_translate=1;
		  DMA_Cmd(DMA1_Channel6,DISABLE); 
		  Length = USART2->SR;  
 		 Length = USART2->DR; //清USART_IT_IDLE标志 
 			Length = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel6);		
    
    if(Uart_Rx[1] == 'A'  &&  Uart_Rx[5] == 'M')
    {
      if(Length > 45  &&  Length < 80)
      {		
        OSQPost(QSem,(void *)Uart_Rx);
      }
    }
    else if(Uart_Rx[3] == 'R'  &&  Uart_Rx[4] == 'M'  &&  Uart_Rx[5] == 'C')
    {
      OSQPost(QSem, (void*)Uart_Rx);
    }
			//printf("\r\n%d",Length);	
		 	DMA1_Channel6->CNDTR = UART_RX_LEN;//重装填,并让接收地址偏址从0开始
		  DMA_Cmd(DMA1_Channel6, ENABLE);//处理完,重开DMA 
  }
  __nop();
}

void  Putc_UART1(u8 ch)
{
	USART_SendData(USART1, ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
}
// void  Putc_UART3(u8 ch)
// {
// 	USART_SendData(USART3, ch);
// 	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
// }
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
