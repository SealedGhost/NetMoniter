#include "lpc177x_8x_gpdma.h"
#include "lpc177x_8x_pinsel.h"
#include "dma.h"
#include "uart.h"
#include "lpc177x_8x_uart.h"
#include <ucos_ii.h>
#include "GUI.h"
//#include "WM.h"
#include "MainTask.h"  

#define DMA_SIZE		3

extern int ReleasedDectSwitch;
uint8_t DMADest_Buffer[DMA_SIZE]; 
GPDMA_Channel_CFG_Type GPDMACfg;
extern volatile const void *GPDMA_LUTPerAddr[];
extern const LPC_GPDMACH_TypeDef *pGPDMACh[8];
extern const uint8_t GPDMA_LUTPerBurst[];
extern const uint8_t GPDMA_LUTPerWid[];
volatile int myCnt = 0;
__IO uint8_t UART2_RX[50];//
extern OS_EVENT *QSem;//
extern OS_MEM   *PartitionPt;
extern uint8_t  Partition[MSG_QUEUE_TABNUM][100];
void DMA_IRQHandler (void)
{
	uint8_t *pt,*pt0,index,err;	

	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0))	/* 检查DMA通道0中断状态 */
	{ 
		GPDMA_ChannelCmd(0, DISABLE); 
 
		if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0))/* 检查DMA通道0终端计数请求状态，读取DMACIntTCStatus寄存器来判断中断是否因为传输的结束而产生（终端计数） */ 
		{			
//			GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);/* 清除DMA通道0终端计数中断请求 */				
     switch (DMADest_Buffer[0])
			{
			
				case 0x4E: //按下

								switch (DMADest_Buffer[1])
								{
									case 0x61://放大
										        if(DMADest_Buffer[2]==0x81)
										        {printf("a");GUI_StoreKeyMsg(GUI_KEY_LARGE ,1);}    else printf("error 0x4E 0x61 but not 0x81");break;
									case 0x62://归中
									          if(DMADest_Buffer[2]==0x80)
										        {printf("b");/*GUI_StoreKeyMsg(GUI_KEY_LARGE ,1);*/}else printf("error 0x4E 0x62 but not 0x80");break;
									case 0x63://缩小
										       if(DMADest_Buffer[2]==0x7F)
										       {printf("c");GUI_StoreKeyMsg(GUI_KEY_REDUCE,1);}     else printf("error 0x4E 0x63 but not 0x7F");break;	
									case 0x65://PWM++
                           if(DMADest_Buffer[2]==0x7D)
                           {printf("e");/*BACK_PWM--;if(BACK_PWM==-1)BACK_PWM=0;PWM_SET(BACK_PWM);*/}else printf("error 0x4E 0x65 but not 0x7D");break;													 
									case 0x66://PWM--
                           if(DMADest_Buffer[2]==0x7C)
                           {printf("f");/*BACK_PWM++;if(BACK_PWM==11)BACK_PWM=10;PWM_SET(BACK_PWM);*/}else printf("error 0x4E 0x66 but not 0x7C");break;
									case 0x67://航迹关
                           if(DMADest_Buffer[2]==0x7B)
                           {printf("g");/*BACK_PWM--;if(BACK_PWM==-1)BACK_PWM=0;PWM_SET(BACK_PWM);*/}else printf("error 0x4E 0x67 but not 0x7B");break;													 
									case 0x68://航迹开
                           if(DMADest_Buffer[2]==0x7A)
                           //{printf("h");/*BACK_PWM++;if(BACK_PWM==11)BACK_PWM=10;PWM_SET(BACK_PWM);*/}else printf("error 0x4E 0x68 but not 0x7A");break;
													 {printf("n");GUI_StoreKeyMsg(GUI_KEY_ENTER ,1); }else printf("error 0x4E 0x6E but not 0x74");break;
									case 0x69://监控关
										       if(DMADest_Buffer[2]==0x79)
										       {printf("i");GUI_StoreKeyMsg(GUI_KEY_CANCEL,1);}    else printf("error 0x4E 0x69 but not 0x79");break;													 
									case 0x6A://监控开
										       if(DMADest_Buffer[2]==0x78)
										       {printf("j");GUI_StoreKeyMsg(GUI_KEY_MONITORING,1);}else printf("error 0x4E 0x6A but not 0x78");break;		
									case 0x6B://左
										       if(DMADest_Buffer[2]==0x77)
										       {printf("k");GUI_StoreKeyMsg(GUI_KEY_LEFT    ,1); }else printf("error 0x4E 0x6B but not 0x77");break;
									case 0x6C://上
										       if(DMADest_Buffer[2]==0x76)
										       {printf("l");GUI_StoreKeyMsg(GUI_KEY_UP    ,1); }else printf("error 0x4E 0x6C but not 0x76");break;
									case 0x6D://下
													 if(DMADest_Buffer[2]==0x75)
													 {printf("m");GUI_StoreKeyMsg(GUI_KEY_DOWN ,1);}else printf("error 0x4E 0x6D but not 0x75");break;
									case 0x6E://确定
													 if(DMADest_Buffer[2]==0x74)
													 {printf("n");GUI_StoreKeyMsg(GUI_KEY_ENTER ,1); }else printf("error 0x4E 0x6E but not 0x74");break;
									case 0x6F://右
													 if(DMADest_Buffer[2]==0x73)
													 {printf("o");GUI_StoreKeyMsg(GUI_KEY_RIGHT ,1); }else printf("error 0x4E 0x6F but not 0x73");break;												
									case 0x70://菜单
													 if(DMADest_Buffer[2]==0x72)
													 {printf("p");GUI_StoreKeyMsg(GUI_KEY_MENU  ,1);}else printf("error 0x4E 0x70 but not 0x72");break;
									case 0x71://返回
													 if(DMADest_Buffer[2]==0x71)
													 {printf("q");GUI_StoreKeyMsg(GUI_KEY_BACKSPACE,1);}else printf("error 0x4E 0x71 but not 0x71");break;
// 									case 0x3A://F1
// 										if(DMADest_Buffer[2]==0x78)
// 										{printf("F1:");GUI_StoreKeyMsg(GUI_KEY_F1,1);}else printf("error 0x4E 0x3A but not 0x78");break;
// 									case 0x3B://F2
// 										if(DMADest_Buffer[2]==0x77)
// 										{printf("F2;");GUI_StoreKeyMsg(GUI_KEY_F2,1);}else printf("error 0x4E 0x3B but not 0x77");break;
									default:printf("ERROR DEFAULT 0x4E");break;
								}break;
								
				case 0x43: //松开
					if(ReleasedDectSwitch)
								switch (DMADest_Buffer[1])
								{
										case 0x6C:if(DMADest_Buffer[2]==0x76){printf("5");GUI_StoreKeyMsg(GUI_KEY_UP    ,0);}else printf("error 0x43 0x6C but not 0x76");break;
										case 0x6B:if(DMADest_Buffer[2]==0x77){printf("7");GUI_StoreKeyMsg(GUI_KEY_LEFT  ,0);}else printf("error 0x43 0x6B but not 0x77");break;
										case 0x6F:if(DMADest_Buffer[2]==0x73){printf("9");GUI_StoreKeyMsg(GUI_KEY_RIGHT ,0);}else printf("error 0x43 0x6F but not 0x73");break;
										case 0x6D:if(DMADest_Buffer[2]==0x75){printf("0");GUI_StoreKeyMsg(GUI_KEY_DOWN  ,0);}else printf("error 0x43 0x6D but not 0x75");break;
										
									default:printf("ERROR DEFAULT 0x43");break;
								}break;
				default:printf("ERROR UNDEFINE THIS KEY");lpc1788_DMA_Init();break;									
			}		
			LPC_GPDMACH0->CDestAddr = GPDMACfg.DstMemAddr;// Assign memory destination address
			LPC_GPDMACH0->CControl= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMACfg.TransferSize) \
														| GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMACfg.SrcConn]) \
														| GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMACfg.SrcConn]) \
														| GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMACfg.SrcConn]) \
														| GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMACfg.SrcConn]) \
														| GPDMA_DMACCxControl_DI \
														| GPDMA_DMACCxControl_I;			
		}
		if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0) )		/* 检查DMA通道0中断错误状态 */
		{
  printf(" \n\r\aDMA ERR!\n\r\a");
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);//Channel0_Err++;	/* 清除DMA通道0中断错误请求 */
   lpc1788_DMA_Init();
		}
  
  
  GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
		GPDMA_ChannelCmd(0, ENABLE);
	}
///////////////////////////////////////////////////////	
	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 1))
	{
    GPDMA_ChannelCmd(1, DISABLE);
     
    if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 1))/* 检查DMA通道1终端计数请求状态，读取DMACIntTCStatus寄存器来判断中断是否因为传输的结束而产生（终端计数） */ 		
    {
//       GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 1);
//       pt=OSMemGet(PartitionPt,&err);
//       pt0=pt;
//        for(index=0;index<50;index++)	
//        {
//          *pt=UART2_RX[index];
//           pt++;
//        }
//        OSQPost(QSem,(void *)pt0);		

        
       for(index=0;index<50;index++)	
       {
         Partition[myCnt][index]=UART2_RX[index];
       }

       OSQPost(QSem,(void *)Partition[myCnt]); 
       myCnt++;
       myCnt  = myCnt%(MSG_QUEUE_TABNUM);  
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



void lpc1788_DMA_Init(void)
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
	GPDMACfg.SrcConn = GPDMA_CONN_UART0_Rx;	/* 设置DMA源请求连接*/
	GPDMACfg.DstConn = 0;	/* 设置DMA目的请求连接，未使用*/
	GPDMACfg.DMALLI = 0;	/* 设置DMA通道链表项，未使用 */

	GPDMA_Setup(&GPDMACfg);/* 把上面的参数设置进DMA相关寄存器 */
	NVIC_EnableIRQ(DMA_IRQn);
	GPDMA_ChannelCmd(0, ENABLE);
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

