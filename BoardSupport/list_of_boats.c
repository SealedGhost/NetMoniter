#include "boat_struct.h"
#include "analysis_function.h"
#include "uart.h"
#include "Config.h"
#include "string.h"
// find if id exist in aux_boat, yes return 1, otherwise 0, size exclusive
#define alloc_boat_mem (_boat**)malloc(sizeof(_boat*)*5000);

extern int list_endIndex;

extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];
extern boat mothership;



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
     if( (p_msg->longitude/10 < mothership.longitude-30000)  ||  (p_msg->longitude/10 > mothership.longitude+30000) ) 
        return;
     if( (p_msg->latitude/10 < mothership.latitude-30000)  ||  (p_msg->latitude/10 > mothership.latitude+30000) )
        return;
        
     ///若此船已存在，刷新各项参数
     for(i=0;i<=list_endIndex;i++)
     {

        if(boats[i].user_id  == p_msg->user_id)
        {
           boats[i].SOG          = p_msg->SOG;
           boats[i].COG          = p_msg->COG;
           boats[i].true_heading = p_msg->true_heading;
//           boats[i].longitude    = p_msg->longitude/10;
//           boats[i].latitude     = p_msg->latitude/10;
           tmp  = p_msg->longitude/10;
//           if( (tmp >= mothership.longitude-30000)  &&  (tmp <= mothership.latitude+30000) )  /// WARNING: ，若本行打开，经度一直为 0 还不知道原因
              boats[i].longitude  = tmp;
           tmp  = p_msg->latitude/10;
//           if( (tmp >= mothership.latitude-30000)  &&  (tmp <= mothership.latitude+30000) )   ///本行打开 纬度也正常
              boats[i].latitude  = tmp;
           boats[i].time_cnt     = TIMESTAMP; ///刷新时间戳变量
INFO("boats[%d].(lg,lt) = %ld,%ld",i,boats[i].longitude,boats[i].latitude);
           return ;
        }
     }
     
     ///若此船是新的，找到数组中的空位，插入此船信息
     for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
     {
     
//INFO ("%d-user_id:%ld",i,boats[i].user_id);
        if(boats[i].user_id == 0)
        {
           boats[i].user_id      = p_msg->user_id;
           boats[i].SOG          = p_msg->SOG;
           boats[i].COG          = p_msg->COG;
           boats[i].true_heading = p_msg->true_heading;
//           boats[i].longitude    = p_msg->longitude/10;
//           boats[i].latitude     = p_msg->latitude/10;
           tmp  = p_msg->longitude/10;
INFO("tmp:%ld",tmp);           
           if( (tmp >= mothership.longitude-30000)  &&  (tmp <= mothership.latitude+30000) )
           {
              boats[i].longitude  = tmp;
INFO("boats[%d].lg = %ld",i, boats[i].longitude);              
           }           

           tmp  = p_msg->latitude/10;
INFO("tmp:%ld",tmp);           
           if( (tmp >= mothership.latitude-30000)  &&  (tmp <= mothership.latitude+30000) )
           {
              boats[i].latitude  = tmp;
INFO("boats[%d].lt = %ld",i, boats[i].latitude);              
           }    
           boats[i].time_cnt     = TIMESTAMP;
           
           ///若插入位置已超出尾端索引，更新尾端索引                           /// |___________________|_______| <--boats[]
           if(i > list_endIndex) list_endIndex  = i;
INFO("boats[%d].(lg,lt) = %ld,%ld",i,boats[i].longitude,boats[i].latitude);    ///                     ^       
           return ;                                                            ///                     |
        }                                                                      ///                   list_endIndex
     }
     
#if INFO_ENABLE
     INFO("Error:boat list full.");
#endif
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
           boats[i].user_id  = p_msg->user_id;
           
           for(tmp=0;tmp<20;tmp++)
              {
                 boats[i].name[tmp]  = p_msg->name[tmp];
                 if(p_msg->name[tmp] == '\0')
                 {                  
                    break;
                 }
                    
              }               

           if(i > list_endIndex) list_endIndex  = i;
           return;
        }
     }
     
#if INFO_ENABLE
     INFO("Error:boat list full.");
#endif
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
           boats[i].type_of_electronic_position_fixing_device  = p_msg->type_of_ship_and_cargo_type;
           boats[i].time_cnt  = TIMESTAMP;
           return;
        } 
     }
      
     for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
     {
     
        if(boats[i].user_id == 0)
        {
           boats[i].type_of_electronic_position_fixing_device  = p_msg->type_of_ship_and_cargo_type;
           
           if(i > list_endIndex) list_endIndex  = i;
           return;
        }
     }
#if INFO_ENABLE
     INFO("Error:boat list full.");
#endif     
  }
  
 ///每隔若干秒后调用本函数，遍历boats数组对所有船舶的time_cnt变量减一，若有船的time_cnt减为 0 ，
 ///则删去此船（将存放此船的boats[i].user_id设置为0，即可） 
  void updateTimeStamp(boat* boats)
  {
     int i  = 0;
     int j  = 0;
     for(i=0;i <= list_endIndex;i++)
     {
        if(boats[i].user_id && (boats[i].time_cnt>0))
        { 
           boats[i].time_cnt--;
           ///boat_list_p 数组指向所有有效船舶的地址
           boat_list_p[j++]  = &boats[i];
        }
        else
        {
           boats[i].user_id  = 0;
        }
     }
     
     ///记录有效船舶的数量
     N_boat  = j;
     
     for(i=0;i<N_boat;i++)
     {
INFO("boat_list_p[%d].(lg,lt)= %ld,%ld",i,boat_list_p[i]->longitude,boat_list_p[i]->latitude);       
     }
  }



