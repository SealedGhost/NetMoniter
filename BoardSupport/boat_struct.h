#ifndef  BOAT_STRUCT_H
#define  BOAT_STRUCT_H

#include <stdlib.h>
#include "GUI.h"
#include "map_draw.h"

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
	short isVisible;
	long user_id;
  long longitude;
  long latitude;
  float x;
  float y;
	
	char name[20];
  short time_cnt; 
	int type_of_electronic_position_fixing_device;
};

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
	
void disp_fish_boat(const scale_map * scale,long center_longitude, long center_latitude,short center_x,short center_y,_boat **boat_array,short N);


extern _boat test[3];
extern _boat *test_p[500];
extern short N_boat;//extern short N_boat;

#endif
