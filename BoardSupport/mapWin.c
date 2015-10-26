
#include "Setting.h"
#include "MainTask.h"
#include "sysConf.h"
#include "mapWin.h"
#include "dlg.h"
#include "skinColor.h"
#include "str.h"
#include "drawThings.h"

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)



/*-------------------------- Global variables -----------------------------*/
WM_HWIN mapWin;




/*-------------------------- external variables ---------------------------*/
extern Bool isReleasedDet;


/*-------------------------- global variables ------------------------------*/
boat mothership;
char scale_choose = 1;

/*------------------------- local variables --------------------------------*/

const GUI_RECT Rect_MapWin[]  = {MAP_LEFT, 0,       MAP_RIGHT, MAP_BOTTOM};   /// Map Window area.
const GUI_RECT Rect_Title[]  =  {MAP_LEFT, 0,       MAP_RIGHT, MAP_TOP-1};    /// Title of mapWin area.
const GUI_RECT Rect_Map[]  =    {MAP_LEFT, MAP_TOP, MAP_RIGHT, MAP_BOTTOM};   /// Map area in mapWin.



/// Timer about
static WM_HTIMER reTimer;
static WM_HTIMER cursorTimer;

/// Cursor about
static short Dir_x  = 0;
static short Dir_y  = 0;
volatile _cursor __cursor  = {(MAP_LEFT+MAP_RIGHT)/2, (MAP_TOP+MAP_BOTTOM)/2,7300000, 1740000};
static _cursor tmp_cursor;

/// Skin color
static const MapWin_COLOR * pSkin  = mapSkins;  


static unsigned int drawMapSwitch  = 1;
static unsigned int drawMapSwitchCnt  = 0;


long temp_lat = 1920726,temp_long = 7305545;


mapping center;
boat mothership;

static void onCursorMoved();


static void _cbWindowAllFishMap(WM_MESSAGE* pMsg) 
{	
	WM_HWIN hWin = pMsg->hWin;
 
 WM_KEY_INFO * pKeyInfo  = NULL;

 WM_MESSAGE myMsg;

	short i = 0;
	
	temp_lat  = __cursor.latitude;
	temp_long  = __cursor.longitude ;

	switch (pMsg->MsgId) 
	{
		case WM_CREATE: 
       
       /// Initializaton of curosr
       GUI_CURSOR_Select(&GUI_CursorCrossS);
       
       /// Initializaton of skin .
       pSkin  = &(mapSkins[SysConf.Skin]);
       setBoatSkin(SysConf.Skin); 
       /// Initializaton of center
//       center.lgtude  = mothership.longitude;
//       center.lttude  = mothership.latitude;

       center.lgtude  = 7260000;
       center.lttude  = 2220000;
        
       /// Do not exist monited boat.
       if(!pMntHeader)
       {
          myMsg.hWin  = WM_GetClientWindow(confirmWin);
          myMsg.hWinSrc  = pMsg->hWin;
          myMsg.MsgId  = USER_MSG_CHOOSE;
          myMsg.Data.v  = ADD_MONITED;
          WM_SendMessage(myMsg.hWin, &myMsg);
          WM_BringToTop(confirmWin);
          WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0)); 
       }
       else
       {
          WM_SetFocus(pMsg->hWin);
       }
   		  reTimer  = WM_CreateTimer(pMsg->hWin, ID_TIMER_MAP_REFRESH,MAP_REFRESH_SLOT, 0);
       break;
    
  case USER_MSG_SKIN: 
       pSkin  = &(mapSkins[pMsg->Data.v]);
       setBoatSkin(pMsg->Data.v);
       break;
       
  case USER_MSG_SHAPE:
       setShape(pMsg->Data.v);
       break;
       
  case USER_MSG_REPLY:
       switch(pMsg->Data.v)
       {
        case REPLY_OK:
             GUI_CURSOR_Hide();
					        WM_BringToTop (menuWin);
					        WM_ShowWindow(subWins[0]);
             WM_ShowWindow(subWins[1]);
             WM_ShowWindow(mntSettingWin);
//             WM_ShowWindow(_mntSettingWin);
             WM_ShowWindow(subWins[2]);
             WM_ShowWindow(subWins[3]);
             WM_BringToTop(subWins[2]);
             WM_SetFocus(WM_GetDialogItem(menuWin, GUI_ID_USER + 0x03));             
             WM_SetFocus(subWins[2]);
             break;
        
        case REPLY_CANCEL:
             WM_SetFocus(pMsg->hWin);
             break;
       }
       break;  
  
		case WM_TIMER: 
  
       if(ID_TIMER_CURSOR  == WM_GetTimerId(cursorTimer))
       {        
          onCursorMoved();
          WM_RestartTimer(pMsg->Data.v, 50);
       }
       else if(ID_TIMER_MAP_REFRESH == WM_GetTimerId(reTimer))
       {		
          drawMapSwitchCnt++;
          if(drawMapSwitchCnt > AUTO_ADAPTER_CNT)
          {
             drawMapSwitchCnt  = 0;
             drawMapSwitch  = 0;
             GUI_CURSOR_Hide();
          }          
          WM_InvalidateRect( hWin,Rect_MapWin);
          WM_RestartTimer(reTimer, MAP_REFRESH_SLOT);
       }

       break;

		case WM_KEY: 
       pKeyInfo  = (WM_KEY_INFO*)(pMsg->Data.p);    
             
       drawMapSwitchCnt  = 0;
       if(drawMapSwitch == 0)
       {
          drawMapSwitch  = 1;
          WM_InvalidateRect(mapWin, Rect_Map);
//          WM_Paint(mapWin);
       }
       
     //		WM_InvalidateRect(hWin,Rect_Map);  
       if(!GUI_CURSOR_GetState())
       {
          GUI_CURSOR_Show();
       }         
    
       if(pKeyInfo->Key >= GUI_KEY_LEFT  &&  pKeyInfo->Key <= GUI_KEY_DOWN)
       {      
          switch(pKeyInfo->Key)  
          {
             case GUI_KEY_LEFT:
                  Dir_x  = -1;
                  Dir_y  = 0;
                  break;
             case GUI_KEY_UP:
                  Dir_x  = 0;
                  Dir_y  = -1;             
                  break;
             case GUI_KEY_RIGHT:
                  Dir_x  = 1;
                  Dir_y  = 0;             
                  break;
             case GUI_KEY_DOWN:
                  Dir_x  = 0;
                  Dir_y  = 1;             
                  break;
          }             
              WM_DeleteTimer(reTimer);
              cursorTimer  = WM_CreateTimer(mapWin, ID_TIMER_CURSOR, 500, 0);
            
              onCursorMoved();            
       }
       else switch(pKeyInfo->Key)
       {
         case GUI_KEY_RELEASE:               
              WM_DeleteTimer(cursorTimer);         
 	            reTimer  = WM_CreateTimer(pMsg->hWin, ID_TIMER_MAP_REFRESH,MAP_REFRESH_SLOT, 0);          
              break;
         /*----------------------   捕捉到中心按键按下的响应:    -----------------------*/
         /*   中心按键按下后：将本船位置和光标显示到map中心点 */
          
         case GUI_KEY_ENTER:         
            /* 光标定位到中心 */
              __cursor.x = (MAP_LEFT+MAP_RIGHT)/2;
              __cursor.y = (MAP_TOP+MAP_BOTTOM)/2;
           
           /* 光标代表的经纬度设置为母船的经纬度 */
              __cursor.latitude = mothership.latitude;
              __cursor.longitude = mothership.longitude;
              
              center.lgtude  = mothership.longitude;
              center.lttude  = mothership.latitude;
              
              GUI_CURSOR_Hide();
              GUI_CURSOR_SetPosition(__cursor.x, __cursor.y);
              WM_InvalidateRect(mapWin,Rect_Map);
              WM_Paint(mapWin);
              GUI_CURSOR_Show();
              break;
     
        case GUI_KEY_LARGE: 
             if(scale_choose <MAX_GEAR)
                scale_choose++;
             WM_InvalidateRect( hWin,Rect_Map);//WM_Paint(hWin);
             break;
     
        case GUI_KEY_REDUCE:   
             if(scale_choose >0)
                scale_choose--;
             WM_InvalidateRect( hWin,Rect_Map);//WM_Paint(hWin);
             break;
					
        case GUI_KEY_MENU:	         
             GUI_CURSOR_Hide();
             WM_BringToTop(menuWin);
             WM_ShowWindow(subWins[0]);
             WM_ShowWindow(subWins[1]);
             WM_ShowWindow(mntSettingWin);
             WM_ShowWindow(subWins[2]);
             WM_ShowWindow(subWins[3]);
             WM_SetFocus(menuWin);
             break;

			     }
		      break;
			
		case WM_PAINT: 
/// Draw  map grid   
  
    
    GUI_SetBkColor(pSkin->bkColor);
//    GUI_ClearRect(0,40, 800, 480);
    GUI_ClearRectEx(Rect_Map);
//    drawMapSwitch  = 1;
    if(drawMapSwitch)
    {
       setView(center.lgtude, center.lttude, &measuring_scale[scale_choose]);
    }
    else
    {
       setAutoView();
    }
   
/// Draw map title 
//    GUI_SetBkColor(pSkin->ttl_Label);   
//    GUI_ClearRect(0,0,800,40);
//    GUI_ClearRectEx(Rect_Title);
    
    GUI_DrawGradientV(MAP_LEFT,0,MAP_RIGHT,MAP_TOP-1,pSkin->ttl_bkTop,pSkin->ttl_bkBottom);
    
    GUI_SetColor(pSkin->ttl_Label);
    GUI_SetFont(&GUI_Font24B_1);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_DispStringAt("N:",  0,   5);
    GUI_DispStringAt("E:",  150, 5);
    GUI_DispStringAt("SOG:",300, 5);
    GUI_DispStringAt("COG:",480, 5);
    
    GUI_SetColor(pSkin->ttl_Context);
    
    lltostr(mothership.latitude, pStrBuf);
    GUI_DispStringAt(pStrBuf, 20, 5);
    lltostr(mothership.longitude, pStrBuf);
    GUI_DispStringAt(pStrBuf, 170, 5);
    
    if(SysConf.Unit == UNIT_km)
    {
       int sog  = mothership.SOG * 18;
       sprintf(pStrBuf, "%3d.%02dkm", sog/100, sog%100);
    }
    else
    {
       sprintf(pStrBuf, "%2d.%dkt", mothership.SOG/10,mothership.SOG%10); 
    }
      
    GUI_DispStringAt(pStrBuf, 360, 5);
    
    sprintf(pStrBuf, "%3d", mothership.COG/10);
    pStrBuf[3]  = 194;
    pStrBuf[4]  = 176;
    pStrBuf[5]  = '\0';
    GUI_DispStringAt(pStrBuf, 540, 5);

    sprintf(pStrBuf, "%02ld-%02ld/%02ld  %02ld:%02ld",(SYS_Date%10000)/100,SYS_Date/10000,  SYS_Date%100, 
                                           SYS_Time/10000+8, (SYS_Time%10000)/100);                                            
    GUI_DispStringAt(pStrBuf, 600, 5);
   
//    GUI_SetPenSize(3);
//    GUI_SetColor(pSkin->Title_HorLine);
//    GUI_DrawLine(0, MAP_TOP-2, 800, MAP_TOP-2); 

    GUI_SetPenSize(1);
    GUI_SetColor(GUI_DARKGRAY);
    GUI_AA_FillPolygon(Points_Compass_1, 3, 100, 100);
    GUI_SetColor(GUI_GRAY);
    GUI_AA_FillPolygon(Points_Compass_2, 3, 100, 100);
    GUI_SetColor(GUI_RED);
    GUI_AA_FillPolygon(Points_Compass_3, 3, 100, 100);  
    GUI_SetColor(GUI_DARKRED);
    GUI_AA_FillPolygon(Points_Compass_4, 3, 100, 100);



		break;

		default:
			 WM_DefaultProc(pMsg);
	}
}



WM_HWIN mapWinCreate()
{
   WM_HWIN handle;
   handle  = WM_CreateWindowAsChild (0, 0, 800, 480, WM_HBKWIN, WM_CF_SHOW, &_cbWindowAllFishMap, 0);
   return handle;
}



static void onCursorMoved()
{
   tmp_cursor.x  = __cursor.x + Dir_x*CURSOR_SPEED;
   tmp_cursor.y  = __cursor.y + Dir_y*CURSOR_SPEED;

   tmp_cursor.longitude  = __cursor.longitude + measuring_scale[scale_choose].minute/measuring_scale[scale_choose].pixel * Dir_x*CURSOR_SPEED;
   tmp_cursor.latitude   = __cursor.latitude - measuring_scale[scale_choose].minute/measuring_scale[scale_choose].pixel * Dir_y*CURSOR_SPEED;
   
   /// Cursor into valid area.
   if(    tmp_cursor.longitude > MAP_LEFT_LG  &&  tmp_cursor.longitude < MAP_RIGHT_LG  
      &&  tmp_cursor.latitude > MAP_BOTTOM_LT  &&  tmp_cursor.latitude < MAP_TOP_LT)
   {   
      __cursor.longitude  = tmp_cursor.longitude;
      __cursor.latitude   = tmp_cursor.latitude;  
      
      /// Page turn.
      if(   tmp_cursor.x < MAP_LEFT || tmp_cursor.x > MAP_RIGHT 
         || tmp_cursor.y < MAP_TOP ||  tmp_cursor.y > MAP_BOTTOM)
      { 
         center.lgtude  = center.lgtude + measuring_scale[scale_choose].minute/measuring_scale[scale_choose].pixel * (MAP_WIDTH/2) * Dir_x ;
         center.lttude  = center.lttude - measuring_scale[scale_choose].minute/measuring_scale[scale_choose].pixel * (MAP_HEIGHT/2) * Dir_y;
         
         __cursor.x  = (MAP_LEFT+MAP_RIGHT)/2;
         __cursor.y  = (MAP_TOP+MAP_BOTTOM)/2;
         __cursor.longitude  = center.lgtude;
         __cursor.latitude  =  center.lttude;
     
         GUI_CURSOR_Hide();
         GUI_CURSOR_SetPosition(__cursor.x, __cursor.y);
         WM_InvalidateRect(mapWin,Rect_Map);
         WM_Paint(mapWin);
         GUI_CURSOR_Show();
      }
      else
      {
         __cursor.x  = tmp_cursor.x;
         __cursor.y  = tmp_cursor.y;
         GUI_CURSOR_SetPosition(__cursor.x, __cursor.y);
      }
   }
   else   
   {
      ;
   }
}
