#ifndef  BOAT_STRUCT_H
#define  BOAT_STRUCT_H

#include <stdlib.h>
#include "GUI.h"
#include "map.h"
#include "lpc_types.h"
#include "sysConf.h"


typedef enum _MNT_States MNT_States;
enum _MNT_States  {MNTState_None=0,MNTState_Init, MNTState_Delete, MNTState_Choosen,MNTState_Default,MNTState_Monited,MNTState_Triggered,MNTState_Masked};


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
	 long target;
};


typedef struct _BERTH BERTH;
struct _BERTH
{
   boat Boat;
   unsigned char mntState;
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


static  GUI_POINT  Points_boat[3]  = {
                                           { 6, 12},
                                           {-6, 12},
                                           { 0,-12}
                                                         };
                                           

/*
   (-20,190)  (20,190)
         /\          /\
        /  \        /  \
       /    \      /    \ 
      /      \____/      \
     /(-10,160) (10,160)  \
    /                      \
   /                        \
  /                          \
 /                            \
/ (-100,30)           (100,30) \
\                              /
 \                            /
  \                          /
   \                        /
    \                      /
     \                    /
      \                  /
       \                / 
        \(-20,-110)    /(20,-110)
        /              \
       /                \
      /     (0,-150)     \
     /         ^          \
    /       .     .        \
   /     .           .      \
  /   .                 .    \
 / .                       .  \
/ (-80,-190)          (80,-190)\
  
*/                                               
static  GUI_POINT  Points_fish[11]  = { {  0,-15},
                                        { -8,-19},
                                        { -2,-11},
                                        { -10, 3},
                                        { -2, 19},
                                        { -1, 16},
                                        {  1, 16},
                                        {  2, 19},
                                        { 10,  3},
                                        {  2,-11},
                                        {  8,-19}
                                       };                                     
                                           
//static const GUI_POINT  Polygons[2][8] ={ { {0,12}, {-12,-6}, {-3,-6}, {-6,-12}, {12,-12}, {0,-6}, {12,-6} },
//                                          { {6,12}, {-6, 12}, {0, -12}, {0,  0}, {0,   0}, {0, 0}, {0,  0} }  
//                                        };
//-Add by XiaoLu at 2015/5/7
 /* The shape of mothership */
                                                  //                *
                                                  //              *   *
static const GUI_POINT bPoints[] = {             //            *       *
		                                      {0,-18},  //          *           *
																																								{9,-3},   //           *         *
																																								{6,18},   //            *       *
																																								{-6,18},  //             *     *
																																								{-9,-3}   //              *****        
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
