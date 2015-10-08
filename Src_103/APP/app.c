#include "ucos_ii.h"
#include "includes.h"
//#include "analysis_function.h"
#include "UART.h"
#include "key.h" 
#include "Translate.h"
#include "IWDG.h"



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
//  OS_MEM_DATA MemInfo;
	(void)p_arg;

	 while (1)
	 {
			s = OSQPend(QSem,0,&err);

   if(err==OS_NO_ERR)
			{				
      translate_(s);//,&text_out,&text_out_24A,&text_out_type_of_ship))
			}
			else
			{
//      if(err==OS_TIMEOUT)
//      {
//         Putc_UART2('O');Putc_UART2('U');Putc_UART2('T');
//      }
//      else
//      {
//         Putc_UART2('E');Putc_UART2('R');Putc_UART2('R');
//      }
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
 		OSTimeDlyHMSM(0, 0,0,10);
		curKeyValue = Get_KeyValue();
		switch(curStatus) 
		{
				case KEY_STAT_noKeyPress: /* -----无按键按下的状态----- */
						 if( curKeyValue != NO_KEY_PRESS ) 
						 {// 判断是否有按键按下，有则进入消抖状态，无则保持
							  if(++i==10)
								{
									keyValueBackup = curKeyValue;
									curStatus = KEY_STAT_filter; 
									i=0;
								}									
						 }
						 else
						 {
								//returnKeyValue = NO_KEY_PRESS;
						 }break;
				case KEY_STAT_filter:			/* -----消抖状态----- */		
						 //if( curKeyValue != NO_KEY_PRESS ) 
						 //{	//判断消抖有没有完成，完成则进入按键单击状态，否则保持
							 if( curKeyValue == keyValueBackup )
							 {
								 //printf("%d",keyValueBackup);//key_pressed=1;
//								 returnKeyValue = keyValueBackup; 
								 UART_Tx[0] &= 0;
								 switch(keyValueBackup)
								 {							 
								   case 0x00001:UART_Tx[0] |=6 ;Flag_Uart_Send = 1;break;//放大
								   case 0x00002:UART_Tx[0] |=14;Flag_Uart_Send = 1;break;//归中
								   case 0x00004:UART_Tx[0] |=7 ;Flag_Uart_Send = 1;break;//缩小
								   case 0x00008:UART_Tx[0] |=0 ;break;//缺省
           case 0x00010:UART_Tx[0] |=22;Flag_Uart_Send = 1;break;//调暗
           case 0x00020:UART_Tx[0] |=21;Flag_Uart_Send = 1;break;//调亮
           case 0x00040:UART_Tx[0] |=31;Flag_Uart_Send = 1;break;//航迹关
           case 0x00080:UART_Tx[0] |=28;Flag_Uart_Send = 1;break;//航迹开
           case 0x00100:UART_Tx[0] |=12;Flag_Uart_Send = 1;break;//监控关
           case 0x00200:UART_Tx[0] |=11;Flag_Uart_Send = 1;break;//监控开
           case 0x00400:UART_Tx[0] |=16;Flag_Uart_Send = 1;break;//←
           case 0x00800:UART_Tx[0] |=17;Flag_Uart_Send = 1;break;//↑
           case 0x01000:UART_Tx[0] |=19;Flag_Uart_Send = 1;break;//↓
           case 0x02000:UART_Tx[0] |=13;Flag_Uart_Send = 1;break;//OK
           case 0x04000:UART_Tx[0] |=18;Flag_Uart_Send = 1;break;//→
           case 0x08000:UART_Tx[0] |=5 ;Flag_Uart_Send = 1;break;//菜单							
           case 0x10000:UART_Tx[0] |=8 ;Flag_Uart_Send = 1;break;//返回
           case 0x20000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省
           case 0x40000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省
           case 0x80000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//缺省								
           default :break;
         }
								 if(Flag_Uart_Send)/*for(i=0;i<UATx_Length;i++)*/
         {   
            Putc_UART2(UART_Tx[0]);           
         }
								 curStatus = KEY_STAT_clicking; 
							 }						 
						 //}
						 else
						 {
							 //returnKeyValue = NO_KEY_PRESS; 
						 }break;//返回键值	
				case KEY_STAT_clicking:/* -----单击状态----- */	 
						 if( curKeyValue == NO_KEY_PRESS )	//判断按键是否抬起，是则进入按键释放，否则保持
       {
          UART_Tx[0]|=0x80;/*for(i=0;i<UATx_Length;i++){*/
       
          Putc_UART2(UART_Tx[0]);
          curStatus = KEY_STAT_Release; 
       }

						 else //if( ++keyPressTimer > LONG_PRESS_TIME )			
						 { 

							 curStatus = KEY_STAT_clicking; 
								//returnKeyValue = NO_KEY_PRESS;
						 } break;	
		// 		case KEY_STAT_longKeyPress: /* -----长按状态----- */			
		// 			   if( curKeyValue == NO_KEY_PRESS ) 
		// 				 {	// 判断按键有没有抬起，是则进入释放状态，否则按键连发 
		// 				  curStatus = KEY_STAT_Release;	
		// 				  returnKeyValue = NO_KEY_PRESS; 
		// 			   }
		// 			   else if( ++keyQuickTimer > QUICK_ACT_SPACE )
		// 				 {
		// 				   keyQuickTimer = 0;
		// 				   returnKeyValue = keyValueBackup;
		// 			   }break;		
				case KEY_STAT_Release:/* -----按键释放状态----- */	
						 keyValueBackup = NO_KEY_PRESS;
		//			   keyPressTimer = 0;
						 //keyQuickTimer =0;
//						 if(Flag_Uart_Send)
//       {
//          UART_Tx[0]|=0x80;/*for(i=0;i<UATx_Length;i++){*/

//          Putc_UART1(UART_Tx[0]);          
//          Putc_UART2(UART_Tx[0]);
//       }//printf("%d",'q');
						 curStatus = KEY_STAT_noKeyPress;			
						 break;	
				default:break;
			}		
//     Key_L=0;	 
// 		Key_L |= Key_1Line_Scan();	//普通键扫描
// 		Key_L |= Key_2Line_Scan();
// 		Key_L |= Key_3Line_Scan();
// 		Key_L |= Key_4Line_Scan();
// 		Key_L |= Key_5Line_Scan();	
// 		if(Key_L!=0)
// 		{	
// 		  OSTimeDlyHMSM(0, 0,0,5);
// 			if(Key_L!=0)	//单按键优先级最低
// 			{		
// 				Key_L=0;					
// 				Key_L|=Key_L1= Key_1Line_Scan();
// 				Key_L|=Key_L2= Key_2Line_Scan();
// 				Key_L|=Key_L3= Key_3Line_Scan();
// 				Key_L|=Key_L4= Key_4Line_Scan(); 
// 				Key_L|=Key_L5= Key_5Line_Scan();
// 				if(Key_L!=0)	//单按键优先级最低
// 				{	
// 					UART_Tx[0] = 'N';
// 					if(Key_L1!=0)
// 					{
// 						switch(Key_L1)
// 						{							 
// 							 case 0x00001:UART_Tx[1] = 'a';UART_Tx[2] = 0x81;Flag_Uart_Send = 1;break;//放大
// 							 case 0x00002:UART_Tx[1] = 'b';UART_Tx[2] = 0x80;Flag_Uart_Send = 1;break;//归中
// 							 case 0x00004:UART_Tx[1] = 'c';UART_Tx[2] = 0x7F;Flag_Uart_Send = 1;break;//缩小
//                case 0x00008:UART_Tx[1] = 'd';UART_Tx[2] = 0x7E;Flag_Uart_Send = 1;break;//缺省
// 							default :continue;
// 						}
// 					}
// 					if(Key_L2!=0)
// 					{
// 						switch(Key_L2)
// 						{
// 							case 0x00010:UART_Tx[1] = 'e';UART_Tx[2] = 0x7D;Flag_Uart_Send = 1;break;//调暗
// 							case 0x00020:UART_Tx[1] = 'f';UART_Tx[2] = 0x7C;Flag_Uart_Send = 1;break;//调亮
// 							case 0x00040:UART_Tx[1] = 'g';UART_Tx[2] = 0x7B;Flag_Uart_Send = 1;break;//航迹关
// 							case 0x00080:UART_Tx[1] = 'h';UART_Tx[2] = 0x7A;Flag_Uart_Send = 1;break;//航迹开
// 							default :continue;
// 						}
// 					}
// 					if(Key_L3!=0)
// 					{
// 						switch(Key_L3)
// 						{
// 							case 0x00100:UART_Tx[1] = 'i';UART_Tx[2] = 0x79;Flag_Uart_Send = 1;break;//监控关
// 							case 0x00200:UART_Tx[1] = 'j';UART_Tx[2] = 0x78;Flag_Uart_Send = 1;break;//监控开
// 							case 0x00400:UART_Tx[1] = 'k';UART_Tx[2] = 0x77;Flag_Uart_Send = 1;break;//←
// 							case 0x00800:UART_Tx[1] = 'l';UART_Tx[2] = 0x76;Flag_Uart_Send = 1;break;//↑
// 							default :continue;
// 						}
// 					}
// 					if(Key_L4!=0)
// 					{
// 						switch(Key_L4)
// 						{
// 							case 0x01000:UART_Tx[1] = 'm';UART_Tx[2] = 0x75;Flag_Uart_Send = 1;break;//↓
// 							case 0x02000:UART_Tx[1] = 'n';UART_Tx[2] = 0x74;Flag_Uart_Send = 1;break;//OK
// 							case 0x04000:UART_Tx[1] = 'o';UART_Tx[2] = 0x73;Flag_Uart_Send = 1;break;//→
//               case 0x08000:UART_Tx[1] = 'p';UART_Tx[2] = 0x72;Flag_Uart_Send = 1;break;//菜单							
// 							default :continue;
// 						}
// 					}
// 					if(Key_L5!=0)
// 					{
// 						switch(Key_L5)
// 						{
// 							case 0x10000:UART_Tx[1] = 'q';UART_Tx[2] = 0x71;Flag_Uart_Send = 1;break;//返回
// 							case 0x20000:UART_Tx[1] = 'r';UART_Tx[2] = 0x70;Flag_Uart_Send = 1;break;//缺省
// 							case 0x40000:UART_Tx[1] = 's';UART_Tx[2] = 0x69;Flag_Uart_Send = 1;break;//缺省
//               case 0x80000:UART_Tx[1] = 't';UART_Tx[2] = 0x68;Flag_Uart_Send = 1;break;//缺省								
// 							default :continue;
// 						}
// 					}
// 					if(Flag_Uart_Send )
// 					{
// 						Flag_Uart_Send = 0;for(i=0;i<UATx_Length;i++){Putc_UART2(UART_Tx[i]);}
// 						while(Key_1Line_Scan()!=0|Key_2Line_Scan()!=0|Key_3Line_Scan()!=0|Key_4Line_Scan()!=0|Key_5Line_Scan()!=0);
// 						UART_Tx[0]='C';for(i=0;i<UATx_Length;i++){Putc_UART2(UART_Tx[i]);}
// 					}
// 				}			
// 			 }          					 
// 		 }				
	}
}


