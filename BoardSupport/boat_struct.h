#ifndef  BOAT_STRUCT_H
#define  BOAT_STRUCT_H

#include <stdlib.h>
#include "GUI.h"
#include "map_draw.h"
#include "Setting.h"

#define choose_4 4
#define choose_3 40
#define choose_2 500
#define choose_1 1888

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
	int type_of_electronic_position_fixing_device;
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
 static const GUI_POINT aPoints[] = {
	{ 6, 12},
	{ -6, 12},
	{ 0, -12}
};

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
//-Add end	
	
void disp_fish_boat(const scale_map * scale,long center_longitude, long center_latitude,SIMP_BERTH* pSimpBerth,short N);


extern _boat test[3];
extern _boat *test_p[500];
extern short N_boat;//extern short N_boat;

#endif
