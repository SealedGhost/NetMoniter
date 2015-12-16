#ifndef  UART_H
#define  UART_H
#include <stm32f10x.h>
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "stdio.h"

#define UART_RX_LEN     400 
#define UART_TX_LEN     50


typedef struct
{
	uint8_t Alarm_number;
	uint8_t volume;
}Music_Info;

extern uint8_t Uart_Rx[UART_RX_LEN];//UART1Ω” ’
// extern Music_Info Music_Information;
// extern uint8_t Music_flag;
//extern uint8_t Uart_Tx[UART_TX_LEN];//UART2∑¢ÀÕ

//u8 Flag_Uart_Send = 0;

void GPIO_Configuration(void);
void NVIC_Configuration(void);
void DMA_Configuration(void);
void UART_Configuration(void);
void USART1_IRQHandler(void);

//void  Putc_UART2(u8 ch);
void  Putc_UART1(u8 ch);
//void  Putc_UART3(u8 ch);
#endif
