#include <LPC177x_8x.H>
#include "HZ.h"
#include "exfuns.h"
#include "lcd43.h"
#include "uart.h"
//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
uint32_t SIZE_GBK16;
uint32_t SIZE_GBK24;
uint32_t SIZE_GBK32;

//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	   
void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode)
{
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t *dzk;   
	uint8_t csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	qh=*font;
	ql=*(++font);	
	if(size!=32&&size!=16&&size!=24)return;	//不支持的size	
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<csize;i++)*dzk++=0x00;//填充满格
	    return; //结束访问
	} 	
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		
	switch(size)
	{
		case 16:
			      dzk=(unsigned char*)GBK16_BASEADDR+foffset;	
		        break;
		case 24:
			      dzk=(unsigned char*)GBK24_BASEADDR+foffset;  			     
						break;
		case 32:
			      dzk=(unsigned char*)GBK32_BASEADDR+foffset; 
			      break;		
	}	
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据 	
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)   LCD_SetPixel(LCD_PANEL_UPPER,x,y,BLACK);
			else if(mode==0)LCD_SetPixel(LCD_PANEL_UPPER,x,y,BLACK); 
			temp=temp<<1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode)
{					
	  uint16_t x0=x;
	  uint16_t y0=y;							  	  
    uint8_t bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
			if(!bHz)
			{
				if(*str>0x80)bHz=1;//中文 
				else              //字符
				{      
					if(x>(x0+width-size/2))//换行
			    {				   
						y+=size;
						x=x0;	   
			    }							    
					if(y>(y0+height-size))break;//越界返回      
					if(*str==13)//换行符号
					{         
							y+=size;
				      x=x0;
							str++; 
					}  
					else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入 
			    str++; 
					x+=size/2; //字符,为全字的一半 
				}
			}
			else//中文 
      {     
				bHz=0;//有汉字库    
				if(x>(x0+width-size))//换行
			  { 	    
					y+=size;
					x=x0;		  
			  }
	      if(y>(y0+height-size))break;//越界返回  						     
	      Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
	      str+=2; 
	      x+=size;//下一个汉字偏移	    
      }						 
    }   
}  			 		 
// //在指定宽度的中间显示字符串
// //如果字符长度超过了len,则用Show_Str显示
// //len:指定要显示的宽度			  
// void Show_Str_Mid(uint16_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t len)
// {
// 	uint16_t strlenth=0;
//    	strlenth=strlen((const char*)str);
// 	strlenth*=size/2;
// 	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
// 	else
// 	{
// 		strlenth=(len-strlenth)/2;
// 	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
// 	}
// }   

























		  






