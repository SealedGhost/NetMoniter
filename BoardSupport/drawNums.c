#include "drawNums.h"


static void _drawNum(unsigned char num, short x, short y, short penSize)
{
   int i  = 0;
   short size  = penSize-1;
   
   if(num&0x01)
      GUI_FillRect(x,   y,   x+99,   y+size);
   if(num&0x02)
      GUI_FillRect(x+99-size, y, x+99, y+99);
   if(num&0x04)
      GUI_FillRect(x+99-size, y+100, x+99,y+199);
   if(num&0x08)
      GUI_FillRect(x, y+199-size, x+99, y+199);
   if(num&0x10)
      GUI_FillRect(x, y+100, x+size,y+199);
   if(num&0x20)
      GUI_FillRect(x, y, x+size, y+99);
   if(num&0x40)
      GUI_FillRect(x, y+99-size/2, x+99, y+99+size/2);
}

void 