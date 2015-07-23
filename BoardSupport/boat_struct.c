#include "boat_struct.h"
#include "map_draw.h"
#include "Config.h"

boat mothership;
short isAllBoatVisible = 1;

//void draw_ship(boat* p_ship)
//{
//	 GUI_POINT aEnlargedPoints[GUI_COUNTOF(aPoints)];
//	 GUI_RotatePolygon(aEnlargedPoints, aPoints, GUI_COUNTOF(aPoints), p_ship->true_heading*3.14/180);
//	 GUI_DrawPolygon(aEnlargedPoints, GUI_COUNTOF(aPoints), p_ship->x, p_ship->y);
//}



//void draw_ship(_boat *boat_ay,float x,float y,short true_heading,short center_x,short center_y){
void draw_ship(SIMP_BERTH* pSimBerth,short basePoint_x,short basePoint_y){
	//short boat_coordinate_x = center_x + x;
	//short boat_coordinate_y = center_y - y;
	GUI_POINT aEnlargedPoints[GUI_COUNTOF(aPoints)];

	basePoint_x = (MAP_LEFT/2 + MAP_RIGHT/2) + basePoint_x;
	basePoint_y = (MAP_TOP/2 + MAP_BOTTOM/2) - basePoint_y;
//	GUI_RotatePolygon(aEnlargedPoints,aPoints,GUI_COUNTOF(aPoints),pBoat->true_heading*3.14/180);
 GUI_RotatePolygon(aEnlargedPoints, aPoints, GUI_COUNTOF(aPoints), pSimBerth->pBoat->true_heading*3.14/180);
	if((basePoint_x >=MAP_LEFT)&&(basePoint_x<= MAP_RIGHT)&&(basePoint_y >=MAP_TOP)&&(basePoint_y <= MAP_BOTTOM))
	 GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(aPoints),basePoint_x,basePoint_y);
}

void draw_mothership(short basePoint_x, short basePoint_y)
{
	 	GUI_POINT aEnlargedPoints[GUI_COUNTOF(bPoints)];
  	
	  basePoint_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + basePoint_x;
	  basePoint_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - basePoint_y;
	
  	GUI_RotatePolygon(aEnlargedPoints,bPoints,GUI_COUNTOF(bPoints),mothership.true_heading*3.14/180);
	  
	  if(     (basePoint_x>= MAP_LEFT)  &&  (basePoint_x <= MAP_RIGHT) 
						&&  (basePoint_y >= MAP_TOP)  &&  (basePoint_y <= MAP_BOTTOM)  )
		{
			  GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(bPoints),basePoint_x, basePoint_y);
		}
}



void disp_fish_boat(const scale_map * scale,long center_longitude, long center_latitude,SIMP_BERTH* pSimpBerth,short N){
	
	short i=0;
 short base_x = 0;
 short base_y = 0;
	for(i=0;i<N;i++)
	{
//		isAllBoatVisible  = 1;
//		tmp_x  = 1.0*scale->pixel * (boat_array[i]->longitude-center_longitude) / scale->minute;
//		tmp_y  = 1.0*scale->pixel * (boat_array[i]->latitude-center_latitude) / scale->minute;
//		if(isAllBoatVisible || boat_array[i]->isVisible)
//		{
//				base_x =1.0*scale->pixel * (boat_array[i]->longitude - center_longitude) / scale->minute;
    base_x  = 1.0*scale->pixel * (pSimpBerth[i].longitude-center_longitude) / scale->minute;
			 base_y =1.0*scale->pixel * (pSimpBerth[i].latitude-center_latitude) / scale->minute;
				draw_ship(&(pSimpBerth[i]),base_x,base_y);	
//		}
  
	}

	
	base_x  = 1.0*scale->pixel * (mothership.longitude - center_longitude) / scale->minute;
	base_y  = 1.0*scale->pixel * (mothership.latitude - center_latitude) / scale->minute;
	draw_mothership(base_x,base_y);
}
