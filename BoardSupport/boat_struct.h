#ifndef  BOAT_STRUCT_H
#define  BOAT_STRUCT_H

#include <stdlib.h>
#include "GUI.h"
#include "map.h"
#include "lpc_types.h"
#include "sysConf.h"


typedef enum _MNT_States MNT_States;
enum _MNT_States  {  MNTState_None=0,
                     MNTState_Cancel,   ///1
                     MNTState_Init,     ///2
                     MNTState_Pending,  ///3
                     MNTState_Choosen,  ///4
                     MNTState_Default,  ///5
                     MNTState_Monitored,///6
                     MNTState_Triggered,///7
                     MNTState_Masked    ///8No
                  };  


typedef struct _boat _boat;
typedef struct _boat_m24A _boat_m24A;
typedef struct _boat_m24B _boat_m24B;

typedef _boat boat;

struct _boat
{
	 long user_id;
  long longitude;
  long latitude;
  char name[20];
  short SOG;
  short COG;
  short true_heading;
  short time_cnt; 

	 int dist;
  char isHSD;
};


typedef struct _BERTH BERTH;
struct _BERTH
{
   boat Boat;
   unsigned char mntState;
   long isInvader;
   BERTH * pLast;
   BERTH * pNext;
};

typedef struct  _SIMP_BERTH SIMP_BERTH;
struct _SIMP_BERTH
{
   long longitude;
   long latitude;
   int  Dist;
   BERTH * pBerth;
};



struct _boat_m24A{
	long user_id;
	char name[20];
};

struct _boat_m24B{
	long user_id;
	int vender;
};


                                    




static GUI_POINT Points_Compass_1[]  = { { 0,  0},
                                         {-6,  0},
                                         { 0, -24}
                                      };
static GUI_POINT Points_Compass_2[]  = { { 0,  0},
                                         { 6,  0},
                                         { 0, -24}
                                        };
static GUI_POINT Points_Compass_3[]  = { { 0,  0},
                                         { 6,  0},
                                         { 0, 24}
                                        };                                        
static GUI_POINT Points_Compass_4[]  = { { 0,  0},
                                         {-6,  0},
                                         { 0, 24}
                                       };                                       

#endif
