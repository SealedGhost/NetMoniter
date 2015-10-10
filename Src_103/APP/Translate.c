#include "Translate.h"

int translate_(unsigned char *text)//,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship)
{
//  int i=0,comma=0;
  if((text[0]!='!')&&(text[0]!='$'))
    return 0;
  if((text[1]=='A')&&(text[2]==0x49)&&(text[3]=='V')&&(text[4]=='D')&&(text[5]=='M'))
  {
		if (text[15] == 'B' )
			{
				return 18;
			}
			else if (text[15] == 'H')
			{
        return 24;
			}
//     for(i=0;comma<6;i++)
// 		{
// 	   if(text[i]==',')
// 	    comma++;
// 	   if(comma==5)
// 		 {
// 	     i++;
// 			 if(text[i]=='B')
// 				 return 18;
// 			 else if(text[i]=='H')
// 				 return 24;
// 			 else return 0;
// // 	     switch(change_table(text[i]))
// // 		   {
// // 	       case 18:return 18;
// // 								 //break;
// // 			   case 24:if(change_table(text[i+6])&12)
// // 					       {
// //  										return 0;
// // 								  }
// // 					        else
// // 								  {
// // 									   return 24;
// // 				          }
// // 					default:return 0;
// // 	     }
// 	   }
//    }
  }
   else
      return 0;
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

