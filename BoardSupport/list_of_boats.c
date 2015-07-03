#include "boat_struct.h"
#include "analysis_function.h"
#include "uart.h"
#include "Config.h"
#include "string.h"
// find if id exist in aux_boat, yes return 1, otherwise 0, size exclusive
#define alloc_boat_mem (_boat**)malloc(sizeof(_boat*)*5000);

extern int list_endIndex;

extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];

//extern short N_boat;
//  struct _BOAT_NODE
//  {
//     _boat boat;
//     int   timeCnt;
//     struct _BOAT_NODE* pNext;
//  };
//  typedef struct _BOAT_NODE BOAT_NODE;



	void insert(boat * boats,  struct message_18* p_msg )
  {
     int i  = 0;
     
     if(p_msg->user_id == 0)
       return;

     for(i=0;i<=list_endIndex;i++)
     {

        if(boats[i].user_id  == p_msg->user_id)
        {
           boats[i].SOG          = p_msg->SOG;
           boats[i].COG          = p_msg->COG;
           boats[i].true_heading = p_msg->true_heading;
           boats[i].longitude    = p_msg->longitude/10;
           boats[i].latitude     = p_msg->latitude/10;
           boats[i].time_cnt     = TIMESTAMP;
           
           return ;
        }
     }
     
     for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
     {
     
//MYDEBUG ("%d-user_id:%ld",i,boats[i].user_id);
        if(boats[i].user_id == 0)
        {
           boats[i].user_id      = p_msg->user_id;
           boats[i].SOG          = p_msg->SOG;
           boats[i].COG          = p_msg->COG;
           boats[i].true_heading = p_msg->true_heading;
           boats[i].longitude    = p_msg->longitude/10;
           boats[i].latitude     = p_msg->latitude/10;
           boats[i].time_cnt     = TIMESTAMP;
           
           if(i > list_endIndex) list_endIndex  = i;
           return ;
        }
     }
     
     MYDEBUG("Error:boat list full.");
  }
  
  
  void insert_24A(boat* boats, struct message_24_partA* p_msg)
  {
     int i  = 0;
     int tmp  = 0;
   
     if(p_msg->user_id == 0)
       return ;
     
     for(i=0;i<=list_endIndex;i++)
     {
        if(boats[i].user_id == p_msg->user_id)
        {
           memcpy(boats[i].name, p_msg->name, 20);
           boats[i].time_cnt  = TIMESTAMP;
           return;
        }
     }
     
     for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
     {
   
        if(boats[i].user_id == 0)
        {
           boats[i].user_id  = p_msg->user_id;

           memcpy(boats[i].name, p_msg->name, 20);
//           for(tmp=19;tmp>=0;tmp--)
//           {
//              boats[i].name[tmp] = p_msg->name[tmp];
//           }
           if(i > list_endIndex) list_endIndex  = i;
           return;
        }
     }
     
     MYDEBUG("Error:boat list full.");
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
     
     MYDEBUG("Error:boat list full.");
  }
  
  
  void updateTimeStamp(boat* boats)
  {
     int i  = 0;
     int j  = 0;
     for(i=0;i <= list_endIndex;i++)
     {
        if(boats[i].user_id && boats[i].time_cnt)
        {
           boats[i].time_cnt--;
           boat_list_p[j++]  = &boats[i];
        }
        else
        {
           boats[i].user_id  = 0;
        }
     }
     
     N_boat  = j;
MYDEBUG("number of boat = %d",N_boat)    ; 
  }



