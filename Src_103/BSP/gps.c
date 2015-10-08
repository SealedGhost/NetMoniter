#include "gps.h"
#include "string.h"



static double Str_To_Double(unsigned char *buf)
{
	double rev = 0;
	double dat;
	int integer = 1;
	char *str = buf;
	int i;
	
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str++;
	}
	
	return rev;
}

u8 GetComma(u8 num, unsigned char *str)
{
	u8 i,j = 0;
	int len = strlen(str);

	for(i = 0; i < len; i++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	 
}

double Get_Double_Number_for_location(unsigned char *s)
{
	char buf[10];
	u8 i;
	double rev;
	long a,temp;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Double(buf);
	
	temp = rev/100;
	a = temp*600000 + (rev - temp*100)*10000;
	
	return a;	
}

double Get_Double_Number_for_sogcog(unsigned char *s)  //for sog cog
{
	char buf[10];
	u8 i;
	double rev;
	unsigned int a;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Double(buf);
	
	a = rev*10/1;
	
	return a;	
}

double Get_Double_Number_for_datetime(unsigned char *s)  //for time date
{
	char buf[10];
	u8 i;
	double rev;
	unsigned int a;
	
	i = GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = Str_To_Double(buf);
	
	a = rev/1;
	
	return a;	
}
