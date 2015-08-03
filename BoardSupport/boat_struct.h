#ifndef  BOAT_STRUCT_H
#define  BOAT_STRUCT_H

#include <stdlib.h>
#include "GUI.h"
#include "map_draw.h"
#include "lpc_types.h"
#include "SystemConfig.h"


#define choose_4 4
#define choose_3 40
#define choose_2 500
#define choose_1 1888

typedef enum _MNT_States MNT_States;
enum _MNT_States  {MNTState_None=0, MNTState_Choosen,MNTState_Default,MNTState_Monited};


typedef struct _boat _boat;
//typedef struct _ll_choose _ll_choose;
typedef struct _boat_m24A _boat_m24A;
typedef struct _boat_m24B _boat_m24B;

typedef _boat boat;

struct _boat
{
  //struct _boat *next_boat;
	
  short SOG;
  short COG;
  short true_heading;
  short time_cnt; 

	 long user_id;
  long longitude;
  long latitude;
	 int dist;
 	char name[20];
  MNT_States mntStates;
	 int isInvader;
};


typedef struct _BERTH BERTH;
struct _BERTH
{
   boat Boat;
   BERTH * pLast;
   BERTH * pNext;
};

typedef struct  _SIMP_BERTH SIMP_BERTH;
struct _SIMP_BERTH
{
   long longitude;
   long latitude;
   int  Dist;
   boat * pBoat;
};


//typedef struct _BERTH BERTH;
//struct _BERTH
//{
//   boat boat;
//   BERTH * pLast;
//   BERTH * pNext;
//};


struct _boat_m24A{
	long user_id;
	char name[20];
};

struct _boat_m24B{
	long user_id;
	int type_of_ship_and_cargo_type;
};
// struct _ll_choose{
// 	long west_longitude;
// 	long east_longitude;
// 	long north_latitude;
// 	long south_latitude;

// 	long center_longitude;//raw not real
// 	long center_latitude;
// };
// static const GUI_POINT aPoints[] = {
//	{ 6, 12},
//	{ -6, 12},
//	{ 0, -12}
//};

static  GUI_POINT  Points_boat[3]  = {
                                           { 6, 12},
                                           {-6, 12},
                                           { 0,-12}
                                                         };
                                           
//static  GUI_POINT  Points_fish[7]  = {
//                                           {  0,  12},
//                                           {-12,  -6},
//                                           { -3,  -6},
//                                           { -6, -12},
//                                           { 12, -12},
//                                           {  0,  -6},
//                                           { 12,  -6},
// 
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
	
void disp_fish_boat(const scale_map * scale,long center_longitude, long center_latitude,SIMP_BERTH* pSimpBerth,short N);
//void MNT_dispBoat(const scale_map * scale,  long center_lg, long center_lt, MNT_BERTH * pIterator);
//void MNT_dispInfo( short base_x, short base_y, boat * pBoat);
void changeShape(BOAT_SHAPES shape);


#endif
