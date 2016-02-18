#include "sound.h"
#include "uart.h"


uint8_t VOL[6][5]  = { {0x7e, 0x03, 0x31,  5, 0xef},
                       {0x7e, 0x03, 0x31, 10, 0xef},
                       {0x7e, 0x03, 0x31, 15, 0xef},                              
                       {0x7e, 0x03, 0x31, 20, 0xef},                              
                       {0x7e, 0x03, 0x31, 25, 0xef}, 
                       {0x7e, 0x03, 0x31, 30, 0xef}                   
                      };



void SND_SetVol(uint8_t SndVol)
{
   uint8_t vol  = 0;
   if(SndVol < SND_VOL_MIN)
      vol  = SND_VOL_MIN;
   else if(SndVol > SND_VOL_MAX)
      vol  = SND_VOL_MAX;
   else 
      vol  = SndVol;
      
   UART_Send(UART_0, VOL[vol], 5, BLOCKING);
}


void SND_Stop()
{
   UART_SendByte(UART_0, 0x7e);
   UART_SendByte(UART_0, 0x02);
   UART_SendByte(UART_0, 0x0e);
   UART_SendByte(UART_0, 0xef);
}

//void SND_Play()
//{
//   UART_SendByte(UART_0, 0x7e);
//   UART_SendByte(UART_0, 0x01);
//   UART_SendByte(UART_0, 0x0e);
//   UART_SendByte(UART_0, 0xef);
//}


void SND_Play(uint8_t id)
{
   if(id<=SND_ID_NM)
   {
      UART_SendByte(UART_0,0x7e);
      UART_SendByte(UART_0,0x04);
      UART_SendByte(UART_0,0x41);
      UART_SendByte(UART_0,0x00);
      if(id)
         UART_SendByte(UART_0, id);
      else 
         UART_SendByte(UART_0, SND_ID_ZRO);
      UART_SendByte(UART_0,0xef);
   }
}



void SND_ParseDist(int dist, uint8_t* pNums)
{
   if(dist < 99999)
   {
//printf("dist:%d  %d.%d\n\r",dist ,dist/1000, dist%1000/100);
      /// Dist >= 20 nm
      if(dist > 19999)
      {
         if(dist > 999) /// Dist >= 21 nm
         {
            pNums[0]  = dist/10000;
            pNums[1]  = SND_ID_TEN;
            pNums[2]  = dist%10000/1000;
         }
         else
         {
            pNums[0]  = 0;
            pNums[1]  = dist/10000;
            pNums[2]  = SND_ID_TEN;
         }
      }
      /// Dist >= 10 nm
      else if(dist > 9999)
      {
         if(dist >999)
         {
            pNums[0]  = 0;
            pNums[1]  = SND_ID_TEN;
            pNums[2]  = dist%10000/1000;
         }
         else
         {
            pNums[0]  = 0;
            pNums[1]  = 0;
            pNums[2]  = SND_ID_TEN;
         }
      }
      else if(dist > 99)
      {
         if(dist >999)
           pNums[0]  = dist /1000;
         else
           pNums[0]  = SND_ID_ZRO;
         pNums[1]  = SND_ID_DOT;
         pNums[2]  = dist%1000/100;
         if(pNums[2] == 0)
            pNums[2]  = SND_ID_ZRO;
      }
      else
      {
         pNums[0]  = 0;
         pNums[1]  = 0;
         pNums[2]  = SND_ID_ZRO;
      } 

//printf(" %d %d %d\n\r", pNums[0], pNums[1], pNums[2]);      
   }
}















