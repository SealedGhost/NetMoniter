#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_gpdma.h"
#include "uart.h"
#include "timer.h"
#include "dma.h"
#include <ucos_ii.h>
uint8_t Buffer0[23]  = "uart0 init success\n\r";
uint8_t Buffer2[23]  = "uart2 init success!\n\r"; 

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
       PINSEL_ConfigPin(0,3,1);break;
    case 2:
       PINSEL_ConfigPin(0,10,1);
       PINSEL_ConfigPin(0,11,1);break;
    default:break;
	}
	UART_ConfigStructInit(&UARTConfigStruct);/* 初始化UARTConfigStruct结构体为：波特率为115200，8位数据，无奇偶校验，1位停止位 */
	UARTConfigStruct.Baud_rate = 115200;
	UART_Init((UART_ID_Type)port, &UARTConfigStruct);	/* 用UARTConfigStruct给定的参数初始化UART外设 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	UARTFIFOConfigStruct.FIFO_DMAMode = ENABLE;//为UART使能DMA
	UART_FIFOConfig(UART_0, &UARTFIFOConfigStruct);// 为UART0外设初始化FIFO

	UART_TxCmd((UART_ID_Type)port, ENABLE);/* 使能UART发送 */
	
	UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);
	NVIC_DisableIRQ(UART0_IRQn);
		
}


 void UART_Config(unsigned char channel)
{
   UART_CFG_Type UARTConfigStruct1;
   UART_FIFO_CFG_Type UARTFIFOConfigStruct1;
   PINSEL_ConfigPin(0,10,1);
   PINSEL_ConfigPin(0,11,1);
   UART_ConfigStructInit(&UARTConfigStruct1);
   UARTConfigStruct1.Baud_rate = 115200;
   UART_Init((UART_ID_Type)channel, &UARTConfigStruct1);
   
   UART_FIFOConfigStructInit(&UARTFIFOConfigStruct1);
   UARTFIFOConfigStruct1.FIFO_DMAMode = ENABLE;
   UART_FIFOConfig((UART_ID_Type)channel, &UARTFIFOConfigStruct1);
   UART_TxCmd((UART_ID_Type)channel, ENABLE);
}

// static void DMA_Config(unsigned char channel)
// {
// 	GPDMA_Channel_CFG_Type GPDMACfg; 
// // 	GPDMA_Init();
// 	NVIC_DisableIRQ (DMA_IRQn);
// 	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
// 	GPDMACfg.ChannelNum = 1;
// 	GPDMACfg.SrcMemAddr = 0;// Source memory - don't care
// 	GPDMACfg.DstMemAddr = (uint32_t) &UART2_RX;// Destination memory
// 	GPDMACfg.TransferSize = sizeof(UART2_RX);// Transfer size
// 	GPDMACfg.TransferWidth = 0;// Transfer width - don't care
// 	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;// Transfer type
// 	GPDMACfg.SrcConn = GPDMA_CONN_UART2_Rx;// Source connection
// 	GPDMACfg.DstConn = 0;// Destination connection - don't care
// 	GPDMACfg.DMALLI = 0;// Linker List Item - unused
// 	GPDMA_Setup(&GPDMACfg);
// 	NVIC_EnableIRQ (DMA_IRQn);
// 	GPDMA_ChannelCmd(1, ENABLE);
// }

void USER_Init(void)
{	
	GPDMA_Init();
	lpc1788_Uart_Init(0);/* 初始化串口Uart0 使用DMA通道0*/	
 lpc1788_DMA_Init();  
//	printf("uart0 init success!\n\r\r");	
	UART_Send((UART_ID_Type)0, Buffer0,sizeof(Buffer0),BLOCKING);

     
//	UART_Send((UART_ID_Type)2,Buffer2,sizeof(Buffer2),BLOCKING);///* 打印一串字符到串口Uart2 */
////	lpc1788_TIMER_Init();
}





PUTCHAR_PROTOTYPE//重定向C printf函数到Uart0
{
	UART_Send((UART_ID_Type)UART_0, (uint8_t*) &ch, 1, BLOCKING);  /* 发送一个字符到UART */
	while (UART_CheckBusy((UART_ID_Type)UART_0) == SET);/* 等于发送完成，THR必须为空*/
  return ch;
}

/************************************* End *************************************/
