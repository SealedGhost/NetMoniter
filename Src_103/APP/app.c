#include "ucos_ii.h"
#include "includes.h"
//#include "analysis_function.h"
#include "UART.h"
#include "key.h" 
#include "Translate.h"
#include "IWDG.h"
#include "SPI.h"

#define UATx_Length					1			//这个需要后期去查看

volatile uint8_t Res_Flag,Deal_Flag;
uint8_t APP_Rx[60];

char UART_Tx[UATx_Length] = {0};//DMA发送的数组
u8 Flag_Uart_Send ;
///--消息队列的定义部分---
OS_EVENT *QSem;//定义消息队列指针
void *MsgQeueTb[MSG_QUEUE_TABNUM];//定义消息指针数组，队列长度为20
OS_MEM   *PartitionPt;//定义内存分区指针
uint8_t  Partition[MSG_QUEUE_TABNUM][400];


void Task_Translate(void *p_arg)
{
	uint8_t *s; 
	INT8U err;
	uint32_t i=0;
//  OS_MEM_DATA MemInfo;
	(void)p_arg;
	 while (1)
	 {
			s = OSQPend(QSem,0,&err);
// 		  Putc_UART1(s[0]);
// 		  if(s[0]=='!')
// 			{
// 				Res_Flag=1;i=0;
// 				APP_Rx[i]=s[0];
//       }
// 	 		else if(s[0]=='\n')
//  		  {
//  			  Res_Flag=0;
//  			  i=0;
// 				Deal_Flag=1;
//       }
// 			if(Res_Flag)
// 			{
// 				APP_Rx[++i]=s[0];
//       }
// 			if(Deal_Flag)
// 			{
// 				if(translate_(APP_Rx))//,&text_out,&text_out_24A,&text_out_type_of_ship))
// 			  {
//  				  for (i=0;i<50;i++)
//  				    Putc_UART1(APP_Rx[i]);//printf("%c",s[i]);				
//          }
//       }
      if(err==OS_NO_ERR)
			{				
// 		    for (i=0;i<50;i++)
//  				   Putc_UART1(s[i]);//printf("%c",s[i]);	
			  if(translate_(s))//,&text_out,&text_out_24A,&text_out_type_of_ship))
			  {
 				  for (i=0;i<48;i++) SPI2_SendByte(s[i]);
//  				    Putc_UART1(s[i]);//printf("%c",s[i]);
 						SPI2_SendByte('\r');//(0x0D);
 						SPI2_SendByte('\n');					
        } 
// 			  OSMemPut(PartitionPt,s);
//  		 		OSMemQuery(PartitionPt,&MemInfo);				
// 				Putc_UART2(MemInfo.OSNUsed);
// 				Putc_UART2(MemInfo.OSNFree);
//  		    printf("\r\n**********%d**********",MemInfo.OSNUsed);
//  		    printf("**********%d**********\n\r",MemInfo.OSNFree);		 	 
			}
			else
			{
				if(err==OS_TIMEOUT)
				{Putc_UART2('O');Putc_UART2('U');Putc_UART2('T');}
				else{Putc_UART2('E');Putc_UART2('R');Putc_UART2('R');}
      }
			OSTimeDly(10);
	 }
}

void Task_KEY(void *p_arg)
{
	u8 i=0;
	//u32 Key_L=0,Key_L1=0,Key_L2=0,Key_L3=0,Key_L4=0,Key_L5=0;	
		u32 curKeyValue = NO_KEY_PRESS;           	//当前读到的键值
	 static u32 keyValueBackup = NO_KEY_PRESS;		//键值的备份
	 static unsigned char curStatus = KEY_STAT_noKeyPress; // 当前的状态	
	(void)p_arg;
	while (1)
	{
 		OSTimeDlyHMSM(0, 0,0,5);
		curKeyValue = Get_KeyValue();
		switch(curStatus) 
		{
				case KEY_STAT_noKeyPress: /* -----无按键按下的状态----- */
						 if( curKeyValue != NO_KEY_PRESS ) 
						 {// 判断是否有按键按下，有则进入消抖状态，无则保持
							  if(++i==2)
								{
									keyValueBackup = curKeyValue;
									curStatus = KEY_STAT_filter; 
									i=0;
								}									
						 }
						 else
						 {
						 }break;
				case KEY_STAT_filter:			/* -----消抖状态----- */		
							 if( curKeyValue == keyValueBackup )
							 {
								 UART_Tx[0] &= 0;//printf("%d",keyValueBackup);
								 switch(keyValueBackup)
								 {							 
								   case 0x00001:UART_Tx[0] |=14;Flag_Uart_Send = 1;break;// 归中
								   case 0x00002:UART_Tx[0] |=18;Flag_Uart_Send = 1;GPIO_ResetBits(GPIOA,GPIO_Pin_11);break;//→  
								   case 0x00004:UART_Tx[0] |=19;Flag_Uart_Send = 1;GPIO_ResetBits(GPIOA,GPIO_Pin_9);break;//↓    
								   case 0x00008:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省
									 case 0x00010:UART_Tx[0] |=8 ;Flag_Uart_Send = 1;GPIO_ResetBits(GPIOC,GPIO_Pin_13);break;//返回  //调暗
									 case 0x00020:UART_Tx[0] |=17;Flag_Uart_Send = 1;GPIO_ResetBits(GPIOA,GPIO_Pin_8);break;//↑  
									 case 0x00040:UART_Tx[0] |=31;Flag_Uart_Send = 1;break;//航迹关
									 case 0x00080:UART_Tx[0] |=13;Flag_Uart_Send = 1;GPIO_ResetBits(GPIOA,GPIO_Pin_12);break;//OK 
									 case 0x00100:UART_Tx[0] |=11;Flag_Uart_Send = 1;Sound_U();break;//监控开 
									 case 0x00200:UART_Tx[0] |=6 ;Flag_Uart_Send = 1;break;//放大  
									 case 0x00400:UART_Tx[0] |=5 ;Flag_Uart_Send = 1;break;//菜单	
									 case 0x00800:UART_Tx[0] |=16;Flag_Uart_Send = 1;GPIO_ResetBits(GPIOA,GPIO_Pin_10);break;//←
									 case 0x01000:UART_Tx[0] |=20;Flag_Uart_Send = 1;break;//
									 case 0x02000:UART_Tx[0] |=7 ;Flag_Uart_Send = 1;break;//缩小
									 case 0x04000:UART_Tx[0] |=12;Flag_Uart_Send = 1;Sound_D();break;//监控关
									 case 0x08000:UART_Tx[0] |=28;Flag_Uart_Send = 1;GPIO_SetBits(GPIOB,GPIO_Pin_5);break;//航迹开						
									 case 0x10000:UART_Tx[0] |=21;Flag_Uart_Send = 1;GPIO_ResetBits(GPIOB,GPIO_Pin_5);break;//调亮
									 case 0x20000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省
									 case 0x40000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省
									 case 0x80000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省								
									 default :break;
                 }
								 if(Flag_Uart_Send){Putc_UART3(UART_Tx[0]);}
								 curStatus = KEY_STAT_clicking; 
							 }						 
						 else
						 {
							 //returnKeyValue = NO_KEY_PRESS; 
						 }break;//返回键值	
				case KEY_STAT_clicking:/* -----单击状态----- */	 
						 if( curKeyValue == NO_KEY_PRESS )	//判断按键是否抬起，是则进入按键释放，否则保持
								curStatus = KEY_STAT_Release; 
						 else //if( ++keyPressTimer > LONG_PRESS_TIME )			
						 { 
							 curStatus = KEY_STAT_clicking; 
						 } break;	
				case KEY_STAT_Release:/* -----按键释放状态----- */						 
						 switch(keyValueBackup)
						 {							 
// 							 case 0x00010:UART_Tx[0] |=22;break;//调暗
// 							 case 0x00020:UART_Tx[0] |=21;break;//调亮
							 case 0x00040:break;//航迹关
							 case 0x08000:break;//航迹开
							 case 0x00800:GPIO_SetBits(GPIOA,GPIO_Pin_10);break;//←
							 case 0x00020:GPIO_SetBits(GPIOA,GPIO_Pin_8);break;//↑
							 case 0x00004:GPIO_SetBits(GPIOA,GPIO_Pin_9);break;//↓
							 case 0x00002:GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOC,GPIO_Pin_13);break;//→			
							 case 0x00080:GPIO_SetBits(GPIOA,GPIO_Pin_12);break;//OK 
							 case 0x00010:GPIO_SetBits(GPIOC,GPIO_Pin_13);break;//返回 
							 default :break;
						 }
						 keyValueBackup = NO_KEY_PRESS;
						 if(Flag_Uart_Send){UART_Tx[0]|=0x80;Putc_UART3(UART_Tx[0]);Flag_Uart_Send=0;}
				     curStatus = KEY_STAT_noKeyPress;			
						 break;	
				default:break;
			}						
	}
}


