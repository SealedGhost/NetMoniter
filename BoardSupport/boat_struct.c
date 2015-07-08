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
void draw_ship(_boat *pBoat,short center_x,short center_y){
	//short boat_coordinate_x = center_x + x;
	//short boat_coordinate_y = center_y - y;
	GUI_POINT aEnlargedPoints[GUI_COUNTOF(aPoints)];

	pBoat->x = center_x + pBoat->x;
	pBoat->y = center_y - pBoat->y;
	GUI_RotatePolygon(aEnlargedPoints,aPoints,GUI_COUNTOF(aPoints),pBoat->true_heading*3.14/180);
	if((pBoat->x >=MAP_LEFT)&&(pBoat->x<= MAP_RIGHT)&&(pBoat->y >=MAP_TOP)&&(pBoat->y <= MAP_BOTTOM))
	 GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(aPoints),pBoat->x,pBoat->y);
}

void draw_mothership(short mothership_x,short mothership_y, short center_x, short center_y)
{
	 	GUI_POINT aEnlargedPoints[GUI_COUNTOF(bPoints)];
  	
	  mothership.x  = center_x + mothership_x;
	  mothership.y  = center_y - mothership_y;
	
  	GUI_RotatePolygon(aEnlargedPoints,bPoints,GUI_COUNTOF(bPoints),mothership.true_heading*3.14/180);
	  
	  if(     (mothership.x>= MAP_LEFT)  &&  (mothership.x <= MAP_RIGHT) 
						&&  (mothership.y >= MAP_TOP)  &&  (mothership.y <= MAP_BOTTOM)  )
		{
			  GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(bPoints),mothership.x, mothership.y);
		}
}



void disp_fish_boat(const scale_map * scale,long center_longitude, long center_latitude,short center_x,short center_y,_boat **boat_array,short N){
	
	short i=0;
	for(i=0;i<N;i++)
	{
//		isAllBoatVisible  = 1;
//		tmp_x  = 1.0*scale->pixel * (boat_array[i]->longitude-center_longitude) / scale->minute;
//		tmp_y  = 1.0*scale->pixel * (boat_array[i]->latitude-center_latitude) / scale->minute;
//		if(isAllBoatVisible || boat_array[i]->isVisible)
//		{
				boat_array[i]->x =1.0*scale->pixel * (boat_array[i]->longitude - center_longitude) / scale->minute;
				boat_array[i]->y =1.0*scale->pixel * (boat_array[i]->latitude - center_latitude) / scale->minute;
				draw_ship(boat_array[i],center_x,center_y);	
//		}
  
	}

	
	mothership.x  = 1.0*scale->pixel * (mothership.longitude - center_longitude) / scale->minute;
	mothership.y  = 1.0*scale->pixel * (mothership.latitude - center_latitude) / scale->minute;
	draw_mothership(mothership.x, mothership.y, center_x, center_y);
}
