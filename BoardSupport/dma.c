#include "lpc177x_8x_gpdma.h"
#include "lpc177x_8x_pinsel.h"
#include "dma.h"
#include "uart.h"
#include "lpc177x_8x_uart.h"
#include <ucos_ii.h>
#include "GUI.h"
//#include "WM.h"
#include "MainTask.h"  
#include "pwm.h"
#include "Config.h"
//#include "dlg.h"
/*---------------------------- Macro defines --------------------------------------*/
#define DMA_SIZE		50
/*---------------------------- External  variables --------------------------------*/
extern volatile const void *GPDMA_LUTPerAddr[];
extern const LPC_GPDMACH_TypeDef *pGPDMACh[8];
extern const uint8_t GPDMA_LUTPerBurst[];
extern const uint8_t GPDMA_LUTPerWid[];

extern OS_EVENT *QSem;//
extern OS_MEM   *PartitionPt;
extern uint8_t  Partition[MSG_QUEUE_TABNUM][100];

/// If key pressed , isKeyTrigged will be TRUE. Your apps must set iskeyTrigged FALSe after using it.
extern int isKeyTrigged;                   
/*-------------------------- Local Variables --------------------------------------*/
/*-------------------------- Global Variables -------------------------------------*/
volatile int xlCnt  = 0;
// volatile Bool Doubleclick  = FALSE;
uint8_t DMADest_Buffer[DMA_SIZE]; 
GPDMA_Channel_CFG_Type GPDMACfg;

volatile int myCnt = 0;
__IO uint8_t UART2_RX[50];//

void DMA_IRQHandler (void)
{
	uint8_t *pt,*pt0,index,err;	

	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0))
	{
    GPDMA_ChannelCmd(0, DISABLE);  
    
    if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0))/* 检查DMA通道1终端计数请求状态，读取DMACIntTCStatus寄存器来判断中断是否因为传输的结束而产生（终端计数） */ 		
    {

       if(DMADest_Buffer[0] == '!'  ||  DMADest_Buffer[0] == '$')
       {
          xlCnt=0;
          for(index=0;index<50;index++)	
          {
            Partition[myCnt][index]=DMADest_Buffer[index];
          }

          OSQPost(QSem,(void *)Partition[myCnt]); 
          myCnt++;
          myCnt  = myCnt%(MSG_QUEUE_TABNUM);
          
          LPC_GPDMACH0->CDestAddr = GPDMACfg.DstMemAddr;// Assign memory destination address
          LPC_GPDMACH0->CControl= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMACfg.TransferSize) \
                     | GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMACfg.SrcConn]) \
                     | GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMACfg.SrcConn]) \
                     | GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMACfg.SrcConn]) \
                     | GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMACfg.SrcConn]) \
                     | GPDMA_DMACCxControl_DI \
                     | GPDMA_DMACCxControl_I;  
          GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
          GPDMA_ChannelCmd(0, ENABLE);	                   
       } 
       else
       {

          GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);
          SPI1_DMA_Init();
          printf("dma error\n\r");
       }       
//printf("myCnt:%d",myCnt);       
     
//        LPC_GPDMACH1->CControl = (LPC_GPDMACH1->CControl & 0xfffff000)|(sizeof(UART2_RX) &0x0fff);
//        LPC_GPDMACH1->CDestAddr = (uint32_t) &UART2_RX;//ÖØÖÃÆðÊ¼µØÖ·		
//printf("\r\nDMA OK.\n\r");    
    }
    
//    if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)  )		/* 检查DMA通道0中断错误状态 */
//    {
//       printf("\r\n\aDMA Err!\r\n");
//       GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);//Channel0_Err++;	/* 清除DMA通道0中断错误请求 */
//       SPI1_DMA_Init();  //lpc1788_DMA_Init();
//    }
       
//    GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
//    GPDMA_ChannelCmd(0, ENABLE);		
 }
}



void SPI1_DMA_Init(void)
{
	NVIC_DisableIRQ(DMA_IRQn);		/* 关闭GPDMA中断 */
	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));/* 设置抢占优先级preemption为1, 子优先级sub-priority为1 */
	//GPDMA_Init();	/* 初始化GPDMA控制器，打开DMA时钟电源，复位各通道寄存器，清除错误标志等 */
	GPDMACfg.ChannelNum = 0;	/* 使用DMA通道0 */
	GPDMACfg.SrcMemAddr =0;	/* 源地址 */
	GPDMACfg.DstMemAddr = (uint32_t)DMADest_Buffer;	/* 目的地址 */
	GPDMACfg.TransferSize = sizeof(DMADest_Buffer);/* 传输大小 */
	//GPDMACfg.TransferWidth =GPDMA_WIDTH_WORD;	/* 传输宽度 */
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;	/* 设置传输类型为外设到存储器 */
	GPDMACfg.SrcConn = GPDMA_CONN_SSP1_Rx;	/* 设置DMA源请求连接*/
	GPDMACfg.DstConn = 0;	/* 设置DMA目的请求连接，未使用*/
	GPDMACfg.DMALLI = 0;	/* 设置DMA通道链表项，未使用 */

	GPDMA_Setup(&GPDMACfg);/* 把上面的参数设置进DMA相关寄存器 */
	LPC_SSP1->DMACR |=0x11;//SSP_DMACmd (0, SSP_DMA_RXDMA_EN, ENABLE);
	NVIC_EnableIRQ(DMA_IRQn);
	GPDMA_ChannelCmd(0, ENABLE);
}
// void DMA_Config(unsigned char channel)
// {
// 	GPDMA_Channel_CFG_Type GPDMACfg1;
// // 	GPDMA_Init();
// 	NVIC_DisableIRQ (DMA_IRQn);
// 	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
// 	GPDMACfg1.ChannelNum = 1;
// 	GPDMACfg1.SrcMemAddr = 0;// Source memory - don't care
// 	GPDMACfg1.DstMemAddr = (uint32_t) &UART2_RX;// Destination memory
// 	GPDMACfg1.TransferSize = sizeof(UART2_RX);// Transfer size
// //	GPDMACfg1.TransferWidth = 0;// Transfer width - don't care
// 	GPDMACfg1.TransferType = GPDMA_TRANSFERTYPE_P2M;// Transfer type
// 	GPDMACfg1.SrcConn = GPDMA_CONN_UART2_Rx;// Source connection
// 	GPDMACfg1.DstConn = 0;// Destination connection - don't care
// 	GPDMACfg1.DMALLI = 0;// Linker List Item - unused
// 	GPDMA_Setup(&GPDMACfg1);
// 	NVIC_EnableIRQ (DMA_IRQn);
// 	GPDMA_ChannelCmd(1, ENABLE);
// }

