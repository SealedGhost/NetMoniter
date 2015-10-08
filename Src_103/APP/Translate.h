#ifndef  Translate_H
#define  Translate_H
#include <stm32f10x.h>
#include "UART.h"  
// #include "boat_struct.h"
#include "analysis_function.h"

extern uint8_t Message_AIS[100];


void test11(void );
int translate_(unsigned char *text);//,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship);
int change_table(unsigned char ch);
message_18 translate_m18(unsigned char *text,int start);
message_24_partA translate_m24A(unsigned char *text,int start);


#endif
