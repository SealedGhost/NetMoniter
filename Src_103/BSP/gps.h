#ifndef __GPS_H
#define __GPS_H

#include "stm32f10x.h"

typedef unsigned char u8;


extern u8 GetComma(u8 num, unsigned char *str);
extern double Get_Double_Number_for_location(unsigned char *s);
extern double Get_Double_Number_for_sogcog(unsigned char *s);
extern double Get_Double_Number_for_datetime(unsigned char *s);

#endif
