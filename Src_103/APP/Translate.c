#include "Translate.h"
#include "gps.h"
#include "SPI.h"

u32 latitude  = 0;
u32 longitude  = 0;
u16 sog  = 0;
u16 cog  = 0;
u32 date  = 0;
u32 time  = 0;

static u8 gpscnt  = 0;




int translate_(unsigned char *text)//,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship)
{
  int i=0;
  int cnt  = 0;
  
  if((text[0]!='!')&&(text[0]!='$'))
  {    
    return 0;
  }
  
    if((text[1]=='A')&&(text[2]=='I')&&(text[3]=='V')&&(text[4]=='D')&&(text[5]=='M'))
    {
       for(i=6; i<20; i++)
       {
          if(text[i] == ',')
          { 
             cnt++;          
             if(cnt==5)
             {
                break;
             }
          }
       }      
       if(i<20)
       {
          if(text[i+1] == 'B' || text[i+1] == 'C')
          {          
             for(i=0; i<48; i++)
             {
                SPI2_SendByte(text[i]);
             }
             SPI2_SendByte('\r');
             SPI2_SendByte('\n');            
             return 18;
          }
          else if(text[i+1] == 'H')
          {
             for(i=0; i<48; i++)
             {
                SPI2_SendByte(text[i]);
             }
             SPI2_SendByte('\r');
             SPI2_SendByte('\n');
             return 24;
          }
       }
       
       return 0;
    }
  
  

	///else if((text[1]=='G')&&(text[2]=='P')&&(text[3]=='R')&&(text[4]=='M')&&(text[5]=='C')) //GPS GPRMC
 	else if((text[3]=='R')&&(text[4]=='M')&&(text[5]=='C'))
	{
		if(gpscnt == 0) 
		{
			
     latitude = Get_Double_Number_for_location(&text[GetComma( 3, text)]);
     longitude = Get_Double_Number_for_location(&text[GetComma( 5, text)]);
     
     sog = Get_Double_Number_for_sogcog(&text[GetComma( 7, text)]);
     cog = Get_Double_Number_for_sogcog(&text[GetComma( 8, text)]);

     date = Get_Double_Number_for_datetime(&text[GetComma( 9, text)]);
     time = Get_Double_Number_for_datetime(&text[GetComma( 1, text)]);
     
     text[6] = (u8)(latitude >> 24);
     text[7] = (u8)(latitude >> 16);
     text[8] = (u8)(latitude >> 8);
     text[9] = (u8)latitude;
     
     text[10] = (u8)(longitude >> 24);
     text[11] = (u8)(longitude >> 16);
     text[12] = (u8)(longitude >> 8);
     text[13] = (u8)longitude;
     
     text[14] = (u8)(sog >> 8);
     text[15] = (u8)sog;
    
     text[16] = (u8)(cog >> 8);
     text[17] = (u8)cog;

     text[18] = (u8)(date >> 24);
     text[19] = (u8)(date >> 16);
     text[20] = (u8)(date >> 8);
     text[21] = (u8)date;
     
     text[22] = (u8)(time >> 24);
     text[23] = (u8)(time >> 16);
     text[24] = (u8)(time >> 8);
     text[25] = (u8)time;
     
  //			for (i=0;i<48;i++)
  //				Putc_UART1(text[i]); //printf("%c",s[i]);
  //				Putc_UART1('\r');
  //				Putc_UART1('\n');
     for(i=0; i<48; i++)
     {
        SPI2_SendByte(text[i]);
     }
     SPI2_SendByte('\r');
     SPI2_SendByte('\n');
		}
  
  gpscnt++;
  gpscnt  = gpscnt%10;
		return 0;
	}
  else
		return 0;
}





int change_table(unsigned char ch)
{
  int temp_add_40=0,temp_add_sum=0,temp_final;
    temp_add_40=ch+40;
    if(temp_add_40>128)
    {
      temp_add_sum=temp_add_40+32;
    }
    else
    {
      temp_add_sum=temp_add_40+40;
    }
    temp_final=temp_add_sum&63;
    return temp_final;
}

