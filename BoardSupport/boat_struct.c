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
void draw_ship(_boat *boat_ay,short true_heading,short center_x,short center_y){
	//short boat_coordinate_x = center_x + x;
	//short boat_coordinate_y = center_y - y;
	GUI_POINT aEnlargedPoints[GUI_COUNTOF(aPoints)];

	boat_ay->x = center_x + boat_ay->x;
	boat_ay->y = center_y - boat_ay->y;
	GUI_RotatePolygon(aEnlargedPoints,aPoints,GUI_COUNTOF(aPoints),true_heading*3.14/180);
	if((boat_ay->x >=MAP_LEFT)&&(boat_ay->x<= MAP_RIGHT)&&(boat_ay->y >=MAP_TOP)&&(boat_ay->y <= MAP_BOTTOM))
	 GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(aPoints),boat_ay->x, boat_ay->y);
}

void draw_mothership(short mothership_x,short mothership_y,short true_heading, short center_x, short center_y)
{
	 	GUI_POINT aEnlargedPoints[GUI_COUNTOF(bPoints)];
  	
	  mothership.x  = center_x + mothership_x;
	  mothership.y  = center_y - mothership_y;
	
  	GUI_RotatePolygon(aEnlargedPoints,bPoints,GUI_COUNTOF(bPoints),true_heading*3.14/180);
	  
	  if(     (mothership.x>= MAP_LEFT)  &&  (mothership.x <= MAP_RIGHT) 
						&&  (mothership.y >= MAP_TOP)  &&  (mothership.y <= MAP_BOTTOM)  )
		{
			  GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(bPoints),mothership.x, mothership.y);
		}
}

//-Add by XiaoLu at 2015/5/7
/*************************************

	Function:       draw_mothership()
	Description:    draw the mothership
	Calls:          
	Called By:      
	Table Accessed: NULL
	Table Updated:  NULL
	Input:          center x coordinate ,
                 center y coordinate ,
                 heading angle
	Output:         void
	Return:         void
	Others:         

***************************************/
//void draw_mothership(const long mothership_longitude, const long mothership_latitude,short mothership_x,short mothership_y,const short true_heading)
//{
// 
//	/* 设置旋转角度 */
//		GUI_POINT aEnlargedPoints[GUI_COUNTOF(bPoints)];
//		GUI_RotatePolygon(aEnlargedPoints,bPoints,GUI_COUNTOF(bPoints),true_heading*3.14/180);
//		
//	/* 若母船在屏幕显示范围内。则绘制母船 */
//		if(     (mothership_x >= 0)  &&  (mothership_x <= map_width) 
//						&&  (mothership_y >= 0)  &&  (mothership_y <= map_height)  )
//		{
//			  GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(bPoints),mothership_x, mothership_y);
//		}
//}

//void draw_mothership(boat * p_mothership)
//{
//	/* 设置旋转角度 */
//		GUI_POINT aEnlargedPoints[GUI_COUNTOF(bPoints)];
//		GUI_RotatePolygon(aEnlargedPoints,bPoints,GUI_COUNTOF(bPoints),p_mothership->true_heading*3.14/180);	
//		GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(bPoints),p_mothership->x, p_mothership->y);
//	
//}


//-Add end


void disp_fish_boat(char scale_choose,long center_longitude, long center_latitude,short center_x,short center_y,_boat **boat_array,short N){
	
	short i=0;

	MYDEBUG("N:%d",N);
	for(i=0;i<N;i++)
	{

//		isAllBoatVisible  = 1;
//		tmp_x  = 1.0*measuring_scale[scale_choose].pixel * (boat_array[i]->longitude-center_longitude) / measuring_scale[scale_choose].minute;
//		tmp_y  = 1.0*measuring_scale[scale_choose].pixel * (boat_array[i]->latitude-center_latitude) / measuring_scale[scale_choose].minute;
//		if(isAllBoatVisible || boat_array[i]->isVisible)
//		{
				boat_array[i]->x =1.0*measuring_scale[scale_choose].pixel * (boat_array[i]->longitude - center_longitude) / measuring_scale[scale_choose].minute;

				boat_array[i]->y =1.0*measuring_scale[scale_choose].pixel * (boat_array[i]->latitude - center_latitude) / measuring_scale[scale_choose].minute;

				draw_ship(boat_array[i],boat_array[i]->true_heading,center_x,center_y);
	
//		}
  
	}
//-Add by Xiaolu at 2015/5/7	
	
		 /* 将母船与center点的经纬度距离转化为pixel距离 */
//	 tmp_x = 1.0*measuring_scale[scale_choose].pixel * (mothership.longitude-center_longitude) / measuring_scale[scale_choose].minute;
//	 tmp_y = 1.0*measuring_scale[scale_choose].pixel * (mothership.latitude-center_latitude) / measuring_scale[scale_choose].minute; 
//	
//	 /* 得到母船的pixel坐标 */
//  mothership.x  = tmp_x + center_x;
//	 mothership.y  = tmp_y - center_y;
//	
//	 draw_mothership(mothership.longitude,mothership.latitude,mothership.x,mothership.y,mothership.true_heading);
//-Add end	
	
	mothership.x  = 1.0*measuring_scale[scale_choose].pixel * (mothership.longitude - center_longitude) / measuring_scale[scale_choose].minute;
	mothership.y  = 1.0*measuring_scale[scale_choose].pixel * (mothership.latitude - center_latitude) / measuring_scale[scale_choose].minute;
	draw_mothership(mothership.x, mothership.y, mothership.true_heading, center_x, center_y);
}
