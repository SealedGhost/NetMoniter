#include "str.h"
#include <stdio.h>
#include <string.h>


char strBuf[20]  = "?????.???";
char* pStrBuf = strBuf;



void disttostr( char * str, int num)
{

   if(num > 99999)
      sprintf(str, "%s", "????");
   else if(num > 9999)
      sprintf(str, "%2d.%02d",num/1000, (num%1000)/10);
   else 
      sprintf(str, "%0d.%03d", num/1000, (num%1000)); 
}


long strtoi(const char*str)
{
	long val = 0;
	int i = 0;
 int len  = 0;
 
	
 if(str != NULL)
 {
 
    len  = strlen(str); 
    for(i=0; i<len; i++)
    {
       if(str[i]>='0'  &&  str[i]<='9')
       {
          val  = val*10 + str[i]-'0';
       }
       else if(str[i] != ' ')
       {     
          return 0;
       }
    }
 }
	return val;
}



void lltostr(long l_o_l,char *str)
{
	int degree  = 0;
	int minute_int  = 0;
	int minute_dec  = 0;
	char tmp[10];
 if( l_o_l==0)
 {
    str[0]  = '?';
    str[1]  = '?';
    str[2]  = '?';
    str[3]  = 194;
    str[4]  = 176;
    str[5]  = '?';
    str[6]  = '?';
    str[7]  = '.';
    str[8] = '?';
    str[9] = '?';
    str[10] = '?';
    str[11] = '\0';
 }
 else
 {
    degree = l_o_l/(60000);
    minute_int = (l_o_l%(60000))/1000;
    minute_dec = (l_o_l%(60000))-minute_int*1000;
    
   //	ttoi(degree,tmp);
    sprintf(str,"%03d",degree);
    str[3]  = 194;
    str[4]  = 176;
    sprintf(str+5,"%02d",minute_int);
    str[7] = '.';
    sprintf(str+8,"%03d",minute_dec); 
 }

}


char * ttoi(long num, char * str)
{
    char *index = "0123456789",temp;
    long unum=num;
    int i=0,j=0,js,s;
    do{
			str[i++]=index[unum%10];
			unum/=10;
    }while(unum);
    str[i]='\0';
    js=(i-1)/2;
    for(j=0;j<=js;j++){
			s=i-1-j;
			temp = str[j];
			str[j]=str[s];
			str[s]=temp;
    }
    return str;
}


void str_trim(char * pszSrc, int nMaxLen)
{
   int i  = 0;
  
   for(; i< nMaxLen-1; i++)
   {
      if(pszSrc[i] == 0)
         break;
   }
      
   for(; i; i--)
   {
      if(pszSrc[i] == 0 || pszSrc[i] == 32)
      {
         pszSrc[i]  = 0;      
      }
      else
      {
         return;
      }
   }
   return;   
}


/** @brief      判断两个相同类型结构体是否相等
 *
 *  @dscrp 
 *  @para [in]  两个结构体的起始地址和结构体的大小
 *  @para [out] 如果有地址为NULL，或者不相等，返回FALSE。否则返回TRUE
 */
Bool Mem_isEqual(void* _pSrc, void* _pDst, unsigned int nSize)
{
   unsigned char* pSrc  = (unsigned char*)_pSrc;
   unsigned char* pDst  = (unsigned char*)_pDst;
   
   if(pStrBuf == NULL  ||  pDst == NULL)
      return FALSE;
   
   while(nSize)
   {
      if(pSrc[nSize-1] != pDst[nSize-1])
      {
         return FALSE;
      }
      else
      {
         nSize--;
      }
   }
   return TRUE;
}

