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


extern uint8_t SND[4][6];
extern int isKeyTrigged;

volatile Bool Doubleclick  = FALSE;
volatile Bool isReleasedDet  = FALSE;



void xl_UART_Config(unsigned char port)
{
   UART_CFG_Type      UARTConfigStruct;
   UART_FIFO_CFG_Type UARTFIFOConfigStruct;
   
   UART_ConfigStructInit(&UARTConfigStruct);
   UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
   
   switch(port)
   {
      case 0:
           PINSEL_ConfigPin(0,2,1);
           PINSEL_ConfigPin(0,3,1);
           UARTConfigStruct.Baud_rate  = 9600;
           
           UART_Init( (UART_ID_Type)port, &UARTConfigStruct );
           UART_FIFOConfig( (UART_ID_Type)port, &UARTFIFOConfigStruct );
           UART_TxCmd( (UART_ID_Type)port, ENABLE );      
           
           UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
           UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);           
           
           NVIC_DisableIRQ(UART0_IRQn);           
           break;
           
      case 2:
           PINSEL_ConfigPin(0,10,1);
           PINSEL_ConfigPin(0,11,1);
           UARTConfigStruct.Baud_rate = 115200;
           
           UART_Init( (UART_ID_Type)port, &UARTConfigStruct );
           UART_FIFOConfig( (UART_ID_Type)port, &UARTFIFOConfigStruct );
           UART_TxCmd( (UART_ID_Type)port, ENABLE );
                     
           UART_IntConfig(UART_2, UART_INTCFG_RBR, ENABLE);
           UART_IntConfig(UART_2, UART_INTCFG_RLS, ENABLE); 
           
           NVIC_SetPriority(UART2_IRQn, ((0x02<<3)|0x02));
	         	NVIC_EnableIRQ(UART2_IRQn);
           break;
           
     default:
           break;
   }
   

}


void USER_Init(void)
{	
	GPDMA_Init();
 xl_UART_Config(0);
 xl_UART_Config(2);

}

void UART2_IRQHandler(void)
{
	uint8_t tmpc;
	uint32_t  tmp, tmp1;
	tmp = ((LPC_UART2->IIR) & UART_IIR_BITMASK) & UART_IIR_INTID_MASK;
	if (tmp == UART_IIR_INTID_RLS)	// Receive Line Status
	{
			tmp1 = UART_GetLineStatus(UART_2);// Check line status
			tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);// Mask out the Receive Ready and Transmit Holding empty status
	}
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))	// Receive Data Available or Character time-out
		{	
 		  UART_Receive(UART_2, &tmpc, 1, NONE_BLOCKING);
    if(tmpc >= GUI_KEY_MENU  &&  tmpc <= GUI_KEY_PGDOWN)   
			 {
             isKeyTrigged  = 1;
             switch(tmpc)
             {
//               case GUI_KEY_PWM_INC:
//                 WM_SendMessageNoPara(WM_GetClientWindow(subWins[3]), GUI_KEY_PWM_INC);
//                 break;								
//               case GUI_KEY_PWM_DEC:
//                 WM_SendMessageNoPara(WM_GetClientWindow(subWins[3]), GUI_KEY_PWM_DEC);
//                 break;	
                   
               case GUI_KEY_TRACE_ENABLE:
                 break;
               case GUI_KEY_TRACE_DISABLE:
                 break;
               default:
                  GUI_StoreKeyMsg(tmpc, 1);
                 break;
             }
             
       }   
       else if(tmpc >= 0x80  &&  tmpc <(0x80+21))
       {
          GUI_StoreKeyMsg(GUI_KEY_RELEASE, 1);
       }
       else
       {    
       }
   }
}



PUTCHAR_PROTOTYPE//重定向C printf函数到Uart0
{
	UART_Send((UART_ID_Type)UART_2, (uint8_t*) &ch, 1, BLOCKING);  /* 发送一个字符到UART */
	while (UART_CheckBusy((UART_ID_Type)UART_2) == SET);/* 等于发送完成，THR必须为空*/
  return ch;
}

/************************************* End *************************************/
