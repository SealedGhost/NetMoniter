#include <stdio.h>
#include <stdlib.h>
#include "analysis_function.h"
#ifndef test
	#define test 
#endif
int change_table(unsigned char ch){
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
char six2asc(int num){
  if(num<32)
    num=num|64;
  return num;
}
message_18 translate_m18(unsigned char *text,int start){
  message_18 message_text;
  int i=0,tmp_6_bin[28];
 // int temp=0;
  long longitude=0;
  long latitude=0;
  for(i=0;(text[start]!=',')&&(i<28);i++,start++)
    tmp_6_bin[i]=change_table(text[start]);

  message_text.message_id=tmp_6_bin[0];//Message ID
  message_text.repeat_indicator=tmp_6_bin[1]>>4;//repeat_indicator
  message_text.user_id=((tmp_6_bin[1]&15)<<26)|(tmp_6_bin[2]<<20)|(tmp_6_bin[3]<<14)|(tmp_6_bin[4]<<8)|(tmp_6_bin[5]<<2)|(tmp_6_bin[6]>>4);//mmsi
  message_text.spare=((tmp_6_bin[6]&15)<<4)|(tmp_6_bin[7]>>2);//spare
  message_text.SOG=((tmp_6_bin[7]&3)<<8)|(tmp_6_bin[8]<<2)|(tmp_6_bin[9]>>4);//SOG
  message_text.position_accuracy=tmp_6_bin[9]&8;//position accuracy

  longitude=((tmp_6_bin[9]&7)<<25)|(tmp_6_bin[10]<<19)|(tmp_6_bin[11]<<13)|(tmp_6_bin[12]<<7)|(tmp_6_bin[13]<<1)|(tmp_6_bin[14]>>5);
  message_text.longitude=longitude/60.0/10000;
  
  latitude=((tmp_6_bin[14]&31)<<22)|(tmp_6_bin[15]<<16)|(tmp_6_bin[16]<<10)|(tmp_6_bin[17]<<4)|(tmp_6_bin[18]>>2);
  message_text.latitude=latitude/60.0/10000;

  message_text.COG=((tmp_6_bin[18]&3)<<10)|(tmp_6_bin[19]<<4)|(tmp_6_bin[20]>>2);
  message_text.true_heading=((tmp_6_bin[20]&3)<<7)|(tmp_6_bin[21]<<1)|(tmp_6_bin[22]>>5);
  message_text.time_stamp=((tmp_6_bin[22]&31)<<1)|(tmp_6_bin[23]>>5);
  message_text.spare2=(tmp_6_bin[23]&24);
  message_text.class_B_unit_flag=((tmp_6_bin[23]&4)>>2);
  message_text.class_B_display_flag=((tmp_6_bin[23]&2)>>1);
  message_text.class_B_DSC_flag=(tmp_6_bin[23]&1);
  message_text.class_B_band_flag=((tmp_6_bin[24]&32)>>5);
  message_text.class_B_message_22_flag=((tmp_6_bin[24]&16)>>4);
  message_text.mode_flag=((tmp_6_bin[24]&8)>>3);
  message_text.RAIM_flag=((tmp_6_bin[24]&4)>>2);
  message_text.communication_state_selector_flag=((tmp_6_bin[24]&2)>>1);
  message_text.communication_state=((tmp_6_bin[24]&1)<<18)|(tmp_6_bin[25]<<12)|(tmp_6_bin[26]<<6)|tmp_6_bin[27];
  return  message_text;
}

message_24_partA translate_m24A(unsigned char *text,int start){
  message_24_partA message_text;
  int i=0,tmp_6_bin[27],j=0;
  for(i=0;(text[start]!=',')&&(i<27);i++,start++)
    tmp_6_bin[i]=change_table(text[start]);
  
  message_text.message_id=tmp_6_bin[0];
  message_text.repeat_indicator=(tmp_6_bin[1]>>4);
  message_text.user_id=((tmp_6_bin[1]&15)<<26)|(tmp_6_bin[2]<<20)|(tmp_6_bin[3]<<14)|(tmp_6_bin[4]<<8)|(tmp_6_bin[5]<<2)|(tmp_6_bin[6]>>4);
  message_text.part_number=((tmp_6_bin[6]&12)>>2);
  for(i=0,j=6;i<20;i++,j++){
    message_text.name[i]=six2asc(((tmp_6_bin[j]&3)<<4)|(tmp_6_bin[j+1]>>2));
  }
  return message_text;
}

type_of_ship translate_m24B(unsigned char *text,int start){
	type_of_ship message_text;
  int i=0,tmp_6_bin[28];//,j=0;
  for(i=0;(text[start]!=',')&&(i<28);i++,start++)
		tmp_6_bin[i]=change_table(text[start]);
	
  message_text.message_id=tmp_6_bin[0];
  message_text.repeat_indicator=(tmp_6_bin[1]>>4);
  message_text.user_id=((tmp_6_bin[1]&15)<<26)|(tmp_6_bin[2]<<20)|(tmp_6_bin[3]<<14)|(tmp_6_bin[4]<<8)|(tmp_6_bin[5]<<2)|(tmp_6_bin[6]>>4);
  message_text.part_number=((tmp_6_bin[6]&12)>>2);
	message_text.type_of_ship_and_cargo_type = (tmp_6_bin[6]<<6)|(tmp_6_bin[7]);
	return message_text;
}

int translate_(unsigned char *text,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship)
{
  int i=0,comma=0;//,j=0;
  if((text[0]!='!')&&(text[0]!='$'))return 0;
  if((text[1]=='A')&&(text[2]==0x49)&&(text[3]=='V')&&(text[4]=='D')&&(text[5]=='M'))
  {
    for(i=0;comma<6;i++)
		{
	   if(text[i]==',')
	     comma++;
	   if(comma==5)
		 {
	     i++;
	     switch(change_table(text[i]))
			 {
	      case 18:
	             (*text_out)=translate_m18(text,i);
							 #ifdef test
// 								 printf("%d\n\n",change_table('O'));
 							     printf("message_id:%d\n",text_out->message_id);
// 								 printf("repeat_indicator:%d\n",text_out.repeat_indicator);
// 								 printf("user_id:%d\n",text_out.user_id);
// 								 printf("spare:%d\n",text_out.spare);
// 								 printf("SOG:%d\n",text_out.SOG);
// 								 printf("position_accuracy:%d\n",text_out.position_accuracy);
// 								 printf("longitude:%lf\n",text_out.longitude);
// 								 printf("latitude:%lf\n",text_out.latitude);
// 								 printf("COG:%d\n",text_out.COG);
// 							 	 printf("true_heading:%d\n",text_out.true_heading);
// 								 printf("time_stamp:%d\n",text_out.time_stamp);
// 								 printf("spare2:%d\n",text_out.spare2);
// 								 printf("class_B_unit_flag:%d\n",text_out.class_B_unit_flag);
// 								 printf("class_B_display_flag:%d\n",text_out.class_B_display_flag);
// 								 printf("class_B_DSC_flag:%d\n",text_out.class_B_DSC_flag);
// 								 printf("class_B_band_flag:%d\n",text_out.class_B_band_flag);
// 								 printf("class_B_message_22_flag:%d\n",text_out.class_B_message_22_flag);
// 								 printf("mode_flag:%d\n",text_out.mode_flag);
// 								 printf("RAIM_FLAG:%d\n",text_out.RAIM_flag);
// 								 printf("communication_state_selector_flag:%d\n",text_out.communication_state_selector_flag);
// 								 printf("communication_state:%d\n",text_out.communication_state);
								 #endif
								 return 18;
			   case 24:if(change_table(text[i+6])&12)
					       {
										*text_out_type_of_ship = translate_m24B(text,i);
										#ifdef test
										printf("message_id:%d B\n",text_out_type_of_ship->message_id);
										#endif
									  return 241;
								 }
					       else
								 {
									 *text_out_24A = translate_m24A(text,i); 
									 #ifdef test
 									 printf("message_id:%d A\n",text_out_24A->message_id);
// 									 printf("repeat_indicator:%d\n",text_out24A.repeat_indicator);
// 									 printf("user_id:%d\n",text_out24A.user_id);
// 									 printf("part_number:%d\n",text_out24A.part_number);
// 									 printf("name:\n");
// 									 for(j=0;j<20;j++)
// 									 {printf("%c",text_out24A.name[j]);}
// 									 printf("\n\n");
									 #endif
									 return 240;
				        }
					default:return 0;
	     }
	   }
   }
  }
   else return 0;	 
	 return 0;
}
