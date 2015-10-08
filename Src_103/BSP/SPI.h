#ifndef __SPI_H
#define	__SPI_H
#include <stm32f10x.h>

#define SPI2_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12) 
#define SPI2_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12) 

void SPI2_Init(void);

u8 SPI2_SendByte(u8 byte)	;






























#endif
