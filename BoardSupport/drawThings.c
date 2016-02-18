#include "drawThings.h"
#include "Setting.h"
#include "skinColor.h"
#include "mapWin.h"
#include "str.h"
#include "28.h"
#include "Compass.c"
#include "map.h"
#include "16.h"
#include "snap.h"

/// Just for copy

/** @brief    
 *  
 *  @dscrp    
 *  @input    
 *  @output
 *  @other    
 */


/*--------------------------------- Macro defines -------------------------------------*/
#define MYABS(x)   ((x)>0?(x):(-(x)))


#define NICK_SHIFT_X  16
#define NICK_SHIFT_Y  50

/*--------------------------------- Global variables ----------------------------------*/

const GUI_POINT  Points_Mute[6]  = {
                                    {  0, -15},
                                    { 15, -15},
                                    { 45, -30},
                                    { 45,  30},
                                    { 15,  15},
                                    {  0,  15}
                                               };
                                    
const GUI_POINT  Points_boat[3]  = {
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
const GUI_POINT  Points_fish[11]  = {   {  0, 15},
                                        {  8, 19},
                                        {  2, 11},
                                        { 10, -3},
                                        {  2,-19},
                                        {  1,-16},
                                        { -1,-16},
                                        { -2,-19},
                                        {-10, -3},
                                        { -2, 11},
                                        { -8, 19}
                                       }; 

 /* The shape of mothership */                                                          
//                *                                                                                          
//              *   *
//            *       *
//          *           *
//           *         *
//            *       *
//             *     *
 //             *****      
const GUI_POINT bPoints[5] = {              
		                                      {0,-18},
																																								{9,-3},  
																																								{6,18}, 
																																								{-6,18}, 
																																								{-9,-3}  
	};

///                                     0      1     2     3     4     5     6     7     8     9
static const unsigned char Nums[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
map_scale AutoScale  = {100, 100};

/*-------------------------------- External variables ---------------------------------*/
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
extern int N_boat;
extern char scale_choose;
extern _cursor __cursor;

extern FunctionalState isMntEnable;

extern unsigned char* pacwater;

extern void*  pSnapLink;
extern char   snapType;


/*-------------------------------- Local variables ------------------------------------*/
static const GUI_POINT * pPoints  = Points_fish;
static  int PointNum  = 11;

static const MapWin_COLOR * pSkin  = mapSkins;


char isAdsorbed  = 0;
long AdsorbedMMSI  = 0;

/** @brief   draw_scale
 *
 *  @dscrp:  Draw scale ruler on map in mapWin.
 *  @input:  pScale  -- Current scale pointer.
 *           x,y     -- Pixel location of the left-bottom vertic.
 *  @output:
 *  @return:
 *  @other : eg.          0.01 km
 *                |______________________|
 */
static void draw_scale(const map_scale * pScale, short x, short y)
{
   uint16_t length  = 0;
   
   if(SysConf.Unit==UNIT_nm)
   {
      length  = pScale->pixel;
   }
   else 
   {
      length  = pScale->pixel * 5 /9;
   }

   GUI_SetColor(pSkin->sclColor);
   GUI_SetPenSize(1);
   GUI_SetLineStyle(GUI_LS_SOLID);
   GUI_SetFont(&GUI_Font16_1);
   
   GUI_DrawHLine(y, x, x+length);
   GUI_DrawVLine(x, y-5, y);
   GUI_DrawVLine(x+length, y-5, y);
   sprintf(pStrBuf, "%ld.%02ld%s",pScale->minute/1000,(pScale->minute%1000)/10,SysConf.Unit==UNIT_nm?"nm":"km");
   GUI_DispStringAt(pStrBuf,x+10,y-17);
}


/** @brief   setBoatSkin
 *
 *  @dscrp   Set pointer pointing different color scheme.
 *  @input   skin   -- The index of color scheme (SKINS_Day  or SKINS_Night)
 *  @output
 */
void setBoatSkin(SKINS skin)
{
   pSkin  = &(mapSkins[skin]);
}


/** @brief    setShape
 *  
 *  @dscrp    Switch the shape scheme of boat.
 *  @input    shape  -- The index of shpae scheme (SHAPE_Boat  or SHAPE_Fish)
 *  @output
 *  @other
 */
void setShape(BOAT_SHAPES shape)
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

static void _drawNum(unsigned char num, short x, short y, short penSize)
{
   short size  = penSize-1;
   
   if(num&0x01)
      GUI_FillRect(x,   y,   x+99,   y+size);
   if(num&0x02)
      GUI_FillRect(x+99-size, y, x+99, y+99);
   if(num&0x04)
      GUI_FillRect(x+99-size, y+100, x+99,y+199);
   if(num&0x08)
      GUI_FillRect(x, y+199-size, x+99, y+199);
   if(num&0x10)
      GUI_FillRect(x, y+100, x+size,y+199);
   if(num&0x20)
      GUI_FillRect(x, y, x+size, y+99);
   if(num&0x40)
      GUI_FillRect(x, y+99-size/2, x+99, y+99+size/2);
}



void  drawNum(unsigned char num, short x, short y, short penSize)
{
   _drawNum(Nums[num], x, y, penSize);
}



/** @brief    fixPos
 *  
 *  @dscrp    Calculate the location of tip area based on reference point of boat which tip belong to.
 *  @input    x,y  -- The pixel location of reference point.
 *  @output
 *  @other    
 */
static void fixPos(short * base_x, short * base_y)
{
   if(*base_x + 10  > MAP_RIGHT-180)
   {
      *base_x  = *base_x-10-130;
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


static void draw_batSlot(short x, short y)
{
   GUI_DrawRect(x+1, y,   x+2, y+1);
   GUI_DrawRect(x,   y+2, x+1, y+3);
   GUI_DrawRect(x-1, y+4, x,   y+5);
   GUI_DrawRect(x-2, y+6, x-1, y+7);
//   GUI_DrawVLine(x, y+6, y+7);
}


/** @brief   draw_boatInfo
 *  
 *  @dscrp   Draw tip area and show information specified by options
 *  @input   
 *  @output
 *  @other    
 */
static void draw_boatInfo( short base_x, short base_y, boat * pBoat,unsigned char options)
{
   if(pBoat == NULL)
      return ;


   if(options)
   {
      fixPos(&base_x,  &base_y);
      GUI_SetTextMode(GUI_TM_NORMAL);

       
      /// Show battery in tip.
      if( options & SHOW_OPTION_BAT )
      {
         if(1 && pBoat->isHSD)
         {                
            char   i  = 0;            
            char   power      = (pBoat->COG/10) % 10;
            short  batBody_x  = base_x +10;
            short  batBody_y  = base_y -11;
            
            GUI_SetColor(pSkin->map_tip_Bat);
            GUI_DrawRect(batBody_x, batBody_y, batBody_x + 19, batBody_y+10); 
            GUI_DrawVLine(batBody_x+20, batBody_y+3, batBody_y+7);
            
                  
            i  = (power + 2) / 3;           
            GUI_SetColor(SysConf.Skin == SKIN_Day?GUI_BLACK:GUI_WHITE);
            GUI_DrawRect(batBody_x, batBody_y, batBody_x+22, batBody_y+11);
            GUI_DrawRect(batBody_x+23, batBody_y+4, batBody_x+24, batBody_y+7);
            GUI_SetColor(pSkin->bkColor);
            GUI_FillRect(batBody_x+1, batBody_y+1, batBody_x+21, batBody_y+10);
            switch(i)            
            {
               case 1:
                    GUI_SetColor(GUI_RED);
                    GUI_FillRect(batBody_x+2, batBody_y+2,batBody_x+7, batBody_y+9);
                    GUI_SetColor(pSkin->bkColor);
                    draw_batSlot(batBody_x+7, batBody_y+2);
                    GUI_DrawVLine(batBody_x+7, batBody_y+8, batBody_y+9);
//                    GUI_DrawPixel(batBody_x+7, batBody_y+8);
                    break;
                case 2:
                     GUI_SetColor(pSkin->map_tip_Bat);
                     GUI_FillRect(batBody_x+2, batBody_y+2, batBody_x+15,batBody_y+9);
                     GUI_SetColor(pSkin->bkColor);
                     draw_batSlot(batBody_x+7, batBody_y+2);
                     draw_batSlot(batBody_x+15,batBody_y+2);
//                     GUI_DrawVLine(batBody_x+7, batBody_y+8, batBody_y+9);
                     GUI_DrawPixel(batBody_x+7, batBody_y+8);
                     GUI_DrawVLine(batBody_x+15,batBody_y+8, batBody_y+9);
//                     GUI_DrawPixel(batBody_x+15,batBody_y+8);
                     break;
                 case 3:
                     GUI_SetColor(pSkin->map_tip_Bat);
                     GUI_FillRect(batBody_x+2, batBody_y+2, batBody_x+20,batBody_y+9);
                     GUI_SetColor(mapSkins->bkColor);
                     draw_batSlot(batBody_x+7, batBody_y+2);
                     draw_batSlot(batBody_x+15,batBody_y+2);
                     break;
            }
            
//            if(i>1)
//               GUI_SetColor(GUI_DARKGREEN);
//            else 
//               GUI_SetColor(GUI_DARKRED);
////printf("i:%d\n\r",i);               
//            for(;i>0;i--)
//            {
//               GUI_FillRect(batBody_x + i*6 -4, batBody_y + 2, batBody_x+i*6-1, batBody_y+8);
//            }
         }
      }
     
      GUI_SetColor(pSkin->map_tip_Text);
      /// Show name in tip.
      if(options & SHOW_OPTION_NAME)
      {
         GUI_SetFont(&GUI_Font16_1);
         GUI_DispStringAt(pBoat->name, base_x+10, base_y+1); 
      }
      
      /// Show ll in tip.
      if(options & SHOW_OPTION_LL)
      {
         GUI_SetFont(&GUI_Font24_1);
         lltostr(pBoat->latitude, pStrBuf);
         GUI_DispStringAt(pStrBuf, base_x+10, base_y+16);
         lltostr(pBoat->longitude, pStrBuf);
         GUI_DispStringAt(pStrBuf, base_x+10, base_y+38);
      }
      
      /// Show MMSI in tip.
      if(options & SHOW_OPTION_SCOG)
      {
//         GUI_SetFont(&GUI_Font16_1);
//         sprintf(pStrBuf, "%09ld", pBoat->user_id);
//         sprintf(pStrBuf, "%d  %d", pBoat->SOG,pBoat->COG);
//         sprintf(pStrBuf, "SOG:%3d", pBoat->SOG);
         GUI_SetFont(&GUI_Font16);
         GUI_DispStringAt("航速", base_x+10, base_y+62); 
         GUI_DispStringAt("航向", base_x+80, base_y+62);         
         
         GUI_SetFont(&GUI_Font16_1);
//         if(SysConf.Unit == UNIT_km)
//         {
//            int sog  = pBoat->SOG *18;
//            sprintf(pStrBuf, "%d.%02d",sog/100, sog%100);
//         }
//         else
//         {
//            sprintf(pStrBuf, "%d.%d",pBoat->SOG/10, pBoat->SOG%10);
//         }
         sprintf(pStrBuf, "%d.%dkt", pBoat->SOG/10, pBoat->SOG%10);
         GUI_DispStringAt(pStrBuf, base_x +32, base_y+62);
         
         sprintf(pStrBuf, "%3d", pBoat->COG/10);         
         pStrBuf[3]  = 194;
         pStrBuf[4]  = 176;
         pStrBuf[5]  = '\0';
         GUI_DispStringAt(pStrBuf, base_x+100, base_y +62);
      }
      
      
      if(options & SHOW_OPTION_DST)
      {
         if(pBoat->dist < 99999)
         {
            GUI_SetFont(&GUI_Font16);
            GUI_DispStringAt("距离本船",base_x+10, base_y+78);
            
            GUI_SetFont(&GUI_Font16_1);
         
            if(SysConf.Unit == UNIT_km)
            {
               int dist  = pBoat->dist *100 /54;
               sprintf(pStrBuf, "%0d.%2d km",dist /1000, (dist %1000) /10);
            }
            else
            {
               sprintf(pStrBuf, "%0d.%2d nm",pBoat->dist /1000, (pBoat->dist %1000) /10);
            }
            GUI_DispStringAt(pStrBuf, base_x+80, base_y+78);
            
           
         }
      }
   }
}


static void draw_dspBoatInfo(short base_x, short base_y, MNT_BERTH* pMntBerth)
{
   if(pMntBerth == NULL)
      return; 
   fixPos(&base_x, &base_y); 
   GUI_SetTextMode(GUI_TM_NORMAL);
   
   GUI_SetColor(pSkin->map_tip_Text);
   GUI_SetFont(&GUI_Font16_1);
   GUI_DispStringAt(pMntBerth->mntBoat.name, base_x+10, base_y+1);
   GUI_SetFont(&GUI_Font24_1);
   lltostr(pMntBerth->snapLg, pStrBuf);
   GUI_DispStringAt(pStrBuf, base_x+10, base_y+16);
   lltostr(pMntBerth->snapLt, pStrBuf);
   GUI_DispStringAt(pStrBuf, base_x+10, base_y+38);
   GUI_SetFont(&GUI_Font16_1);
   if(SysConf.Unit == UNIT_km)
   {
      int dist  = pMntBerth->snapDist *100 /54;
      sprintf(pStrBuf, "%d.%2dkm", dist /1000, dist %1000 /100);
   }
   else
   {
      sprintf(pStrBuf, "%d.%2dnm",pMntBerth->snapDist /1000, pMntBerth->snapDist %1000 /100);
   }
   GUI_DispStringAt(pStrBuf, base_x+50, base_y+78);
   GUI_SetFont(&GUI_Font16);
   GUI_DispStringAt("距离本船",base_x+10, base_y+78);
}

/** @brief     draw_boat
 *  
 *  @dscrp     Draw boat with specified shape 
 *  @input    
 *  @output
 *  @other    
 */
static void draw_boat(boat* pBoat,short basePoint_x,short basePoint_y,  const GUI_POINT * pPoints, const int count){

   GUI_POINT aEnlargedPoints[11];

   if(pBoat == NULL)
      return ;
   
   GUI_RotatePolygon(aEnlargedPoints,pPoints,count,(360-pBoat->COG/10)*3.14/180);   
   GUI_DrawPolygon(aEnlargedPoints,count,basePoint_x,basePoint_y);
}




/** @brief    draw_mothership
 *  
 *  @dscrp    Draw mothership on specified ll and scale.
 *  @input    
 *  @output
 *  @other    
 */
static void draw_mothership(const long lg, const long lt,const map_scale * pScale)
{
   short basePoint_x  = 0;
   short basePoint_y  = 0;
   
   GUI_POINT aEnlargedPoints[GUI_COUNTOF(bPoints)];
   
   basePoint_x  = (mothership.longitude-lg) * pScale->pixel / pScale->minute;
   basePoint_y  = (mothership.latitude -lt) * pScale->pixel / pScale->minute;
  	
	  basePoint_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + basePoint_x;
	  basePoint_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - basePoint_y;
	
  	GUI_RotatePolygon(aEnlargedPoints,bPoints,GUI_COUNTOF(bPoints),(360-mothership.COG/10)*3.14/180);
	  
	  if(     (basePoint_x>= MAP_LEFT)  &&  (basePoint_x <= MAP_RIGHT) 
						&&  (basePoint_y >= MAP_TOP)  &&  (basePoint_y <= MAP_BOTTOM)  )
		 {
      GUI_SetColor(pSkin->boat_Mom);
      GUI_SetPenSize(MM_BOAT_PENSIZE);
      
      GUI_DrawPolygon(aEnlargedPoints,GUI_COUNTOF(bPoints),basePoint_x, basePoint_y);
	 	}
}



/** @brief    disp_boat 
 *  
 *  @dscrp    Display all invader 
 *  @input    
 *  @output
 *  @other    
 */
static void disp_boat(const long lg, const long lt, const map_scale * pScale,short N)
{
	
   short i=0;
   short base_x = 0;
   short base_y = 0;
   
   int isCursorVisible  = GUI_CURSOR_GetState();
   
   GUI_SetPenSize(1);
   GUI_SetLineStyle(GUI_LS_SOLID);
   
   for(i=0;i<N;i++)
   {
      base_x  = pScale->pixel * (SimpBerthes[i].longitude-lg) / pScale->minute;
      base_y  = pScale->pixel * (SimpBerthes[i].latitude-lt) / pScale->minute;
      
      base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
      base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y;    

      /// This boat will be drew
      if(SimpBerthes[i].pBerth->isInvader)
      {
         if((base_x >=MAP_LEFT)&&(base_x<= MAP_RIGHT)&&(base_y >=MAP_TOP)&&(base_y <= MAP_BOTTOM))
         {  
//            /// And its tip should to be drew.         
//            if( !isAdsorbed  &&  isCursorVisible  &&  ( MYABS(base_x-__cursor.x) <= 8)  &&  ( MYABS(base_y-__cursor.y) <= 8) )
            if(!AdsorbedMMSI &&  ( MYABS(base_x-__cursor.x) <= 8)  &&  ( MYABS(base_y-__cursor.y) <= 8))
            {
               AdsorbedMMSI  = SimpBerthes[i].pBerth->Boat.user_id;
            }
            if( AdsorbedMMSI == SimpBerthes[i].pBerth->Boat.user_id  &&  isCursorVisible )
            {
               GUI_SetColor(pSkin->ttl_Context);
               draw_boat(&SimpBerthes[i].pBerth->Boat, base_x, base_y, Points_boat, 3);
               draw_boatInfo(base_x, base_y, &SimpBerthes[i].pBerth->Boat, SHOW_OPTION_NAME | SHOW_OPTION_SCOG | SHOW_OPTION_LL | SHOW_OPTION_DST);
//               isAdsorbed  = 1;
            }
            else
            {
               GUI_SetColor(pSkin->ttl_Label);
               draw_boat(&SimpBerthes[i].pBerth->Boat, base_x, base_y, Points_boat, 3);
            }
         }        
      }
   }
}




/** @brief     disp_mntBoat
 *  
 *  @dscrp     Display all monitored boat.
 *  @input    
 *  @output
 *  @other    
 */
static void disp_mntBoat(const long center_lg,const long center_lt, const map_scale* pScale)
{
   short base_x  = 0;
   short base_y  = 0;
   short base_cur_x = 0;
   short base_cur_y = 0;
   
   int  isCursorVisible  = GUI_CURSOR_GetState();
   
   MNT_BERTH * pIterator  = pMntHeader;
   
//   if(isMntEnable)
//   {
      while(pIterator)
      { 
         if(pIterator->cfgState != MNTState_Monitored)
         {
            pIterator  = pIterator->pNext;
            continue;
         }
         /// Exist and ll valid
//         if(pIterator->pBerth  &&  pIterator->pBerth->Boat.user_id == pIterator->mntBoat.mmsi  && pIterator->pBerth->Boat.dist < 100000)
         if(pIterator->pBerth  &&  pIterator->pBerth->Boat.dist < 100000)
         {
            base_x  = pScale->pixel * (pIterator->pBerth->Boat.longitude - center_lg) / pScale->minute;
            base_y  = pScale->pixel * (pIterator->pBerth->Boat.latitude - center_lt) / pScale->minute;
            
            base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
            base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y;
            
            base_cur_x = base_x;
            base_cur_y = base_y;
       
     
            GUI_SetPenSize(2);            
            GUI_SetLineStyle(GUI_LS_SOLID);           

             ///DSP boat conf.      
            if(pIterator->mntBoat.mntSetting.DSP_Setting.isEnable == ENABLE) 
            {
               GUI_SetColor(pSkin->boat_Dsp);    
            }
            if((base_x >=MAP_LEFT)&&(base_x<= MAP_RIGHT)&&(base_y >=MAP_TOP)&&(base_y <= MAP_BOTTOM))
            {
//               if(!isAdsorbed  &&  isCursorVisible  && ( MYABS(base_x-__cursor.x) <= 8)  &&  ( MYABS(base_y-__cursor.y) <= 8) )
               if(!AdsorbedMMSI &&  ( MYABS(base_x-__cursor.x) <= 8)  &&  ( MYABS(base_y-__cursor.y) <= 8))
               {
                  AdsorbedMMSI  = pIterator->mntBoat.mmsi;
               }
               if(AdsorbedMMSI == pIterator->mntBoat.mmsi  &&  isCursorVisible )
               {
//                  GUI_SetColor(pSkin->ttl_Context);
                  GUI_SetFont(&GUI_Font30);
                  GUI_SetColor(mapSkins->ttl_Context);
                  draw_boat(&(pIterator->pBerth->Boat), base_x, base_y,pPoints, PointNum);
                  GUI_DispStringAt(pIterator->nickName, base_x-NICK_SHIFT_X, base_y-NICK_SHIFT_Y);
                  draw_boatInfo(base_x, base_y, &(pIterator->pBerth->Boat), SHOW_OPTION_NAME | SHOW_OPTION_LL | SHOW_OPTION_SCOG | SHOW_OPTION_BAT | SHOW_OPTION_DST);
               }
               else
               {

                  GUI_SetColor(pSkin->boat_Org);  
                  if( (pIterator->trgState&0x0f) == MNTState_Triggered)
                  {
                     GUI_SetColor(GUI_RED);
                     if(pIterator->flsState&0x01)
                     {
                        GUI_SetFont(&GUI_Font30);
                        draw_boat(&(pIterator->pBerth->Boat), base_x, base_y,pPoints, PointNum);
                        GUI_DispStringAt(pIterator->nickName, base_x-NICK_SHIFT_X, base_y-NICK_SHIFT_Y); 
                     }
                     pIterator->flsState  = ~pIterator->flsState;
                  }   
                  else
                  {
                     GUI_SetFont(&GUI_Font30);
                     draw_boat(&(pIterator->pBerth->Boat), base_x, base_y,pPoints, PointNum);
                     GUI_DispStringAt(pIterator->nickName, base_x-NICK_SHIFT_X, base_y-NICK_SHIFT_Y);
                  }
               }
            }   
            
            ///   Drg circle conf. 
            if(pIterator->cfgState == MNTState_Monitored  &&  pIterator->mntBoat.mntSetting.DRG_Setting.isEnable == ENABLE)
            {
               base_x  = pScale->pixel * (pIterator->mntBoat.lg - center_lg) / pScale->minute;
               base_y  = pScale->pixel * (pIterator->mntBoat.lt - center_lt) / pScale->minute;
               
               base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
               base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y;
               
               GUI_SetColor(pSkin->boat_Drg);      
               GUI_DrawCircle(base_x, base_y, pIterator->mntBoat.mntSetting.DRG_Setting.Dist*pScale->pixel/pScale->minute);
               
               if(pIterator->trgState&(0x01<<6))
               {
                  GUI_SetPenSize(1);
                  GUI_SetColor(GUI_GRAY);
                  GUI_SetLineStyle(GUI_LS_DOT);
                  GUI_DrawLine(base_cur_x, base_cur_y, base_x, base_y);
               }
            }        
            
            ///   BGL circle conf.
            if(pIterator->mntBoat.mntSetting.BGL_Setting.isEnable == ENABLE  &&  (pIterator->trgState&0xf0) < (0x01<<6) )
            {
               GUI_SetColor(pSkin->boat_Bgl);    
               GUI_SetLineStyle(GUI_LS_SOLID);               
               GUI_DrawCircle(base_cur_x, base_cur_y,pIterator->mntBoat.mntSetting.BGL_Setting.Dist*pScale->pixel/pScale->minute);            
            } 
       

         }
//         else if(pIterator->cfgState == MNTState_Monitored)
         else
         {
            base_x  = pScale->pixel * (pIterator->snapLg - center_lg) / pScale->minute;
            base_y  = pScale->pixel * (pIterator->snapLt - center_lt) / pScale->minute;
            
            base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
            base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y; 
            
            if( (base_x >= MAP_LEFT)  &&  (base_x <= MAP_RIGHT)  &&  (base_y >= MAP_TOP)  &&  (base_y <= MAP_BOTTOM)  )
            {
               GUI_SetPenSize(1);
               GUI_SetLineStyle(GUI_LS_DOT);
               GUI_SetFont(&GUI_Font30);
//               if(!isAdsorbed  &&  isCursorVisible  &&  (MYABS(base_x-__cursor.x) <= 12)  &&  (MYABS(base_y-__cursor.y) <= 12))
               if(!AdsorbedMMSI  &&   (MYABS(base_x-__cursor.x) <= 12)  &&  (MYABS(base_y-__cursor.y) <= 12))
               {
                  AdsorbedMMSI  = pIterator->mntBoat.mmsi;
               }
               if(AdsorbedMMSI==pIterator->mntBoat.mmsi  &&  isCursorVisible)
               {
                  GUI_SetColor(pSkin->ttl_Context);
                  GUI_DrawPolygon(pPoints, PointNum, base_x, base_y);
                  GUI_DispStringAt(pIterator->nickName, base_x-NICK_SHIFT_X, base_y-NICK_SHIFT_Y);
                  draw_dspBoatInfo( base_x, base_y, pIterator);
               }
               else
               {
                  GUI_SetColor(GUI_RED);
                  GUI_DrawPolygon(pPoints, PointNum, base_x, base_y);
                  GUI_DispStringAt(pIterator->nickName, base_x-NICK_SHIFT_X, base_y-NICK_SHIFT_Y);
               }
            }
         }
         
         
         pIterator  = pIterator->pNext;
      }
//   }
   
   /// GUI_KEY_MNT_Cancel
//   else
//   {
//      GUI_SetColor(pSkin->boat_Org);
//      GUI_SetPenSize(2);
//      GUI_SetLineStyle(GUI_LS_SOLID);
//      while(pIterator)
//      {
//         if(pIterator->pBerth  &&  pIterator->pBerth->Boat.user_id == pIterator->mntBoat.mmsi  && pIterator->pBerth->Boat.dist < 100000)
//         {
//            base_x  = pScale->pixel * (pIterator->pBerth->Boat.longitude - center_lg) / pScale->minute;
//            base_y  = pScale->pixel * (pIterator->pBerth->Boat.latitude - center_lt) / pScale->minute;
//            
//            base_x  = (MAP_LEFT/2 + MAP_RIGHT/2) + base_x;
//            base_y  = (MAP_TOP/2 + MAP_BOTTOM/2) - base_y;
//            
//            GUI_SetFont(&GUI_Font30);
//            
//            if(!AdsorbedMMSI  && ( MYABS(base_x-__cursor.x) <= 12)  &&  ( MYABS(base_y-__cursor.y) <= 12) )
//            {
//               AdsorbedMMSI  = pIterator->mntBoat.mmsi;
//            }
//            
//            if(AdsorbedMMSI == pIterator->mntBoat.mmsi  &&  isCursorVisible)
//            {
//               
//               GUI_SetColor(pSkin->ttl_Context);
//               draw_boat(&(pIterator->pBerth->Boat), base_x, base_y, pPoints, PointNum);
//               GUI_DispStringAt(pIterator->nickName, base_x - NICK_SHIFT_X, base_y - NICK_SHIFT_Y);
//               draw_boatInfo(base_x, base_y, &(pIterator->pBerth->Boat), SHOW_OPTION_NAME|SHOW_OPTION_SCOG|SHOW_OPTION_LL|SHOW_OPTION_BAT);
//            }
//            else
//            {
//               GUI_SetColor(pSkin->boat_Org);
//               draw_boat(&(pIterator->pBerth->Boat), base_x, base_y, pPoints, PointNum);
//               GUI_DispStringAt(pIterator->nickName, base_x - NICK_SHIFT_X, base_y - NICK_SHIFT_Y);
//            }
//            draw_boat(&(pIterator->pBerth->Boat), base_x, base_y,pPoints, PointNum);            
//         }
//         
//         pIterator  = pIterator->pNext;
//      }
//   }
}



 
/** @brief    draw_map_grid
 *  
 *  @dscrp    Draw grid on map
 *  @input    
 *  @output
 *  @other    
 */
static void draw_map_grid(mapping gridAnchor,const map_scale * pScale)
{
   short x = 0;
   short y = 0;

   x  = gridAnchor.x;
   y  = gridAnchor.y;
   
   GUI_SetLineStyle(GUI_LS_DASH);
   GUI_SetColor(pSkin->map_Grid);
     
   
   while(x<MAP_LEFT)
   {
      x  += pScale->pixel;
   }
   while(x<MAP_RIGHT)
   {
      GUI_DrawLine(x, MAP_TOP, x, MAP_BOTTOM);
      x  += pScale->pixel;
   }
   
   while(y<MAP_TOP)
   {
      y  += pScale->pixel;
   }
   while(y<MAP_BOTTOM)
   {
      GUI_DrawLine(MAP_LEFT, y, MAP_RIGHT, y);
      y  += pScale->pixel;
   }
}





static unsigned int getAreaIdByLL(long lg,  long lt)
{
   int i  = 0;
   int diffCnt  = 0;
   int fishingAreaId  = 0;

   
   if(lg < 6360000 || lg >=7890000 || lt <= 210000 || lt > 2460000)
   {
      return 0;
   }
   
   for(i=0; i<N_areaRecord; i++)
   {
      /// Find record 'lt' of whihc contains lt
      if(lt > fishingAreaRecord[i].lt-29999)
      {  
         if(lt > fishingAreaRecord[i].lt)      
         {
            return 0;
         }
         /// Not on left of org.
         if(lg >= fishingAreaRecord[i].lg)
         {
            diffCnt  = (lg-fishingAreaRecord[i].lg) / 30000 + 1;
            
            /// On org area.
            if(diffCnt <= fishingAreaRecord[i].orgAreaNum)
            {
               fishingAreaId  = fishingAreaRecord[i].bgnId+diffCnt-1;
                     return fishingAreaId;
//               return (fishingAreaRecord[i].bgnId+diffCnt);
            }
            /// On exp area   
            else if(diffCnt <= fishingAreaRecord[i].expAreaNum+fishingAreaRecord[i].orgAreaNum)
            {
               /// In unmasked exp area
               if(   diffCnt > fishingAreaRecord[i].orgAreaNum+fishingAreaRecord[i].expMaskIdR  
                  || diffCnt < fishingAreaRecord[i].orgAreaNum+fishingAreaRecord[i].expMaskIdL )  
               {
                  if(fishingAreaRecord[i].bgnId+fishingAreaRecord[i].orgAreaNum-1 < 99)
                     fishingAreaId  = (fishingAreaRecord[i].bgnId+fishingAreaRecord[i].orgAreaNum-1) * 100 + diffCnt-fishingAreaRecord[i].orgAreaNum;
                  else
                     fishingAreaId  = (fishingAreaRecord[i].bgnId+fishingAreaRecord[i].orgAreaNum-1) * 10 + diffCnt-fishingAreaRecord[i].orgAreaNum;                   
                  return fishingAreaId;
               }
               /// In masked exp area.
               else
               {
                  return 0;
               }
            }
            /*
            else
            {
               continue;
            }
            */
         }
         /// On left of org means out of fishing area.
         else
         {
            return 0;
         }
      }
      /*
      else
      {
         coninue;
      }
      */

   }
   return 0;
}




/** @brief    disp_map
 *  
 *  @dscrp    
 *  @input    
 *  @output
 *  @other    
 */
static void disp_map(const long longitude, const long latitude,const map_scale * pScale)
{
   unsigned int exp_id  = 0;   
   int row  = 0;
   int col  = 0;   
    
  
   short tmp_x = 0;
   short tmp_y = 0;
   
   long tmp_lttude  = 0;
   long tmp_lgtude  = 0;
  
   unsigned int fishingAreaId  = 0;
   
   mapping  anchor;
   
   short shift_x  = (pScale->pixel>>1) - 16;
   short shift_y  = (pScale->pixel>>1)- 8;
   
   anchor.lgtude  = longitude;
   anchor.lttude  = latitude;

   anchor.x  = (MAP_LEFT+MAP_RIGHT)/2 - (anchor.lgtude%pScale->minute) / (pScale->minute/pScale->pixel);
   anchor.y  = (MAP_TOP +MAP_BOTTOM)/2 + (anchor.lttude%pScale->minute) / (pScale->minute/pScale->pixel);
   anchor.lgtude  = anchor.lgtude - (anchor.lgtude%pScale->minute);
   anchor.lttude  = anchor.lttude - (anchor.lttude%pScale->minute);

	  while(anchor.x  >= MAP_LEFT+(pScale->pixel-shift_x))
		 {
			   anchor.x  -= pScale->pixel;
			   anchor.lgtude -= pScale->minute;
		 }
		 while(anchor.y  > MAP_TOP+(pScale->pixel-shift_y))
		 {
			   anchor.y  -= pScale->pixel;
			   anchor.lttude  += pScale->minute;
		 }
		  
		 draw_map_grid(anchor, pScale);
		 
		 tmp_x  = anchor.x;
		 tmp_y  = anchor.y;
		 tmp_lgtude  = anchor.lgtude;
		 tmp_lttude  = anchor.lttude;

		 GUI_SetColor (pSkin->net_lab);  
   GUI_SetFont(&GUI_Font16B_1);
   
   
   for(tmp_y=anchor.y;tmp_y<=MAP_BOTTOM;)			 
		 {				 
      if(scale_choose <=  1)
      {
         tmp_x  = anchor.x;
         tmp_lgtude  = anchor.lgtude ;
         
         for(tmp_x=anchor.x; tmp_x<=MAP_RIGHT-(pScale->pixel - shift_x);)
         {       
            fishingAreaId  = getAreaIdByLL(tmp_lgtude, tmp_lttude);         
            if(measuring_scale[1].minute == pScale->minute  && (0 != fishingAreaId))
            {
               if( 1000 > fishingAreaId)
               {
                  GUI_DispDecAt(fishingAreaId, tmp_x+shift_x, tmp_y+shift_y, 3);							 
               }
               else
               {
                  GUI_DispDecAt(fishingAreaId, tmp_x+shift_x, tmp_y+shift_y, 4);
               }
               row  = ((fishingAreaRecord[0].lt - tmp_lttude) % 30000) / 10000;
               col  = (tmp_lgtude % 30000) / 10000;
               exp_id  = row*3+col+1;                                          
               GUI_DispChar('_' );
               GUI_DispDec(exp_id, 1);							 							 
            }
            else if((measuring_scale[0].minute == pScale->minute) && (0 != fishingAreaId))
            {
               if(1000 > fishingAreaId)
               {
                  GUI_DispDecAt(fishingAreaId, tmp_x+shift_x+10, tmp_y+shift_y, 3);							 
               }
               else
               {
                  GUI_DispDecAt(fishingAreaId, tmp_x+shift_x+10, tmp_y+shift_y, 4);
               }
            }
           
            tmp_x  += pScale->pixel;
            tmp_lgtude  += pScale->minute;
         }					 
      }
      if( (tmp_y+latitude_display_y_shift) >= MAP_TOP )
      {
        lltostr(tmp_lttude,pStrBuf);
        GUI_DispStringAt(pStrBuf, MAP_RIGHT+latitude_display_x_shift,tmp_y+latitude_display_y_shift);         
      }		 
      tmp_y  += pScale->pixel;
      tmp_lttude  -= pScale->minute;
			 }
    
    
}



/** @brief    getmntWrapPara
 *  
 *  @dscrp    
 *  @input    
 *  @output
 *  @other    
 */
void getMntWrapPara(long *halfDiff_lg, long* halfDiff_lt, map_scale* pScale)
{  
   long min_lg  = mothership.longitude;
   long max_lg  = mothership.longitude;
   long min_lt  = mothership.latitude;
   long max_lt  = mothership.latitude;
   
   long maxDiff_lg  = 0;
   long maxDiff_lt  = 0;
   
   long drgDist  = 0;
   long bglDist  = 0;
   
   MNT_BERTH * pIterator  = pMntHeader;
   
   while(pIterator)
   {
//      if(pIterator->cfgState == MNTState_Monitored  &&  pIterator->mntBoat.mntSetting.DRG_Setting.isEnable)
//      {
//         drgDist  = pIterator->mntBoat.mntSetting.DRG_Setting.Dist;
//         if(pIterator->mntBoat.lg - drgDist  <  min_lg)
//         {
//            min_lg  = pIterator->mntBoat.lg - drgDist;
//         }
//         else if(pIterator->mntBoat.lg + drgDist  >  max_lg)
//         {
//            max_lg  = pIterator->mntBoat.lg + drgDist;
//         }
//         
//         if(pIterator->mntBoat.lt -drgDist  <  min_lt)
//         {
//            min_lt  = pIterator->mntBoat.lt - drgDist;
//         }
//         else if(pIterator->mntBoat.lt + drgDist  >  max_lt)
//         {
//            max_lt  = pIterator->mntBoat.lt + drgDist;
//         }
//      }   
      
      if(pIterator->pBerth  &&  pIterator->pBerth->Boat.dist<99999)
      {
          if(pIterator->mntBoat.mntSetting.DRG_Setting.isEnable)
          {
             drgDist  = pIterator->mntBoat.mntSetting.DRG_Setting.Dist;
             if(pIterator->mntBoat.lg - drgDist  <  min_lg)
             {
                min_lg  = pIterator->mntBoat.lg - drgDist;
             }
             else if(pIterator->mntBoat.lg + drgDist  >  max_lg)
             {
                max_lg  = pIterator->mntBoat.lg + drgDist;
             }
             
             if(pIterator->mntBoat.lt -drgDist  <  min_lt)
             {
                min_lt  = pIterator->mntBoat.lt - drgDist;
             }
             else if(pIterator->mntBoat.lt + drgDist  >  max_lt)
             {
                max_lt  = pIterator->mntBoat.lt + drgDist;
             }
          }
           
//         if(pIterator->pBerth->Boat.dist < 99999)
//         {   
            bglDist  = pIterator->mntBoat.mntSetting.BGL_Setting.isEnable?pIterator->mntBoat.mntSetting.BGL_Setting.Dist:100;
                         
            if(pIterator->pBerth->Boat.longitude - bglDist < min_lg)
            {
               min_lg  = pIterator->pBerth->Boat.longitude - bglDist;
            }
            else if(pIterator->pBerth->Boat.longitude + bglDist > max_lg)
            {
               max_lg  = pIterator->pBerth->Boat.longitude + bglDist;
            }
            
            if(pIterator->pBerth->Boat.latitude - bglDist < min_lt)
            {
               min_lt  = pIterator->pBerth->Boat.latitude - bglDist;
            }
            else if(pIterator->pBerth->Boat.latitude + bglDist > max_lt)
            {
               max_lt  = pIterator->pBerth->Boat.latitude + bglDist;
            }
//         }
         

      }
      else if(pIterator->cfgState == MNTState_Monitored)
      {
         if(pIterator->snapLg  &&  pIterator->snapLt)
         {
            if(pIterator->snapLg < min_lg)
            {
               min_lg  = pIterator->snapLg;
            }
            else if(pIterator->snapLg > max_lg)
            {
               max_lg  = pIterator->snapLg;
            } 
            
            if(pIterator->snapLt < min_lt )
            {
               min_lt  = pIterator->snapLt;
            }
            else if(pIterator->snapLt > max_lt)
            {
               max_lt  = pIterator->snapLt ;
            }
         }
      }
      
      pIterator  = pIterator->pNext;
   }
   
//   while(pIterator)
//   {         
//         if(pIterator->mntBoat.mntSetting.DRG_Setting.isEnable   &&  pIterator->cfgState == MNTState_Monitored)
//         {
//            drgDist  = pIterator->mntBoat.mntSetting.DRG_Setting.Dist;
//            
//            if(pIterator->mntBoat.lg - drgDist < min_lg )
//            {
//               min_lg  = pIterator->mntBoat.lg - drgDist;
//            }
//            else if(pIterator->mntBoat.lg + drgDist > max_lg)
//            {
//               max_lg  = pIterator->mntBoat.lg + drgDist;
//            }
//            
//            if(pIterator->mntBoat.lt - drgDist < min_lt)
//            {
//               min_lt  = pIterator->mntBoat.lt - drgDist;
//            }
//            else if(pIterator->mntBoat.lt + drgDist > max_lt)
//            {
//               max_lt  = pIterator->mntBoat.lt + drgDist;
//            }
//         }
//         
//              
//         if(pIterator->pBerth  &&  (pIterator->pBerth->Boat.user_id == pIterator->mntBoat.mmsi)  &&  (pIterator->pBerth->Boat.dist < 100000) )
//         {
//            bglDist  = pIterator->mntBoat.mntSetting.BGL_Setting.isEnable?pIterator->mntBoat.mntSetting.BGL_Setting.Dist:100;         
//            
//            if(pIterator->pBerth->Boat.longitude - bglDist < min_lg)
//            {
//               min_lg  = pIterator->pBerth->Boat.longitude - bglDist;
//            }
//            else if(pIterator->pBerth->Boat.longitude + bglDist > max_lg)
//            {
//               max_lg  = pIterator->pBerth->Boat.longitude + bglDist;
//            }
//            
//            if(pIterator->pBerth->Boat.latitude - bglDist < min_lt)
//            {
//               min_lt  = pIterator->pBerth->Boat.latitude - bglDist;
//            }
//            else if(pIterator->pBerth->Boat.latitude + bglDist > max_lt)
//            {
//               max_lt  = pIterator->pBerth->Boat.latitude + bglDist;
//            }
//         }
//         
//      pIterator  = pIterator->pNext;
//   }
   
//   maxDiff_lg  = max_lg - min_lg + pScale->minute/pScale->pixel*50;
//   maxDiff_lt  = max_lt - min_lt + pScale->minute/pScale->pixel*50;
   
   maxDiff_lg  = max_lg - min_lg;
   maxDiff_lt  = max_lt - min_lt;
   
//   if(maxDiff_lt > 30000)
//       return ;
   
   *halfDiff_lg  = max_lg/2 + min_lg/2;
   *halfDiff_lt  = max_lt/2 + min_lt/2;
   
   
   ///map的宽为800pix，高为400pix，判断在适配上述区域时以map的宽来适配还是高来适配
   
   ///若适配区域的宽度大于高度的两倍，则以map的宽来适配
    if(( maxDiff_lg/2) > maxDiff_lt)
//    if( (maxDiff_lg*4)  >  (maxDiff_lt*7) )
    {
       pScale->minute  = ( maxDiff_lg/(8*100) + 1)*100;  ///这种写法保证所得到的scale.minute为100的整数倍
       pScale->minute  = pScale->minute /100 *105;
    }
    ///否则以map的高来适配
    else
    {
       pScale->minute  = ( maxDiff_lt/(4*100) + 1)*100;
       pScale->minute  = pScale->minute /100 *110;
    }
}




/** @brief    setView
 *  
 *  @dscrp    
 *  @input    
 *  @output
 *  @other    
 */
void setManualView(long lg, long lt, map_scale* pScale)
{  
   disp_map(lg, lt, pScale);

//   isAdsorbed  = 0;
   AdsorbedMMSI  = 0;
   if(isMntEnable)
      disp_boat(lg,lt,pScale, N_boat);  
   disp_mntBoat(lg,lt,pScale);

   draw_mothership(lg,lt, pScale);
   draw_scale(pScale, 640, 440);
}




/** @brief    setAutoView
 *  
 *  @dscrp    
 *  @input    
 *  @output
 *  @other    
 */
void setAutoView(long lg, long lt, map_scale* pNull)
{
   getMntWrapPara(&lg, &lt, &AutoScale); 

   SNAP_getPara(&__cursor.longitude, &__cursor.latitude);
   AdsorbedMMSI  = SNAP_getSnapObjMMSI();
   
   __cursor.x  = (__cursor.longitude-lg)*AutoScale.pixel/AutoScale.minute + (MAP_LEFT+MAP_RIGHT)/2;
   __cursor.y  = (lt -__cursor.latitude)*AutoScale.pixel/AutoScale.minute + (MAP_TOP+MAP_BOTTOM)/2;
   GUI_CURSOR_SetPosition(__cursor.x, __cursor.y);
   disp_map(lg, lt, &AutoScale);

//   isAdsorbed  = 0;
//   if(isMntEnable)
      disp_boat(lg, lt, &AutoScale, N_boat);
   disp_mntBoat(lg, lt, &AutoScale);  

   draw_mothership(lg,lt,&AutoScale);
   draw_scale(&AutoScale, 640,440);
}








