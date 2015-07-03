#ifndef __HZ_H__
#define __HZ_H__	 
#include <LPC177x_8x.H>
extern uint32_t SIZE_GBK16;
extern uint32_t SIZE_GBK24;
extern uint32_t SIZE_GBK32;


void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode);					//在指定位置显示一个汉字
void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode);	//在指定位置显示一个字符串 
void Show_Str_Mid(uint16_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t len);

#endif
