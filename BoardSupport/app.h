#include "analysis_function.h"
#include "boat_struct.h"
#include "Config.h"

//--消息类型定义
// typedef struct message_18 message_18;
// typedef struct message_24 message_24;
// typedef struct message_24_partA message_24_partA;
// typedef struct message_24_partB message_24_partB;
// typedef struct vender_id vender_id;
// typedef struct call_sign call_sign;
// typedef struct dimension_of_ship dimension_of_ship;

// struct message_18
// {
//   int message_id;
//   int repeat_indicator;
//   unsigned long user_id;
//   int spare;
//   int SOG;
//   int position_accuracy;
//   double longitude;
//   double latitude;
//   int COG;
//   int true_heading;
//   int time_stamp;
//   int spare2;
//   int class_B_unit_flag;
//   int class_B_display_flag;
//   int class_B_DSC_flag;
//   int class_B_band_flag;
//   int class_B_message_22_flag;
//   int mode_flag;
//   int RAIM_flag;
//   int communication_state_selector_flag;
//   int communication_state;
// };

// struct message_24_partA
// {
//   int message_id;
//   int repeat_indicator;
//   unsigned long user_id;
//   int part_number;
//   char name[20];
// };
// struct vender_id
// {
//   char manufacturer_id[3];
//   int unit_model_code;
//   unsigned long unit_serial_number;
// };

// struct call_sign
// {
// 	int f;
// };

// struct dimension_of_ship{
//   int A;
//   int B;
//   int C;
//   int D;
// };

// struct message_24_partB
// {
//   int message_id;
//   int repeat_indicator;
//   unsigned long user_id;
//   int part_number;
//   int type_of_ship_and_cargo_type;
//   vender_id vender_id_;
//   call_sign call_sign_;
//   dimension_of_ship dimension_of_ship_;
//   int type_of_electronic_position_fixing_device;
//   int spare;
// };

//////////////////////////////////////////////////
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
///////////////////////////////
char six2asc(int num)
{
  if(num<32)
    num=num|64;
  return num;
}
//////////////////////////////////////////////
long getMMSI(unsigned char* text, int start)
{
  int i=0,tmp_6_bin[28];
  long mmsi = 0;
  for(i=0;(text[start]!=',')&&(i<28);i++,start++)
    tmp_6_bin[i]=change_table(text[start]);

  mmsi  = ((tmp_6_bin[1]&15)<<26)|(tmp_6_bin[2]<<20)|(tmp_6_bin[3]<<14)|(tmp_6_bin[4]<<8)|(tmp_6_bin[5]<<2)|(tmp_6_bin[6]>>4);//mmsi
  return mmsi;     
}


message_18 translate_m18(unsigned char *text,int start)
{
  message_18 message_text;
  int i=0,tmp_6_bin[28];
//   int temp=0;
   int longitude=0;
   int latitude=0;
  for(i=0;(text[start]!=',')&&(i<28);i++,start++)
    tmp_6_bin[i]=change_table(text[start]);

  message_text.message_id=tmp_6_bin[0];//Message ID
  message_text.repeat_indicator=tmp_6_bin[1]>>4;//repeat_indicator
  message_text.user_id=((tmp_6_bin[1]&15)<<26)|(tmp_6_bin[2]<<20)|(tmp_6_bin[3]<<14)|(tmp_6_bin[4]<<8)|(tmp_6_bin[5]<<2)|(tmp_6_bin[6]>>4);//mmsi
  message_text.spare=((tmp_6_bin[6]&15)<<4)|(tmp_6_bin[7]>>2);//spare
  message_text.SOG=((tmp_6_bin[7]&3)<<8)|(tmp_6_bin[8]<<2)|(tmp_6_bin[9]>>4);//SOG
  message_text.position_accuracy=tmp_6_bin[9]&8;//position accuracy

  longitude=((tmp_6_bin[9]&7)<<25)|(tmp_6_bin[10]<<19)|(tmp_6_bin[11]<<13)|(tmp_6_bin[12]<<7)|(tmp_6_bin[13]<<1)|(tmp_6_bin[14]>>5);
  message_text.longitude=longitude/10;
  
  latitude=((tmp_6_bin[14]&31)<<22)|(tmp_6_bin[15]<<16)|(tmp_6_bin[16]<<10)|(tmp_6_bin[17]<<4)|(tmp_6_bin[18]>>2);
  message_text.latitude=latitude/10;

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
/////////////////////////////////////////////////////////////
message_24_partA translate_m24A(unsigned char *text,int start){
  message_24_partA message_text;
  char tmp  = 0;
  
  int i=0,tmp_6_bin[27],j=0;
  for(i=0;(text[start]!=',')&&(i<27);i++,start++)
		tmp_6_bin[i]=change_table(text[start]);
 
  message_text.message_id=tmp_6_bin[0];
  message_text.repeat_indicator=(tmp_6_bin[1]>>4);
  message_text.user_id=((tmp_6_bin[1]&15)<<26)|(tmp_6_bin[2]<<20)|(tmp_6_bin[3]<<14)|(tmp_6_bin[4]<<8)|(tmp_6_bin[5]<<2)|(tmp_6_bin[6]>>4);
  
  
  message_text.part_number=((tmp_6_bin[6]&12)>>2);
  for(i=0,j=6;i<19;i++,j++)
  {
    tmp  = six2asc(((tmp_6_bin[j]&3)<<4)|(tmp_6_bin[j+1]>>2));
    if(tmp != '@')
      message_text.name[i]=tmp;
    else
    {
      message_text.name[i]  = '\0';
//INFO("name:%s",message_text.name);      
      break;
    }
  }
  message_text.name[19]  = '\0';
  return message_text;
}

type_of_ship translate_m24B(unsigned char *text,int start){
	type_of_ship message_text;
  int i=0,tmp_6_bin[28];
  for(i=0;(text[start]!=',')&&(i<28);i++,start++)
		tmp_6_bin[i]=change_table(text[start]);
	
  message_text.message_id=tmp_6_bin[0];
  message_text.repeat_indicator=(tmp_6_bin[1]>>4);
  message_text.user_id=((tmp_6_bin[1]&15)<<26)|(tmp_6_bin[2]<<20)|(tmp_6_bin[3]<<14)|(tmp_6_bin[4]<<8)|(tmp_6_bin[5]<<2)|(tmp_6_bin[6]>>4);
  message_text.part_number=((tmp_6_bin[6]&12)>>2);
	 message_text.type_of_ship_and_cargo_type = (tmp_6_bin[6]<<6)|(tmp_6_bin[7]);
	 return message_text;
}

//函数声明
void UI_Task(void *p_arg);
void Insert_Task(void *p_arg);
void Refresh_Task(void *p_arg);
void App_TaskStart(void);
int translate_(unsigned char *text,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship);













