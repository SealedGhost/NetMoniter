
#include "stdio.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

void lpc1788_Uart_Init(int port);
void USER_Init(void); 
void UART_Config(unsigned char channel);
