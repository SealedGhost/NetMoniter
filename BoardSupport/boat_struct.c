#include "boat_struct.h"
#include "map_draw.h"
#include "Config.h"
#include "Setting.h"
#include "Drawinformation.h"
/*--------------------------------- Macro defines -------------------------------------*/
#define MYABS(x)   ((x)>0?(x):(-(x)))




/*--------------------------------- external variables ---------------------------------*/
extern int N_boat;
extern char * pStrBuf;
extern SIMP_BERTH SimpBerthes[BOAT_LIST_SIZE_MAX];
extern CONF_SYS SysConf;

extern MapWin_COLOR mapSkins[2];
extern MapWin_COLOR * pMapSkin;

extern _cursor __cursor;


GUI_POINT * pPoints  = Points_fish;
short       PointNum  = 11; 

boat mothership;
short isAllBoatVisible = 1;




void changeShape(BOAT_SHAPES shape)
{
   if(shape == SHAPE_Boat)
   {
      pPoints  = Points_boat;
      PointNum  = 3;
   }
   else 
   {
      pPoints  = Points_fish;
      PointNum  = 11;
   }
}

void fixPos(short * base_x, short * base_y)
{
   if(*base_x + 10  > MAP_RIGHT-180)
   {
      *base_x  = *base_x-10-180;
   }
   else
   {
      *base_x  = *base_x+10;
   }
   
   if(*base_y+10 > MAP_BOTTOM-80)
   {
      *base_y  = *base_y-10-80;
   }
   else
   {
      *base_y  = *base_y+10;
   }
}


void MNT_dispInfo( short base_x, short base_y, boat * pBoat)
{
   if(pBoat == NULL)
      return ;
//   GUI_ALPHA_STATE AlphaState;
    
//   GUI_SetDrawMode(GUI_DRAWMODE_XOR);
   fixPos(&base_x, &base_y);
//   GUI_SetUserAlpha(&AlphaState, 50);
//   GUI_SetAlpha(0x40);
   GUI_SetColor(pMapSkin->BackGround);

   GUI_ClearRect(base_x, base_y, base_x+180,base_y+80);
//   
//   GUI_SetColor(pMapSkin->Boat_Tip_Border);
//   GUI_DrawRect(base_x, base_y, base_x+180,base_y+80);
   
//   GUI_SetAlpha(0);
   
   GUI_SetFont(&GUI_Font16_1);
   GUI_SetColor(pMapSkin->Boat_Name);
   GUI_DispStringAt(pBoat->name, base_x+10, base_y+1); 
   
   GUI_SetColor(pMapSkin->Boat_Cdnt);
   GUI_SetFont(&GUI_Font24_1);
   
   lltostr(pBoat->latitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, base_x+10, base_y+16);
   
   lltostr(pBoat->longitude, pStrBuf);
   GUI_DispStringAt(pStrBuf, base_x+10, base_y+38);
   

   GUI_SetFont(&GUI_Font16_1);
   sprintf(pStrBuf, "%09ld", pBoat->user_id);
   GUI_DispStringAt(pStrBuf, base_x+10, base_y+62);
//   GUI_SetFont(&GUI_Font16_1);
//   GUI_DispDecAt(pBoat->SOG, base_x+10, base_y+62, 3);
//   GUI_DispDecAt(pBoat->COG, base_x+10+60, base_y+62, 3);
}

//void draw_ship(_boat *boat_ay,float x,float y,short true_heading,short center_x,short center_y){
void draw_ship(boat* pBoat,short basePoint_x,short basePoint_y,  const GUI_POINT * pPoints, const int count){

	GUI_POINT aEnlargedPoints[11];

 if(pBoat == NULL)
    return ;
 
	GUI_RotatePolygon(aEnlargedPoints,pPoints,PointNum,pBoat->COG*3.14/180);

	if((basePoint_x >=MAP_LEFT)&&(basePoint_x<= MAP_RIGHT)&&(basePoint_y >=MAP_TOP)&&(basePoint_y <= MAP_BOTTOM))
 {
	  GUI_DrawPolygon(aEnlargedPoints,PointNum,basePoint_x,basePoint_y);
   MNT_dispInfo(basePoint_x, basePoint_y, pBoat);
 }
}


void drawColrofulBoat(boat * pBoat, short basePoint_x, short basePoint_y, const GUI_POINT * pPoints, const int count, GUI_COLOR color)
{
	GUI_POINT aEnlargedPoints[11];

 if(pBoat == NULL)
    return;
 
 GUI_SetColor(color);
	GUI_RotatePolygon(aEnlargedPoints,pPoints,count,pBoat->COG*3.14/180);

	if((basePoint_x >=MAP_LEFT)&&(basePoint_x<= MAP_RIGHT)&&(basePoint_y >=MAP_TOP)&&(basePoint_y <= MAP_BOTTOM))
 {
	  GUI_DrawPolygon(aEnlargedPoints,count,basePoint_x,basePoint_y);
 }

}


void draw_mothership(short basePoint_x, short basePoint_y)
{
	 	GUI_POINT aEnlargedPoints[GUI_COUNTOF(bPoints)];
  	
	  basePoint_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + basePoint_x;
	  basePoint_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - basePoint_y;
	
  	GUI_RotatePolygon(aEnlargedPoints,bPoints,GUI_COUNTOF(bPoints),mothership.COG*3.14/180);
	  
	  if(     (basePoint_x>= MAP_LEFT)  &&  (basePoint_x <= MAP_RIGHT) 
						&&  (basePoint_y >= MAP_TOP)  &&  (basePoint_y <= MAP_BOTTOM)  )
		{
     GUI_SetColor(MM_BOAT_COLOR);
     GUI_SetPenSize(MM_BOAT_PENSIZE);
     
			  GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(bPoints),basePoint_x, basePoint_y);
     MNT_dispInfo(basePoint_x, basePoint_y, &mothership); 
		}
}



void disp_fish_boat(const scale_map * scale,long center_longitude, long center_latitude,SIMP_BERTH* pSimpBerth,short N){
	
	short i=0;
 short base_x = 0;
 short base_y = 0;
	for(i=0;i<N;i++)
	{
    base_x  = 1.0*scale->pixel * (pSimpBerth[i].longitude-center_longitude) / scale->minute;
			 base_y =1.0*scale->pixel * (pSimpBerth[i].latitude-center_latitude) / scale->minute;
    
    base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
    base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y;    
//				draw_ship(pSimpBerth[i].pBoat,base_x,base_y,aPoints, 3);	
    if(pSimpBerth[i].pBerth->Boat.target)
    {
       drawColrofulBoat(&(pSimpBerth[i].pBerth->Boat), base_x, base_y, Points_boat,  3, GUI_RED);
       
       if( MYABS(__cursor.x-base_x)<=10 && MYABS(__cursor.y-base_y)<=10 )
       {
          MNT_dispInfo(base_x, base_y, &(pSimpBerth[i].pBerth->Boat));        
       }
    }
//		}
  
	}

	
//	base_x  = 1.0*scale->pixel * (mothership.longitude - center_longitude) / scale->minute;
//	base_y  = 1.0*scale->pixel * (mothership.latitude - center_latitude) / scale->minute;
////	draw_mothership(base_x,base_y);
// draw_ship(&mothership, base_x, base_y, bPoints, 5);
 
 
}





void MNT_dispBoat(const scale_map * scale,  long center_lg, long center_lt, MNT_BERTH * pIterator)
{
   short base_x  = 0;
   short base_y  = 0;
//   GUI_EnableAlpha(1);
//   GUI_SetAlpha(0x80);
   
   while(pIterator)
   {
      if(pIterator->pBoat)
      {
         base_x  = 1.0*scale->pixel * (pIterator->pBoat->longitude - center_lg) / scale->minute;
         base_y  = 1.0*scale->pixel * (pIterator->pBoat->latitude - center_lt) / scale->minute;
         
         base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
         base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y;
    
///  Org boat conf.
         GUI_SetColor(GUI_LIGHTGRAY);    
         GUI_SetPenSize(2); 
         
///   DSP boat conf.      
         if(pIterator->mntBoat.mntSetting.DSP_Setting.isEnable == ENABLE) 
         {
            GUI_SetColor(GUI_LIGHTCYAN);    
         }
         
         draw_ship(pIterator->pBoat, base_x, base_y,pPoints, PointNum);
    
///   BGL circle conf.
         if(pIterator->mntBoat.mntSetting.BGL_Setting.isEnable == ENABLE)
         {
            GUI_SetColor(BGL_BOAT_COLOR);
            GUI_SetPenSize(1); 
            
            GUI_DrawCircle(base_x, base_y,pIterator->mntBoat.mntSetting.BGL_Setting.Dist*scale->pixel/scale->minute);            
         } 
    
///   Drg circle conf. 
         if(pIterator->mntBoat.mntSetting.DRG_Setting.isEnable == ENABLE)
         {
            base_x  = 1.0*scale->pixel * (pIterator->mntBoat.lg - center_lg) / scale->minute;
            base_y  = 1.0*scale->pixel * (pIterator->mntBoat.lt - center_lt) / scale->minute;
            
            base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
            base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y;
            
            GUI_SetColor(DRG_BOAT_COLOR);
            GUI_SetPenSize(DRG_PENSIZE);        
            GUI_DrawCircle(base_x, base_y, pIterator->mntBoat.mntSetting.DRG_Setting.Dist*scale->pixel/scale->minute);
         }
      }
      else
      {
         base_x  = 1.0*scale->pixel * (pIterator->mntBoat.lg - center_lg) / scale->minute;
         base_y  = 1.0*scale->pixel * (pIterator->mntBoat.lt - center_lt) / scale->minute;
         
         base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
         base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y; 
      
         GUI_SetColor(GUI_YELLOW);      
         GUI_DrawCircle(base_x, base_y, 20);
      }
      pIterator  = pIterator->pNext;
   }
   
   
   	base_x  = 1.0*scale->pixel * (mothership.longitude - center_lg) / scale->minute;
	   base_y  = 1.0*scale->pixel * (mothership.latitude - center_lt) / scale->minute;
    
   	draw_mothership(base_x,base_y);   
   
    disp_fish_boat(scale, center_lg, center_lt, SimpBerthes, N_boat);
}




