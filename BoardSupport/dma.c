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
#define DMA_SIZE		1
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
volatile uint8_t Brightness  = 3;
// volatile Bool Doubleclick  = FALSE;
uint8_t DMADest_Buffer[DMA_SIZE]; 
GPDMA_Channel_CFG_Type GPDMACfg;

volatile int myCnt = 0;
__IO uint8_t UART2_RX[50];//

void DMA_IRQHandler (void)
{
	uint8_t *pt,*pt0,index,err;	

	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 1))
	{
    GPDMA_ChannelCmd(1, DISABLE);
     
    if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 1))/* 检查DMA通道1终端计数请求状态，读取DMACIntTCStatus寄存器来判断中断是否因为传输的结束而产生（终端计数） */ 		
    {
       if(UART2_RX[0] == '!'  ||  UART2_RX[0] == '$')
       {
          for(index=0;index<50;index++)	
          {
            Partition[myCnt][index]=UART2_RX[index];
          }

          OSQPost(QSem,(void *)Partition[myCnt]); 
          myCnt++;
          myCnt  = myCnt%(MSG_QUEUE_TABNUM);
       } 
       else
       {
          printf("dma error\n\r");
       }       
//printf("myCnt:%d",myCnt);       
      
       LPC_GPDMACH1->CControl = (LPC_GPDMACH1->CControl & 0xfffff000)|(sizeof(UART2_RX) &0x0fff);
       LPC_GPDMACH1->CDestAddr = (uint32_t) &UART2_RX;//ÖØÖÃÆðÊ¼µØÖ·		
//printf("\r\nDMA OK.\n\r");    
    }
    
    if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 1)  )		/* 检查DMA通道0中断错误状态 */
    {
       printf("\r\n\aDMA Err!\r\n");
        GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 1);//Channel0_Err++;	/* 清除DMA通道0中断错误请求 */
        DMA_Config(1);  //lpc1788_DMA_Init();
    }
       
    GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 1);
    GPDMA_ChannelCmd(1, ENABLE);		
 }
}


void DMA_Config(unsigned char channel)
{
	GPDMA_Channel_CFG_Type GPDMACfg1;
// 	GPDMA_Init();
	NVIC_DisableIRQ (DMA_IRQn);
	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
	GPDMACfg1.ChannelNum = 1;
	GPDMACfg1.SrcMemAddr = 0;// Source memory - don't care
	GPDMACfg1.DstMemAddr = (uint32_t) &UART2_RX;// Destination memory
	GPDMACfg1.TransferSize = sizeof(UART2_RX);// Transfer size
//	GPDMACfg1.TransferWidth = 0;// Transfer width - don't care
	GPDMACfg1.TransferType = GPDMA_TRANSFERTYPE_P2M;// Transfer type
	GPDMACfg1.SrcConn = GPDMA_CONN_UART2_Rx;// Source connection
	GPDMACfg1.DstConn = 0;// Destination connection - don't care
	GPDMACfg1.DMALLI = 0;// Linker List Item - unused
	GPDMA_Setup(&GPDMACfg1);
	NVIC_EnableIRQ (DMA_IRQn);
	GPDMA_ChannelCmd(1, ENABLE);
}

