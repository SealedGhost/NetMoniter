#include "boat_struct.h"
#include "analysis_function.h"
#include "uart.h"
#include "Config.h"
#include "string.h"
#include <math.h>
#include <stdlib.h>
// find if id exist in aux_boat, yes return 1, otherwise 0, size exclusive
#define alloc_boat_mem (_boat**)malloc(sizeof(_boat*)*5000);

#define NOT_ALMOST(x, y)  (x>y?(x-y>10):(y-x>10))

#define PI 3.14
const unsigned int  R_KM  = 6371;
const unsigned int  R_NM  = (unsigned int)(0.54*R_KM);
const float         LLTOA = PI/60000/180;
extern int list_endIndex;

extern unsigned char * pStrBuf;
extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];
extern boat mothership;

static double getSphereDist(long lg_1,long lt_1, long lg_2, long lt_2);
void myftoa(unsigned char * str, float num);
//float getSphereDist(long lg_1,long lt_1, long lg_2, long lt_2);

/**
* 
*
*/
	void insert_18(boat * boats,  struct message_18* p_msg )
  {
     int i  = 0;
     long tmp  = 0;
     
     ///若本条消息的ID为 0 放弃本次插入
     if(p_msg->user_id == 0)
       return;

    /// 若本条消息的经纬度超出母船 30分 放弃本次插入     
     if( (p_msg->longitude < mothership.longitude-30000)  ||  (p_msg->longitude > mothership.longitude+30000) ) 
        return;
     if( (p_msg->latitude < mothership.latitude-30000)  ||  (p_msg->latitude > mothership.latitude+30000) )
        return;
        
     ///若此船已存在，刷新各项参数
     for(i=0;i<=list_endIndex;i++)
     {
        if(boats[i].user_id  == p_msg->user_id)
        {
//printf("\r\nupdate 18  at [%3d]\r\n",i);    
           if(   NOT_ALMOST(p_msg->longitude,boats[i].longitude)  
              || NOT_ALMOST(p_msg->latitude,boats[i].latitude)    )  
           {              
              boats[i].SOG          = p_msg->SOG;
              boats[i].COG          = p_msg->COG;
              boats[i].true_heading = p_msg->true_heading;
           
              boats[i].longitude    = p_msg->longitude;
              boats[i].latitude     = p_msg->latitude;
              boats[i].dist         = getSphereDist(boats[i].latitude, boats[i].longitude, mothership.latitude, mothership.longitude);
              if(boats[i].dist - 100.0  > 0)
                 boats[i].dist /= 10;
           }
           else
           {
//printf("\r\ntoo nearly\r\n");     
             ;      
           }
//           tmp  = p_msg->longitude/10;
//           if( (tmp >= mothership.longitude-30000)  &&  (tmp <= mothership.longitude+30000) )  /// WARNING: ，若本行打开，经度一直为 0 还不知道原因
//              boats[i].longitude  = tmp;
//           tmp  = p_msg->latitude/10;
//           if( (tmp >= mothership.latitude-30000)  &&  (tmp <= mothership.latitude+30000) )   ///本行打开 纬度也正常
//              boats[i].latitude  = tmp;
           boats[i].time_cnt     = TIMESTAMP; ///刷新时间戳变量

           return ;
        }
     }
     
     ///若此船是新的，找到数组中的空位，插入此船信息
     for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
     {
     
        if(boats[i].user_id == 0)
        {
//printf("\r\ninsert 18  at [%3d]\r\n",i);        
           boats[i].user_id      = p_msg->user_id;
           boats[i].SOG          = p_msg->SOG;
           boats[i].COG          = p_msg->COG;
           boats[i].true_heading = p_msg->true_heading;
           boats[i].longitude    = p_msg->longitude;
           boats[i].latitude     = p_msg->latitude;
           boats[i].dist         = getSphereDist(boats[i].latitude, boats[i].longitude, mothership.latitude, mothership.longitude);
           if(boats[i].dist - 100.0  > 0)
              boats[i].dist /= 10;
//printf("insert dist:%lf:",boats[i].dist);           
//           tmp  = p_msg->longitude/10;
//INFO("tmp:%ld",tmp);           
//           if( (tmp >= mothership.longitude-30000)  &&  (tmp <= mothership.longitude+30000) )
//           {
//              boats[i].longitude  = tmp;
//INFO("boats[%d].lg = %ld",i, boats[i].longitude);              
//           }           

//           tmp  = p_msg->latitude/10;
//INFO("tmp:%ld",tmp);           
//           if( (tmp >= mothership.latitude-30000)  &&  (tmp <= mothership.latitude+30000) )
//           {
//              boats[i].latitude  = tmp;
//INFO("boats[%d].lt = %ld",i, boats[i].latitude);              
//           }    
           boats[i].time_cnt     = TIMESTAMP;
           
           
            /// |___________________|_______| <--boats[]
            ///                     ^       
            ///                     |
            ///                list_endIndex
           ///若插入位置已超出尾端索引，更新尾端索引                          
           if(i > list_endIndex) 
           {
              list_endIndex  = i;            
           }
           return ;                                                                   
        }                                                                             
     }
     

     INFO("Error:boat list full.");

  }
  
  
  void insert_24A(boat* boats, struct message_24_partA* p_msg)
  {
     int i  = 0;
     int tmp  = 0;
   
     if(p_msg->user_id == 0)
       return ;
     
     ///此船存在
     for(i=0;i<=list_endIndex;i++)
     {     
        if(boats[i].user_id == p_msg->user_id)
        {
//printf("\r\nupdate 24A at [%3d]\r\n",i);        
//           memcpy(boats[i].name, p_msg,20); ///用memcpy 有问题

           ///若此结构体原来没有存船名，（即先收到该id船舶的18或24B消息）
           if(boats[i].name[0] == 0)
           {            
              for(tmp=0;tmp<20;tmp++)
              {
                 boats[i].name[tmp]  = p_msg->name[tmp];
                 if(p_msg->name[tmp] == '\0')
                 {                    
                    break;
                 }                 
                    
              }            
           }
           boats[i].time_cnt  = TIMESTAMP;
          
           return;
        }
     }
     
     ///此船为新的
     for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
     { 
        if(boats[i].user_id == 0)
        {
//printf("\r\ninsert 24A at [%3d]\r\n",i);        
           boats[i].user_id  = p_msg->user_id;
           
           for(tmp=0;tmp<20;tmp++)
           {
              boats[i].name[tmp]  = p_msg->name[tmp];
              if(p_msg->name[tmp] == '\0')
              {                  
                 break;
              }
                 
           } 
           boats[i].time_cnt     = TIMESTAMP;           

           if(i > list_endIndex)
           {
              list_endIndex  = i;
              
           }           
           return;
        }
     }
     

     INFO("Error:boat list full.");
  }
  
  
  void insert_24B(boat* boats,  struct type_of_ship* p_msg)
  {
     int i  = 0;
     
     if(p_msg->user_id == 0)
       return;
     
     for(i=0;i<=list_endIndex;i++)
     {
        if(boats[i].user_id == p_msg->user_id)
        {
//printf("\r\nupdate 24B at [%3d]\r\n",i);        
           boats[i].type_of_electronic_position_fixing_device  = p_msg->type_of_ship_and_cargo_type;
           boats[i].time_cnt  = TIMESTAMP;
              return;
        } 
     }
      
     for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
     {
     
        if(boats[i].user_id == 0)
        {
//printf("\r\ninsert 24B at [%3d]\r\n",i);        
           boats[i].type_of_electronic_position_fixing_device  = p_msg->type_of_ship_and_cargo_type;
           boats[i].time_cnt     = TIMESTAMP;           
           if(i > list_endIndex) list_endIndex  = i;
              return;
        }
     }

     INFO("Error:boat list full.");     
  }
  
 ///每隔若干秒后调用本函数，遍历boats数组对所有船舶的time_cnt变量减一，若有船的time_cnt减为 0 ，
 ///则删去此船（将存放此船的boats[i].user_id设置为0，即可） 
  void updateTimeStamp(boat* boats)
  {
     int i  = 0;
     int j  = 0;
     for(i=0;i <= list_endIndex;i++)
     {
        if(boats[i].user_id && (boats[i].time_cnt>=0))
        { 
           boats[i].time_cnt--;
           ///boat_list_p 数组指向所有有效船舶的地址
           boat_list_p[j++]  = &boats[i];
        }
        else if(boats[i].time_cnt  == -1 )
        {
           boats[i].user_id  = 0;
           boats[i].longitude  = 0;
           boats[i].latitude  = 0;
           boats[i].true_heading  = 0;
           boats[i].type_of_electronic_position_fixing_device  = 0;
           boats[i].name[0] = '\0';
           boats[i].time_cnt  = 0;
//printf("\r\ndelete one at [%3d]\r\n",i);           
        }

     }
     
     while(boats[list_endIndex].user_id == 0)
        list_endIndex--;
     ///记录有效船舶的数量
     N_boat  = j;
//     myftoa(pStrBuf, 0.12);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 0.123);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 0.1234);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 1.12);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 1.123);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 1.1234);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 123.0);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 123.1234);
//     printf("\r\nstr:%s",pStrBuf);
//     myftoa(pStrBuf, 1234.012);
//     printf("\r\nstr:%s",pStrBuf);
//     getSphereDist(1928457,7127540,1927264,7128662);
//    getSphereDist(1928457, 7127540, 1927264, 7128662);
   
  }



static double getSphereDist(long lt_1,long lg_1, long lt_2, long lg_2)
{
	float dist = 0.0;
	float f_1 = 1.0*lt_1 / 60000;
	float f_2 = 1.0*lt_2 / 60000;
	float l_1 = 1.0*lg_1 / 60000;
	float l_2 = 1.0*lg_2 / 60000;
	float diff = 1.0*(lg_1 - lg_2) / 60000;
 float tmp  = 0.0;
 long aa = 0;
// f_1  = lt_1/60000 + ( (lt_1%60000)/10000 )*0.01667;

// printf("\r\nf_1:%lf\r\n",f_1);

// f_2  = 2.12345;
// printf("\r\nf_2:%lf\r\n",f_2);
// 
// tmp  = cos((f_1-f_2)*PI/180);
// printf("\r\ntmp:%f",tmp);
// 
// 

	dist = 6371 / 1.852 * acos(cos((f_1 - f_2)*PI / 180) / 2
		- cos((f_1 + f_2)*PI / 180) / 2
		+ cos((diff + f_1 + f_2)*PI / 180) / 4
		+ cos((diff + f_1 - f_2)*PI / 180) / 4
		+ cos((diff - f_1 + f_2)*PI / 180) / 4
		+ cos((diff - f_1 - f_2)*PI / 180) / 4
		);
 return dist;

//  dist = R_NM * acos(       cos((f_1-f_2)*PI/180)/2
//                           -cos((f_1+f_2)*PI/180)/2
//                           +cos((diff+f_1+f_2)*PI/180)/4
//                           +cos((diff+f_1-f_2)*PI/180)/4
//                           +cos((diff-f_1+f_2)*PI/180)/4
//                           +cos((diff-f_1-f_2)*PI/180)/4
//                            ); 
//  printf("\r\nSphere dist:%lf\r\n",dist); 
//  
//  dist = R_NM * acos(  cos(2.9E-7 * (lt_1-lt_2))/2
//                           -cos((f_1+f_2)*PI/180)/2
//                           +cos((diff+f_1+f_2)*PI/180)/4
//                           +cos((diff+f_1-f_2)*PI/180)/4
//                           +cos((diff-f_1+f_2)*PI/180)/4
//                           +cos((diff-f_1-f_2)*PI/180)/4
//                            ); 
//  printf("\r\nSphere dist:%lf\r\n",dist);
//  dist = R_NM * acos(  cos(LLTOA *(      lt_1 - lt_2) )/2
//                      -cos(LLTOA *(      lt_1 + lt_2) )/2
//                      +cos(LLTOA *(tmp + lt_1 + lt_2) )/4
//                      +cos(LLTOA *(tmp + lt_1 - lt_2) )/4
//                      +cos(LLTOA *(tmp - lt_1 + lt_2) )/4
//                      +cos(LLTOA *(tmp - lt_1 - lt_2) )/4
//                            ); 
//  printf("\r\nSphere dist:%lf\r\n",dist);   


   
}



/*
float getSphereDist(long lt_1,long lg_1, long lt_2, long lg_2)
{
  float dist = 0.0;
  float f_1  = 1.0*lt_1/60000;
  float f_2  = 1.0*lt_2/60000;
  float l_1  = 1.0*lg_1/60000;
  float l_2  = 1.0*lg_2/60000;
//  long  tmp  = lg_1 - lg_2;
  float diff = 1.0*(lg_1-lg_2)/60000;
  
//  dist = 6371/1.852* acos(  cos((f_1-f_2)*PI/180)/2
//                           -cos((f_1+f_2)*PI/180)/2
//                           +cos((diff+f_1+f_2)*PI/180)/4
//                           +cos((diff+f_1-f_2)*PI/180)/4
//                           +cos((diff-f_1+f_2)*PI/180)/4
//                           +cos((diff-f_1-f_2)*PI/180)/4
//                            );
//  printf("\r\nSphere dist:%lf\r\n",dist); 

  dist = R_NM * acos(       cos((f_1-f_2)*PI/180)/2
                           -cos((f_1+f_2)*PI/180)/2
                           +cos((diff+f_1+f_2)*PI/180)/4
                           +cos((diff+f_1-f_2)*PI/180)/4
                           +cos((diff-f_1+f_2)*PI/180)/4
                           +cos((diff-f_1-f_2)*PI/180)/4
                            ); 
//  printf("\r\nSphere dist:%lf\r\n",dist); 
  
  return dist;
//  dist = R_NM * acos(  cos(2.9E-7 * (lt_1-lt_2))/2
//                           -cos((f_1+f_2)*PI/180)/2
//                           +cos((diff+f_1+f_2)*PI/180)/4
//                           +cos((diff+f_1-f_2)*PI/180)/4
//                           +cos((diff-f_1+f_2)*PI/180)/4
//                           +cos((diff-f_1-f_2)*PI/180)/4
//                            ); 
//  printf("\r\nSphere dist:%lf\r\n",dist);
//  dist = R_NM * acos(  cos(LLTOA *(      lt_1 - lt_2) )/2
//                      -cos(LLTOA *(      lt_1 + lt_2) )/2
//                      +cos(LLTOA *(tmp + lt_1 + lt_2) )/4
//                      +cos(LLTOA *(tmp + lt_1 - lt_2) )/4
//                      +cos(LLTOA *(tmp - lt_1 + lt_2) )/4
//                      +cos(LLTOA *(tmp - lt_1 - lt_2) )/4
//                            ); 
//  printf("\r\nSphere dist:%lf\r\n",dist);   
   
}
*/


void myftoa(unsigned char * str, float num)
{
    int i  = 0;
    int j  = 0;
    int subVal  = 0;
    unsigned tmp  = 0;
    
    
    subVal  = (num*1000)/1;
//printf("\r\nsubVal:%d",subVal);  
    for(i=0;i<3;i++)
    {
       str[i]  = '0'+subVal%10;
       subVal  = subVal/10;
    }
    str[i++]  = '.';
    subVal  = num/1;

    str[i++]  = subVal%10 + '0';
    
    subVal  = subVal/10;
    while(subVal)
    {
       str[i++]  = subVal%10 + '0';
       subVal  = subVal/10;
    }
    str[i]  = '\0';
    i--;
    for (j = 0; j<(i + 1) / 2; j++)
    {
     str[j] =      str[j] ^ str[i - j];
     str[i - j] =  str[j] ^ str[i - j];
     str[j] =      str[j] ^ str[i - j];

    }
    
    str[5]  = '\0';
//printf("\r\nstr:%s",str);  
}



