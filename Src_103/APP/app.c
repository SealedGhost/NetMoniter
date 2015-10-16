#include "ucos_ii.h"
#include "includes.h"
//#include "analysis_function.h"
#include "UART.h"
#include "key.h" 
#include "Translate.h"
#include "IWDG.h"
#include "SPI.h"

#define UATx_Length					1			//这个需要后期去查看

extern uint8_t isAlarm;
extern uint8_t armState;
extern Music_Info Music_Information;
extern uint8_t Music_flag;

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
//  OS_MEM_DATA MemInfo;

	 while (1)
	 {
      s = OSQPend(QSem,0,&err);
         if(err==OS_NO_ERR)
         {
            translate_(s);
         }      

      else
      {
         if(err==OS_TIMEOUT)
         {
            Putc_UART2('O');Putc_UART2('U');Putc_UART2('T');}
         else
         {
            Putc_UART2('E');Putc_UART2('R');Putc_UART2('R');
         }
      }
  
		   	OSTimeDly(10);
   }
}



//void Task_Music(void *p_arg)
//{
//	uint8_t i;
//	static uint8_t Music_vol=2;
//	static uint8_t Music_vol_old=1;
////	Sound_D();
////  Sound_D();
////  Sound_D();


//	while(1)
//	{
//			if(isAlarm)
//			{
//				if(Music_flag)
//		    {
//					switch (Music_Information.volume)
//					{
//						case 0x01:Music_vol=0;break;
//						case 0x02:Music_vol=1;break;
//						case 0x03:Music_vol=2;break;
//						case 0x04:Music_vol=3;break;
//						case 0x05:Music_vol=4;break;
//					}
//			  Putc_UART1('V');
//     Putc_UART1(Music_vol);
//					if(Music_vol==0)break;
//					if(Music_vol>Music_vol_old)
//					{
//        for(i=0;i<Music_vol-Music_vol_old;i++)
//        {
//           GPIO_ResetBits(GPIOC,GPIO_Pin_15);
//           OSTimeDlyHMSM(0,0,0,20);
//           GPIO_SetBits(GPIOC,GPIO_Pin_15);
//           OSTimeDlyHMSM(0,0,0,20);
//        }
//					}
//					else if(Music_vol<Music_vol_old)
//					{
//        for(i=0;i<Music_vol_old-Music_vol;i++)
//        {
//           GPIO_ResetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20);
//           GPIO_SetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20);
//        }
//					}//Putc_UART1();
//     OpenMusic();
//					switch (Music_Information.Alarm_number)
//					{
//						 case 1:Choose_Alarm1();break;
//						 case 2:Choose_Alarm2();break;
//						 case 3:Choose_Alarm3();break;
//						 default:break;
//					}
//					Music_flag=0;
//				}
//      }
//			else if(!isAlarm)
//			{
//				switch(Music_vol)
//				{
////					case 1:break;
////					case 2:Sound_D();break;
////					case 3:Sound_D();Sound_D();break;
////					case 4:Sound_D();Sound_D();Sound_D();break;
//       case 4:
//           GPIO_ResetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20);
//           GPIO_SetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20);
//         
//       case 3:
//           GPIO_ResetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20);
//           GPIO_SetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20);

//       case 2:
//           GPIO_ResetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20);
//           GPIO_SetBits(GPIOC,GPIO_Pin_14);
//           OSTimeDlyHMSM(0,0,0,20); 
//           break;           
//       // case 1:break;  
//					default:break;
//        }
//      }
//			
//		}
//		OSTimeDly(10);
//}


void Task_KEY(void *p_arg)
{
	u8 i=0;
	//u32 Key_L=0,Key_L1=0,Key_L2=0,Key_L3=0,Key_L4=0,Key_L5=0;	
		u32 curKeyValue = NO_KEY_PRESS;           	//当前读到的键值
	 static u32 keyValueBackup = NO_KEY_PRESS;		//键值的备份
	 static unsigned char curStatus = KEY_STAT_noKeyPress; // 当前的状态	
  
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
           
						 }
       break;
				case KEY_STAT_filter:			/* -----消抖状态----- */		
							 if( curKeyValue == keyValueBackup )
							 {
//								  if(isAlarm)
//									{
//										 isAlarm  = FALSE;
//										 Close_Alarm1();
//										 Close_Alarm3();
//										 Close_Alarm2();
//									}
								 UART_Tx[0] &= 0;//printf("%d",keyValueBackup);
								 switch(keyValueBackup)
								 {							 
								   case 0x00001:UART_Tx[0] |=14;Flag_Uart_Send = 1;break;// 归中
								   case 0x00002:UART_Tx[0] |=18;Flag_Uart_Send = 1;break;//→  
								   case 0x00004:UART_Tx[0] |=19;Flag_Uart_Send = 1;break;//↓    
								   case 0x00008:UART_Tx[0] |=0 ;/*Flag_Uart_Send = 1;*/break;//缺省
									 case 0x00010:UART_Tx[0] |=8 ;Flag_Uart_Send = 1;break;//返回  
									 case 0x00020:UART_Tx[0] |=17;Flag_Uart_Send = 1;break;//↑  
									 case 0x00040:UART_Tx[0] |=0;Flag_Uart_Send = 1;break;//缺省
									 case 0x00080:UART_Tx[0] |=13;Flag_Uart_Send = 1;break;//OK 
									 case 0x00100:UART_Tx[0] |=11;Flag_Uart_Send = 1;break;//监控开 
									 case 0x00200:UART_Tx[0] |=6 ;Flag_Uart_Send = 1;break;//放大  
									 case 0x00400:UART_Tx[0] |=5 ;Flag_Uart_Send = 1;break;//菜单	
									 case 0x00800:UART_Tx[0] |=16;Flag_Uart_Send = 1;break;//←
									 case 0x01000:UART_Tx[0] |=12;Flag_Uart_Send = 1;break;//监控关
									 case 0x02000:UART_Tx[0] |=7 ;Flag_Uart_Send = 1;break;//缩小
									 case 0x04000:UART_Tx[0] |=31;Flag_Uart_Send = 1;break;//航迹关 
									 case 0x08000:UART_Tx[0] |=28;Flag_Uart_Send = 1;break;//航迹开						
									 case 0x10000:UART_Tx[0] |=21;Flag_Uart_Send = 1;break;//亮度
									 case 0x20000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省
									 case 0x40000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省
									 case 0x80000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省								
									 default :break;
                 }
								 if(Flag_Uart_Send)
                 {                                    
//										GPIO_SetBits(GPIOB,GPIO_Pin_5);
//										GPIO_ResetBits(GPIOA,GPIO_Pin_8);
										Putc_UART3(UART_Tx[0]);
                 }
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
// 						 switch(keyValueBackup)
// 						 {							 
// // 							 case 0x00010:UART_Tx[0] |=22;break;//调暗
// // 							 case 0x00020:UART_Tx[0] |=21;break;//调亮
// 							 case 0x00040:break;//航迹关
// 							 case 0x08000:break;//航迹开
// 							 case 0x00800:GPIO_SetBits(GPIOA,GPIO_Pin_10);break;//←
// 							 case 0x00020:GPIO_SetBits(GPIOA,GPIO_Pin_8);break;//↑
// 							 case 0x00004:GPIO_SetBits(GPIOA,GPIO_Pin_9);break;//↓
// 							 case 0x00002:GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOC,GPIO_Pin_13);break;//→			
// 							 case 0x00080:GPIO_SetBits(GPIOA,GPIO_Pin_12);break;//OK 
// 							 case 0x00010:GPIO_SetBits(GPIOC,GPIO_Pin_13);break;//返回 
// 							 default :break;
// 						 }
						 keyValueBackup = NO_KEY_PRESS;
						 if(Flag_Uart_Send){UART_Tx[0]|=0x80;Putc_UART3(UART_Tx[0]);Flag_Uart_Send=0;}
				     curStatus = KEY_STAT_noKeyPress;			
						 break;	
				default:break;
			}						
	}
}


