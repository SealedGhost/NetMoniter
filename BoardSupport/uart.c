#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_gpdma.h"
#include "uart.h"
#include "timer.h"
#include "dma.h"
#include "GUI.h"
#include "dlg.h"
#include "MainTask.h" 
#include <ucos_ii.h>
#include "sound.h"
uint8_t Buffer0[23]  = "uart0 init success\n\r";
uint8_t Buffer2[23]  = "uart2 init success!\n\r"; 

extern uint8_t SND[4][6];
extern int isKeyTrigged;

volatile Bool Doubleclick  = FALSE;
volatile Bool isReleasedDet  = FALSE;
// __IO uint8_t UART_RX[128];//DMA_UART的串口接收缓冲区
//  OS_EVENT *QSem;//定义消息队列指针
//  OS_MEM   *PartitionPt;
//extern void *MsgQeueTb[10];//定义消息指针数组，队列长度为10
//extern uint8_t Partition[10][128];//[SIZE];
void lpc1788_Uart_Init(int port)//LPC1788_Uart_Init
{
   UART_CFG_Type UARTConfigStruct;	/* UART配置 */
   UART_FIFO_CFG_Type UARTFIFOConfigStruct;	/* UART FIFO配置 */
   switch(port)
   {
    case 0:
       PINSEL_ConfigPin(0,2,1);
       PINSEL_ConfigPin(0,3,1);
       break; 
	}
	UART_ConfigStructInit(&UARTConfigStruct);/* 初始化UARTConfigStruct结构体为：波特率为115200，8位数据，无奇偶校验，1位停止位 */
	//UARTConfigStruct.Baud_rate = 115200;
	UART_Init((UART_ID_Type)port, &UARTConfigStruct);	/* 用UARTConfigStruct给定的参数初始化UART外设 */
	//UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	/* 初始化FIFOConfigStruct为默认值:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	UART_FIFOConfig((UART_ID_Type)port, &UARTFIFOConfigStruct);	/* 为UART外设初始化FIFO */
//	UARTFIFOConfigStruct.FIFO_DMAMode = ENABLE;//为UART使能DMA
	UART_TxCmd((UART_ID_Type)port, ENABLE);/* 使能UART发送 */
	
		UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
		UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);
		NVIC_EnableIRQ(UART0_IRQn);
// 	UARTFIFOConfigStruct.FIFO_DMAMode = ENABLE;//为UART使能DMA
// 	UART_FIFOConfig(UART_0, &UARTFIFOConfigStruct);// 为UART0外设初始化FIFO

// 	UART_TxCmd((UART_ID_Type)port, ENABLE);/* 使能UART发送 */
// 	
// 	UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
// 	UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);

// 	NVIC_DisableIRQ(UART0_IRQn);
}

 void UART_Config(unsigned char channel)
{
   UART_CFG_Type UARTConfigStruct1;
   UART_FIFO_CFG_Type UARTFIFOConfigStruct1;
   PINSEL_ConfigPin(0,10,1);
   PINSEL_ConfigPin(0,11,1);
   UART_ConfigStructInit(&UARTConfigStruct1);
   UARTConfigStruct1.Baud_rate = 9600;
   UART_Init((UART_ID_Type)channel, &UARTConfigStruct1);
   
   UART_FIFOConfigStructInit(&UARTFIFOConfigStruct1);
   UARTFIFOConfigStruct1.FIFO_DMAMode = DISABLE;
   UART_FIFOConfig((UART_ID_Type)channel, &UARTFIFOConfigStruct1);
   UART_TxCmd((UART_ID_Type)channel, ENABLE);
}


void USER_Init(void)
{	
	GPDMA_Init();
	lpc1788_Uart_Init(0);/* 初始化串口Uart0 使用DMA通道0*/	
 UART_Config(2);
 //lpc1788_DMA_Init();  
//	printf("uart0 init success!\n\r\r");	
	UART_Send((UART_ID_Type)0, Buffer0,sizeof(Buffer0),BLOCKING);

     
//	UART_Send((UART_ID_Type)2,Buffer2,sizeof(Buffer2),BLOCKING);///* 打印一串字符到串口Uart2 */
////	lpc1788_TIMER_Init();
}

void UART0_IRQHandler(void)
{
	uint8_t tmpc;
	uint32_t  tmp, tmp1;
	tmp = ((LPC_UART0->IIR) & UART_IIR_BITMASK) & UART_IIR_INTID_MASK;
	if (tmp == UART_IIR_INTID_RLS)	// Receive Line Status
	{
			tmp1 = UART_GetLineStatus(UART_0);// Check line status
			tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);// Mask out the Receive Ready and Transmit Holding empty status
	}
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))	// Receive Data Available or Character time-out
		{	
 		  UART_Receive(UART_0, &tmpc, 1, NONE_BLOCKING);
    if(tmpc >= GUI_KEY_MENU  &&  tmpc <= GUI_KEY_PGDOWN)   
			 {
//SND_Stop();
//          SND_SelectID(0);
          isKeyTrigged  = 1;
          switch(tmpc)
          {
            case GUI_KEY_PWM_INC:
              WM_SendMessageNoPara(WM_GetClientWindow(subWins[3]), GUI_KEY_PWM_INC);
              break;								
            case GUI_KEY_PWM_DEC:
              WM_SendMessageNoPara(WM_GetClientWindow(subWins[3]), GUI_KEY_PWM_DEC);
              break;					 
            case GUI_KEY_TRACE_ENABLE:
              break;
            case GUI_KEY_TRACE_DISABLE:
              break;
            default:
               GUI_StoreKeyMsg(tmpc, 1);
              break;
          }
          
			 }   
			 else if(tmpc >= 0x80)
			 {
       GUI_StoreKeyMsg(GUI_KEY_RELEASE, 1);
//       SND_Stop();
			 }
			 else
			 {
//SND_Stop();       
			 }
   }
}



PUTCHAR_PROTOTYPE//重定向C printf函数到Uart0
{
	UART_Send((UART_ID_Type)UART_0, (uint8_t*) &ch, 1, BLOCKING);  /* 发送一个字符到UART */
	while (UART_CheckBusy((UART_ID_Type)UART_0) == SET);/* 等于发送完成，THR必须为空*/
  return ch;
}

/************************************* End *************************************/
