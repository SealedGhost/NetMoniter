#include <ucos_ii.h>
#include "stdio.h"
#include "uart.h"
#include "app.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_timer.h"
#include "Config.h"
#include "SysSetting.h"

//#ifndef test_test
//	#define test_test
//#endif

/* 定义任务优先级 */
#define UI_Task_PRIO       11
#define Insert_Task_PRIO      8
#define Refresh_Task_PRIO        9
#define Task_Stack_Use_PRIO  10  
/* 定义任务堆栈大小 */
#define USER_TASK_STACK_SIZE 384
#define TOUCH_TASK_STACK_SIZE 256
#define KEY_TASK_STACK_SIZE 128
#define Task_Stack_Use_STACK_SIZE 128
/* 定义任务堆栈 */
static	OS_STK	UI_Task_Stack[USER_TASK_STACK_SIZE];
static	OS_STK	Insert_Task_Stack[TOUCH_TASK_STACK_SIZE];
static	OS_STK	Refresh_Task_Stack[KEY_TASK_STACK_SIZE];
static  OS_STK  Task_Stack_Use_Stack[Task_Stack_Use_STACK_SIZE];

static  OS_STK_DATA UI_Task_Stack_Use;
static  OS_STK_DATA Insert_Task_Stack_Use;
static  OS_STK_DATA Refresh_Task_Stack_Use;

static volatile int myCnt  = 0;
static volatile int msgCnt  = 0;
void SysTick_Init(void);


extern boat mothership;
extern void MainTask(void);
extern void insert(boat* boats, struct message_18* p_msg);

///--消息队列的定义部分---
OS_EVENT *QSem;//定义消息队列指针
void *MsgQeueTb[MSG_QUEUE_TABNUM];//定义消息指针数组，队列长度为10
OS_MEM   *PartitionPt;//定义内存分区指针
// #pragma arm section rwdata = "SD_RAM2", zidata = "SD_RAM2"
uint8_t  Partition[MSG_QUEUE_TABNUM][100];
// #pragma arm section rwdata
// uint8_t  Partition[20][300]__attribute__((at(0xA1FF0000)));

int list_endIndex  = 0;

///* ADDRESS: 0xAC000000  SIZE: 0x400000  */
#pragma arm section rwdata = "SD_RAM1", zidata = "SD_RAM1"
_boat boat_list[BOAT_LIST_SIZE_MAX]; // 0x10000: 64K
_boat *boat_list_p[BOAT_LIST_SIZE_MAX];
_boat_m24A boat_list_24A[BOAT_LIST_SIZE_MAX];
_boat_m24A *boat_list_p24A[BOAT_LIST_SIZE_MAX];
#pragma arm section rwdata
//_boat boat_list[BOAT_LIST_SIZE_MAX]__attribute__((at(0xA1C00000)));
_boat boat_list[BOAT_LIST_SIZE_MAX]__attribute__((at(0xA1D00000)));

_boat *boat_list_p[BOAT_LIST_SIZE_MAX]__attribute__((at(0xA1E00000)));
_boat *boat_start = boat_list;
_boat *boat_end = boat_list;

_boat_m24A boat_list_24A[BOAT_LIST_SIZE_MAX]__attribute__((at(0xA1F00000)));;
_boat_m24A *boat_list_p24A[BOAT_LIST_SIZE_MAX]__attribute__((at(0xA1F80000)));
_boat_m24A *boat_start24A = boat_list_24A;
_boat_m24A *boat_end24A = boat_list_24A;

_boat_m24B boat_list_24B[BOAT_LIST_SIZE_MAX];
_boat_m24B *boat_lisp_p24B[BOAT_LIST_SIZE_MAX];
_boat_m24B *boat_start24B = boat_list_24B;
_boat_m24B *boat_end24B = boat_list_24B;

///* ADDRESS: 0xAC000000  SIZE: 0x400000  */

//_boat boat_list[BOAT_LIST_SIZE_MAX]; // 0x10000: 64K
//_boat *boat_list_p[BOAT_LIST_SIZE_MAX];
//_boat_m24A boat_list_24A[BOAT_LIST_SIZE_MAX];
//_boat_m24A *boat_list_p24A[BOAT_LIST_SIZE_MAX];

//_boat boat_list[BOAT_LIST_SIZE_MAX];
//_boat *boat_list_p[BOAT_LIST_SIZE_MAX];
//_boat *boat_start = boat_list;
//_boat *boat_end = boat_list;

//_boat_m24A boat_list_24A[BOAT_LIST_SIZE_MAX];
//_boat_m24A *boat_list_p24A[BOAT_LIST_SIZE_MAX];
//_boat_m24A *boat_start24A = boat_list_24A;
//_boat_m24A *boat_end24A = boat_list_24A;

//_boat_m24B boat_list_24B[BOAT_LIST_SIZE_MAX];
//_boat_m24B *boat_lisp_p24B[BOAT_LIST_SIZE_MAX];
//_boat_m24B *boat_start24B = boat_list_24B;
//_boat_m24B *boat_end24B = boat_list_24B;


struct message_18 msg_18;

short N_boat = 0;
_boat test[3];
_boat *test_p[500];
char name1[20]="MAN DE LI";
char name2[20]="ZHE DAI YU ";
SYS_SETTING SysSetting ={ {0,0},
                          {0,0},
													{0,0},
													UNITS_Km,
													1
                          };
SYS_SETTING *p_SysSetting = &SysSetting;											


void UI_Task(void *p_arg)/*描述(Description):	任务UI_Task*/
{
	
//	while(1)
//	{ 
//		OSTimeDly(200);	
		MainTask();
//	}
}
/*描述(Description):	任务Insert_Task*/
void Insert_Task(void *p_arg)  //等待接收采集到的数据
{ 
	uint8_t myVal  = 0;
	int tmp  = 0;
	uint8_t *s; 
	INT8U err;
//	static int a=0;
	message_18 text_out;
	message_24_partA text_out_24A;
	type_of_ship text_out_type_of_ship;
	
	while(1)
	{		 
		s = OSQPend(QSem,0,&err);
    
    tmp  = translate_(s,&text_out,&text_out_24A,&text_out_type_of_ship);

    
//    if(tmp>0 && tmp<28)
//    {
//      msgCnt++;
//      INFO("msgID:%d  msgCnt:%d",tmp,msgCnt);
//    }
//		switch(tmp)
//		{
//			case 18:      
//				insert_18(boat_list, &text_out);
////        boat_list[0].user_id  = msg_18.user_id;
//				break;
//			case 240:				
//				insert_24A(boat_list,&text_out_24A);
//				break;
//			case 241:			
//				insert_24B(boat_list,&text_out_type_of_ship);
//				break;
//			default:			
//				break;
//		}
    
    switch(tmp)
    {
   
       case 18:
         insert_18(boat_list, &text_out);
         break;
        case 240:
         insert_24A(boat_list, &text_out_24A);

//#if INFO_ENABLE
//INFO("insert 24A");         
//#endif
         break;
        case 241:
         insert_24B(boat_list, &text_out_type_of_ship);
         break;
        default:
         break;
    }
		OSMemPut(PartitionPt,s);


		OSTimeDly(20); 

	}
}
void Refresh_Task(void *p_arg)//任务Refresh_Task
{

	while(1)
	{
	
//		  OSTimeDly(30000); 		/* 延时8000ms */
// 		if(boat_list_p) free(boat_list_p);
// 		boat_list_p = (_boat**)malloc(sizeof(_boat*)*max_size);
		
		updateTimeStamp(boat_list);
		OSTimeDlyHMSM(0,0,3,0);
	}
}
void Task_Stack_Use(void *p_arg)
{
	OS_MEM_DATA MemInfo;
	
	while(1)
	{
		OSMemQuery(PartitionPt,&MemInfo);
//		OSTaskStkChk(UI_Task_PRIO ,&UI_Task_Stack_Use);
// 		OSTaskStkChk(Insert_Task_PRIO,&Insert_Task_Stack_Use);
//		OSTaskStkChk(Refresh_Task_PRIO,&Refresh_Task_Stack_Use);
// 		printf("\n\rUI_Task             used/free:%d/%d  usage:%%%d\r\n",UI_Task_Stack_Use.OSUsed,UI_Task_Stack_Use.OSFree,(UI_Task_Stack_Use.OSUsed*100)/(UI_Task_Stack_Use.OSUsed+UI_Task_Stack_Use.OSFree));
// 		printf("Insert_Task_Stack_Use  used/free:%d/%d  usage:%%%d\r\n",Insert_Task_Stack_Use.OSUsed,Insert_Task_Stack_Use.OSFree,(Insert_Task_Stack_Use.OSUsed*100)/(Insert_Task_Stack_Use.OSUsed+Insert_Task_Stack_Use.OSFree));		
// 		printf("Refresh_Task_Stack_Use    used/free:%d/%d  usage:%%%d\r\n",Refresh_Task_Stack_Use.OSUsed,Refresh_Task_Stack_Use.OSFree,(Refresh_Task_Stack_Use.OSUsed*100)/(Refresh_Task_Stack_Use.OSUsed+Refresh_Task_Stack_Use.OSFree));		
		printf("**********%d----------\n\r",MemInfo.OSNUsed);
		printf("**********%d**********\n\r",MemInfo.OSNFree);
		OSTimeDly(1000); 		/* 延时8000ms */
	}
}
void App_TaskStart(void)//初始化UCOS，初始化SysTick节拍，并创建三个任务
{
	INT8U err;
  
  int i  = 0;
//	N_boat = 3;	

	for(i=BOAT_LIST_SIZE_MAX;i>=0;i--)
  {
     boat_list[i].user_id  = 0;
  }
	
   mothership.latitude = 1927265;
   mothership.longitude = 7128660;
   mothership.true_heading  = 0;
//	msg_18.user_id  = 777777772;
//  msg_18.SOG      = 6;
//  msg_18.COG      = 2;
//  msg_18.longitude = 72630000;
//  msg_18.latitude  = 72630000;
//  
//  
//	test[0].user_id = 11029;
//	test[0].SOG = 5;
//	test[0].COG = 5;
//	test[0].isVisible = 1;
//	test[0].true_heading = 180;
//	test[0].longitude = 7305545;
//	test[0].latitude = 1940726;

//	
//	test[1].user_id = 19283;
//	test[0].SOG = 6;
//	test[0].COG = 8;
//	test[1].isVisible = 0;
//	test[1].true_heading = 60;
//	test[1].longitude = 7295545;
//	test[1].latitude = 1949726;


//	test[2].user_id = 19289;
//	test[0].SOG = 9;
//	test[0].COG = 23;
//	test[2].isVisible = 1;
//	test[2].true_heading = 270;
//	test[2].longitude = 7299545;
//	test[2].latitude = 1955726;

//	
//// 	test_p = (_boat**)malloc(sizeof(_boat*)*3);
//	test_p[0] = &test[0];
//	test_p[1] = &test[1];
//	test_p[2] = &test[2];

//	test_p[0] = &test[0];
//	test_p[1] = &test[1];
//	test_p[2] = &test[2];
//  
  
  
	OSInit();
	SysTick_Init();/* 初始化SysTick定时器 */
	QSem = OSQCreate(&MsgQeueTb[0],MSG_QUEUE_TABNUM); //创建消息队列，10条消息
	PartitionPt=OSMemCreate(Partition,MSG_QUEUE_TABNUM,100,&err);
	
	OSTaskCreateExt(UI_Task, (void *)0,(OS_STK *)&UI_Task_Stack[USER_TASK_STACK_SIZE-1],  UI_Task_PRIO, UI_Task_PRIO, (OS_STK *)&UI_Task_Stack[0], USER_TASK_STACK_SIZE,(void*)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* 创建任务 UI_Task */
	OSTaskCreateExt(Insert_Task,(void *)0,(OS_STK *)&Insert_Task_Stack[TOUCH_TASK_STACK_SIZE-1],Insert_Task_PRIO,Insert_Task_PRIO,(OS_STK *)&Insert_Task_Stack[0],TOUCH_TASK_STACK_SIZE,(void*)0,OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* 创建任务 Insert_Task */
	OSTaskCreateExt(Refresh_Task,  (void *)0,(OS_STK *)&Refresh_Task_Stack[KEY_TASK_STACK_SIZE-1],    Refresh_Task_PRIO,  Refresh_Task_PRIO  ,(OS_STK *)&Refresh_Task_Stack[0],  KEY_TASK_STACK_SIZE,(void*)0,  OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);/* 创建任务 Refresh_Task */
//	OSTaskCreate(Task_Stack_Use,(void *)0,(OS_STK *)&Task_Stack_Use_Stack[Task_Stack_Use_STACK_SIZE-1],  Task_Stack_Use_PRIO);/* 创建任务 Refresh_Task */

	OSStart();
}

//		switch(translate_(s,&text_out,&text_out_24A,&text_out_type_of_ship))
int translate_(unsigned char *text,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship)
{
  int i=0,comma=0;
  int tmp  = 0;
  if((text[0]!='!')&&(text[0]!='$'))
    return 0;
  if((text[1]=='A')&&(text[2]==0x49)&&(text[3]=='V')&&(text[4]=='D')&&(text[5]=='M'))
  {
    for(i=0;comma<6;i++){
	   if(text[i]==',')
	    comma++;
	   if(comma==5){
	    i++;
      
     tmp  = change_table(text[i])   ;
     
	   switch(tmp){
	   case 18: 
	          (*text_out)=translate_m18(text,i);

								 return 18;

			   case 24:
                 if(change_table(text[i+6])&12){
										*text_out_type_of_ship = translate_m24B(text,i);
										return 241;
								 }
					       else
								 {
									 *text_out_24A = translate_m24A(text,i); 
									 return 240;
				         }
                      
					default:
              return tmp;
         
	     }
	   }
   }
  }
   else
   return 0;
return 0;
   
   

}

/************************************* End *************************************/
