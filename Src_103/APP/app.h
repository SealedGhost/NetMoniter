#ifndef _APP_H_
#define _APP_H_
#include "ucos_ii.h"
#include "includes.h"
/**************** 用户任务声明 *******************/

#define MSG_QUEUE_TABNUM 20
void Task_LED(void *p_arg);
void Task_Translate(void *p_arg);
void Task_KEY(void *p_arg);

extern OS_EVENT *QSem;//
 extern OS_MEM   *PartitionPt;
 extern uint8_t  Partition[MSG_QUEUE_TABNUM][400];
extern  void *MsgQeueTb[MSG_QUEUE_TABNUM];


#endif //_APP_H_
