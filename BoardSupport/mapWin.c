
#include "Setting.h"
#include "MainTask.h"
#include "sysConf.h"
#include "mapWin.h"
#include "dlg.h"
#include "skinColor.h"
#include "str.h"
#include "drawThings.h"
#include "30.h"
#include "snap.h"

extern GUI_CONST_STORAGE GUI_BITMAP bmgg;

#define ID_WINDOW_0    (GUI_ID_USER + 0x00)


/*-------------------------- Global variables -----------------------------*/
WM_HWIN mapWin;

char scale_choose = 1;


static void (*pfnSetView)(long, long, map_scale*)  = setAutoView;

FunctionalState isMntEnable  = ENABLE;


/*-------------------------- external variables ---------------------------*/
extern Bool isReleasedDet;
extern boat mothership;
extern map_scale AutoScale;

extern Bool isMute;

extern GUI_MEMDEV_Handle hMute;

/*------------------------- local variables --------------------------------*/

const GUI_RECT Rect_MapWin  = {MAP_LEFT, 0,       MAP_RIGHT, MAP_BOTTOM};   /// Map Window area.
const GUI_RECT Rect_Title  =  {MAP_LEFT, 0,       MAP_RIGHT, MAP_TOP-1};    /// Title of mapWin area.
const GUI_RECT Rect_Map  =    {MAP_LEFT, MAP_TOP, MAP_RIGHT, MAP_BOTTOM};   /// Map area in mapWin.
GUI_RECT Rect_WrapMntSwitcher  = {MAP_LEFT+20, MAP_BOTTOM-110, MAP_LEFT+20+80, MAP_BOTTOM-30};


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

static unsigned int drawMapSwitchCnt  = 0;


static MNT_BERTH* pCurPlayBerth  = NULL;

long temp_lat = 1920726,temp_long = 7305545;


extern mapping center;


static void onCursorMoved(void);
static void zoom(void);


static void _cbWindowAllFishMap(WM_MESSAGE* pMsg) 
{	
	WM_HWIN hWin = pMsg->hWin;
 
 WM_KEY_INFO * pKeyInfo  = NULL;

 WM_MESSAGE myMsg;

	
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
       setShape(SysConf.Shape);
       /// Initializaton of center
//       center.lgtude  = mothership.longitude;
//       center.lttude  = mothership.latitude;

       center.lgtude  = 7260000;
       center.lttude  = 2220000;
        
       /// Do not exist monited boat.
       if(!pMntHeader)
       {
          GUI_CURSOR_Hide();
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
printf("setShape:%d\n\r", pMsg->Data.v);       
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
             GUI_CURSOR_Show();
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
          if(drawMapSwitchCnt > CURSOR_HIDE_CNT)
          {
             drawMapSwitchCnt  = 0;
             pfnSetView  = setAutoView;
             SNAP_reset();
             GUI_CURSOR_Hide();
          }          
          WM_InvalidateRect( hWin,&Rect_MapWin);
          WM_RestartTimer(reTimer, MAP_REFRESH_SLOT);
       }

       break;

		case WM_KEY: 
       pKeyInfo  = (WM_KEY_INFO*)(pMsg->Data.p);                 
       drawMapSwitchCnt  = 0;
//       if(drawMapSwitch == 0)
//       {
//          drawMapSwitch  = 1;
//          pfnSetView  = setManualView;
//          WM_InvalidateRect(mapWin, &Rect_Map);
////          WM_Paint(mapWin);
//       }
       
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
          if(pfnSetView == setManualView)          
          {
              WM_DeleteTimer(reTimer);
              cursorTimer  = WM_CreateTimer(mapWin, ID_TIMER_CURSOR, 500, 0);           
              onCursorMoved();            
          }
          else
          {
             SNAP_searchNearestObj(Dir_x, Dir_y);
          }
       }
       else switch(pKeyInfo->Key)
       {
         case GUI_KEY_RELEASE:               
              WM_DeleteTimer(cursorTimer);                
 	            reTimer  = WM_CreateTimer(pMsg->hWin, ID_TIMER_MAP_REFRESH,MAP_REFRESH_SLOT, 0);                       
              break;

         case GUI_KEY_CENTER:
              if(pfnSetView == setManualView)
                 pfnSetView  = setAutoView;
              else
                 pfnSetView  = setManualView;
              WM_InvalidateRect(mapWin,&Rect_Map);
              WM_Paint(mapWin);
              break; 
         /*----------------------   捕捉到中心按键按下的响应:    -----------------------*/
         /*   确定按键按下后：将本船位置和光标显示到map中心点 */
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
              WM_InvalidateRect(mapWin,&Rect_Map);
              WM_Paint(mapWin);
              GUI_CURSOR_Show();
              break;
     
        case GUI_KEY_LARGE: 
             if(pfnSetView == setAutoView)
             {
                int i  = 0;
                
                pfnSetView  = setManualView;
                
                for(; i<MAX_GEAR; i++)
                {
                   if(measuring_scale[i].minute/measuring_scale[i].pixel < AutoScale.minute/AutoScale.pixel)
                   {
                       break;  
                   }
                }
                scale_choose  = i;
                zoom();
                break;
             }
                
             if(scale_choose <MAX_GEAR)
                scale_choose++;
             zoom();
             break;
     
        case GUI_KEY_REDUCE:   
             if(pfnSetView == setAutoView)
             {
                int i  = 0;
                
                pfnSetView  = setManualView;
                
                for(i=MAX_GEAR; i; i--)
                {
                   if(measuring_scale[i].minute/measuring_scale[i].pixel > AutoScale.minute / AutoScale.pixel)
                   {
                      break;
                   }
                }
                scale_choose  = i;
                zoom();
                break;
             }
             if(scale_choose >0)
                scale_choose--;
             zoom();
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
        case GUI_KEY_PWM_INC:       
             WM_SendMessageNoPara(subWins[3], USER_MSG_DIM);
             break;
        case GUI_KEY_MONITORING:
             isMntEnable  = ENABLE;  
             isMute  = FALSE;
             MNT_Disable();
             MNT_Enable();             
             break;
             
        case GUI_KEY_CANCEL:       
             isMntEnable  = DISABLE;
             isMute  = TRUE;
//             MNT_Disable();
             break;             
			     }
		      break;
			
		case WM_PAINT: 
/// Draw  map grid   
    GUI_SetBkColor(pSkin->bkColor);
    GUI_ClearRectEx(&Rect_Map);

    (*pfnSetView)(center.lgtude, center.lttude, &measuring_scale[scale_choose]);
         
    pCurPlayBerth  = MNT_fetchPlayBerth();
    if( (pCurPlayBerth->trgState&0x0f) == MNTState_Triggered)
    {
       GUI_SetColor(GUI_RED);
       if(pCurPlayBerth->nickName[0] >= '0'  &&  pCurPlayBerth->nickName[0] <= '9')
          drawNum(pCurPlayBerth->nickName[0]-'0', 280, 50, 6);
       if(pCurPlayBerth->nickName[1] >= '0'  &&  pCurPlayBerth->nickName[1] <= '9')
          drawNum(pCurPlayBerth->nickName[1]-'0', 420, 50, 6);         
    }
    
//    if(isMntEnable == DISABLE)
    if(isMute == TRUE)
    {   
        GUI_SetColor(GUI_RED);
        GUI_DrawBitmap(&bmgg, 30, 360);
    }
    
    GUI_DrawGradientV(MAP_LEFT,0,MAP_RIGHT,MAP_TOP-1,pSkin->ttl_bkTop,pSkin->ttl_bkBottom);
    
    GUI_SetColor(pSkin->ttl_Label);
    GUI_SetFont(&GUI_Font24B_1);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_DispStringAt("N:",  0,   5);
    GUI_DispStringAt("E:",  150, 5);
    GUI_SetFont(&GUI_Font30);
    GUI_DispStringAt("航速:",300, 0);
    GUI_DispStringAt("航向:",480, 0);
    
    GUI_SetColor(pSkin->ttl_Context);
    GUI_SetFont(&GUI_Font24B_1);
    
    lltostr(mothership.latitude, pStrBuf);
    GUI_DispStringAt(pStrBuf, 20, 5);
    lltostr(mothership.longitude, pStrBuf);
    GUI_DispStringAt(pStrBuf, 170, 5);
    
//    if(SysConf.Unit == UNIT_km)
//    {
//       int sog  = mothership.SOG * 18;
//       sprintf(pStrBuf, "%3d.%02dkm", sog/100, sog%100);
//    }
//    else
//    {
//       sprintf(pStrBuf, "%2d.%dkt", mothership.SOG/10,mothership.SOG%10); 
//    }
    sprintf(pStrBuf, "%2d.%dkt", mothership.SOG/10, mothership.SOG%10);      
    GUI_DispStringAt(pStrBuf, 360, 5);
    
    sprintf(pStrBuf, "%3d", mothership.COG/10);
    pStrBuf[3]  = 194;
    pStrBuf[4]  = 176;
    pStrBuf[5]  = '\0';
    GUI_DispStringAt(pStrBuf, 540, 5);

    sprintf(pStrBuf, "%02ld/%02ld",(SYS_Date%10000)/100,SYS_Date/10000);  
    GUI_DispStringAt(pStrBuf, 650, 5);
    
    sprintf(pStrBuf, "%02ld:%02ld", SYS_Time/10000+8, SYS_Time%10000/100);
    GUI_DispStringAt(pStrBuf, 720, 5);

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
       break;
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
//         center.lgtude  = center.lgtude + measuring_scale[scale_choose].minute/measuring_scale[scale_choose].pixel * (MAP_WIDTH/2) * Dir_x ;
//         center.lttude  = center.lttude - measuring_scale[scale_choose].minute/measuring_scale[scale_choose].pixel * (MAP_HEIGHT/2) * Dir_y;       
         center.lgtude  = tmp_cursor.longitude;
         center.lttude  = tmp_cursor.latitude;
         
         __cursor.x  = (MAP_LEFT+MAP_RIGHT) /2;
         __cursor.y  = (MAP_TOP+MAP_BOTTOM) /2;
        
         __cursor.longitude  = center.lgtude;
         __cursor.latitude  =  center.lttude;
     
         GUI_CURSOR_Hide();
         GUI_CURSOR_SetPosition(__cursor.x, __cursor.y);
         WM_InvalidateRect(mapWin,&Rect_Map);
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



static void zoom(void)
{
   center.lgtude  = __cursor.longitude;
   center.lttude  = __cursor.latitude;
   
   __cursor.x  = (MAP_LEFT+MAP_RIGHT) /2;
   __cursor.y  = (MAP_TOP+MAP_BOTTOM) /2;
   
   WM_InvalidateRect(mapWin, &Rect_Map);
   GUI_CURSOR_SetPosition(__cursor.x,  __cursor.y);
   WM_Paint(mapWin);
}





