#include "DIALOG.h"
#include "Config.h"
#include "MainTask.h"
#include "sysConf.h"
#include "dlg.h"
#include "skinColor.h"
#include "str.h"
#include "HSD_SLIDER.h"
#include "28.h"
#include "GUI.h"
#include "pwm.h"
#include "drawThings.h"
#include "sound.h"
#include "string.h"
#include "lpc177x_8x_nvic.h"

#define ID_WINDOW_0         (GUI_ID_USER + 0x00)


#define ID_SLIDER_SKIN      (GUI_ID_USER + 0x20)
#define ID_SLIDER_BRT       (GUI_ID_USER + 0x21)
#define ID_SLIDER_VOL       (GUI_ID_USER + 0x22)
#define ID_SLIDER_UNIT      (GUI_ID_USER + 0x23)
#define ID_SLIDER_SHAPE     (GUI_ID_USER + 0x24)
#define ID_SLIDER_REVIVE    (GUI_ID_USER + 0x25)

#define ID_BUTTON_REVIVE    (GUI_ID_USER + 0x30)

#define SLD_NUM  6

extern boat mothership;
extern mapping center;

extern GUI_CONST_STORAGE GUI_BITMAP bmfish;
GUI_POINT pPoint[] = {
	{0,0},
	{25,0},
	{35,8},
	{25,16},
	{0,16}
};



static void sldListener(WM_MESSAGE * pMsg);
static void sldReviveCallback(WM_MESSAGE* pMsg);

static const SysWin_COLOR * pSkin  = &SysWinSkins[0];
static void _OnSkinChanged(WM_MESSAGE * pMsg,int val);
static void _OnBrtChanged(WM_MESSAGE * pMsg,int val);
static void _OnVolChanged(WM_MESSAGE * pMsg,int val);
static void _OnUnitChanged(WM_MESSAGE * pMsg,int val);
static void _OnShapeChanged(WM_MESSAGE * pMsg,int val);


static CONF_SYS agentConf;


static  void (* const ProcChanging[SLD_NUM-1])(WM_MESSAGE *, int)  = {
   _OnSkinChanged, 
   _OnBrtChanged,
   _OnVolChanged,
   _OnUnitChanged,
   _OnShapeChanged,

};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = 
{
 { WINDOW_CreateIndirect,  "Window",  ID_WINDOW_0,  SubWin_X,  SubWin_Y, SubWin_WIDTH, SubWin_HEIGHT,  0, 0, 0},

 { HSD_SLIDER_CreateIndirect, "sld0" ,ID_SLIDER_SKIN,   180,   Win_SysSet_txOrg,                                              120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld1", ID_SLIDER_BRT  ,  180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap),   120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld2", ID_SLIDER_VOL,    180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2, 120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld3", ID_SLIDER_UNIT,   180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3, 120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld4", ID_SLIDER_SHAPE,  180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4, 120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, "sld5", ID_SLIDER_REVIVE, 180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5, 120,  30,  0, 0, 0}
// { BUTTON_CreateIndirect,     "btn1", ID_BUTTON_REVIVE, 180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5, 120,  30,  0, 0, 0}
};


static WM_HWIN Slideres[SLD_NUM];


static void  _cbDialog(WM_MESSAGE * pMsg)
{
   WM_MESSAGE myMsg;
   

   int val;
   int sldId;
   int i;
   GUI_RECT r;
      
   switch(pMsg->MsgId)
   {   
      case USER_MSG_DIM:   
           HSD_SLIDER_Loop(Slideres[1]);
           break;
      case USER_MSG_MNT_SWT:
           
           break;
      case USER_MSG_SKIN:    
           pSkin  = &(SysWinSkins[pMsg->Data.v]);
           
           WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
           for(i=SLD_NUM; i;)
           {
              i--;
              HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
						        HSD_SLIDER_SetFocusBkColor (Slideres[i], pSkin->sldBk);		
              HSD_SLIDER_SetSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot); 
						 
              HSD_SLIDER_SetSliderColor(Slideres[i], pSkin->sldSlider);
              HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
           }
           
           break;
           
      case WM_INIT_DIALOG:  
           agentConf.Brt    = SysConf.Brt;
           agentConf.Shape  = SysConf.Shape;
           agentConf.Skin   = SysConf.Skin;
           agentConf.Unit   = SysConf.Unit;
           agentConf.Vol    = SysConf.Vol;
                 
           pSkin  = &(SysWinSkins[SysConf.Skin]);
		
           Slideres[0]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_SKIN);
           WM_SetCallback(Slideres[0], &sldListener);
           HSD_SLIDER_SetRange(Slideres[0], SKIN_Day, SKIN_Night); 
           HSD_SLIDER_SetValue(Slideres[0], SysConf.Skin);
           
           Slideres[1]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_BRT);
           WM_SetCallback(Slideres[1], &sldListener);
           HSD_SLIDER_SetNumTicks(Slideres[1], 6);
           HSD_SLIDER_SetRange(Slideres[1], 1, 6);
           HSD_SLIDER_SetValue(Slideres[1], SysConf.Brt);
           
           Slideres[2]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_VOL);
           WM_SetCallback(Slideres[2], &sldListener);
           HSD_SLIDER_SetNumTicks(Slideres[2], 6);
           HSD_SLIDER_SetRange(Slideres[2], 1, 6);
           HSD_SLIDER_SetValue(Slideres[2], SysConf.Vol);
           
           Slideres[3]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_UNIT);
           WM_SetCallback(Slideres[3], &sldListener);
           HSD_SLIDER_SetRange(Slideres[3], UNIT_nm,  UNIT_km);
           HSD_SLIDER_SetValue(Slideres[3], SysConf.Unit);
           
           Slideres[4]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_SHAPE);
           WM_SetCallback(Slideres[4], &sldListener);
           HSD_SLIDER_SetRange(Slideres[4], SHAPE_Boat,  SHAPE_Fish);
           HSD_SLIDER_SetValue(Slideres[4], SysConf.Shape);
           
           Slideres[5]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_REVIVE);
           WM_SetCallback(Slideres[5], &sldReviveCallback);
           HSD_SLIDER_SetRange(Slideres[5], 0, 4);
           HSD_SLIDER_SetValue(Slideres[5], 0);
//           
//           hBtRevive  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_REVIVE);
//           BUTTON_SetText(hBtRevive, "确定");
//           BUTTON_SetTextColor(hBtRevive, BUTTON_CI_PRESSED, GUI_BLACK);
//           BUTTON_SetBkColor(hBtRevive, BUTTON_BI_UNPRESSED, pSkin->sldSlider);
//           WM_SetCallback(hBtRevive, &myButtonListener);
           
           WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
           
           for(i=SLD_NUM; i; )
           {
              i--;
              HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
						        HSD_SLIDER_SetFocusBkColor (Slideres[i], pSkin->sldBk);		
						 
              HSD_SLIDER_SetSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot); 
						 
              HSD_SLIDER_SetSliderColor(Slideres[i], pSkin->sldSlider);
              HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
           }                           
           break;
           
           
       case USER_MSG_REPLY:
            if(pMsg->Data.v == REPLY_OK)
            {
               if(agentConf.Skin != SysConf.Skin)
               {
                  myMsg.MsgId  = USER_MSG_SKIN;
                  myMsg.Data.v  = agentConf.Skin;
                  WM_BroadcastMessage(&myMsg);
               }
               if(agentConf.Shape != SysConf.Shape)
               {
                  myMsg.hWin  = mapWin;
                  myMsg.MsgId  = USER_MSG_SHAPE;
                  myMsg.Data.v  = agentConf.Shape;
                  WM_SendMessage(myMsg.hWin, &myMsg);
               }

               memcpy(&SysConf, &agentConf, sizeof(SysConf));
               sysStore();
            }
            else 
            {
               /// Roll back
               HSD_SLIDER_SetValue(Slideres[0], SysConf.Skin);
               HSD_SLIDER_SetValue(Slideres[1], SysConf.Brt);
               HSD_SLIDER_SetValue(Slideres[2], SysConf.Vol);
               HSD_SLIDER_SetValue(Slideres[3], SysConf.Unit);
               HSD_SLIDER_SetValue(Slideres[4], SysConf.Shape);
            }
            
            WM_SetFocus(Slideres[0]);
            WM_SetFocus(menuWin);
            break;
       
       case WM_NOTIFY_PARENT:       
            switch(pMsg->Data.v)  
            {
               case WM_NOTIFICATION_VALUE_CHANGED:
                    sldId  = WM_GetId(pMsg->hWinSrc) - ID_SLIDER_SKIN;
                    if(sldId >=0  &&  sldId < SLD_NUM-1)
                    {                  
                       val  = HSD_SLIDER_GetValue(Slideres[sldId]);                     
                       ProcChanging[sldId](pMsg, val);
                    }
                    break;
            }
            break;
          
       case WM_PAINT:{
            GUI_POINT aEnlargePoints[11];      
            
            WM_GetClientRectEx(pMsg->hWin, &r);
            GUI_SetColor(pSkin->bkColor);
            GUI_FillRectEx(&r);
            GUI_SetColor(pSkin->ClientbkColor);
            r.x0 = r.x0 + 10;
            r.x1 = r.x1 - 10;
            r.y0 = r.y0 + 40;
            r.y1 = r.y1 - 40;
            GUI_FillRectEx(&r);
			 
            GUI_SetFont(&GUI_Font30);
					      	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
            GUI_SetColor(pSkin->txColor);
            GUI_DispStringAt("夜间模式:", 30,   Win_SysSet_txOrg );
            GUI_DispStringAt("亮度设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap));
            GUI_DispStringAt("音量设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2);
            GUI_DispStringAt("单位设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3);
            GUI_DispStringAt("船位设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4);
            
            GUI_DispStringAt("关闭", 130,Win_SysSet_txOrg);
            GUI_DispStringAt("开启",  310,Win_SysSet_txOrg);      
            GUI_DispStringAt("恢复出厂设置", 30, Win_SysSet_txOrg + (Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);                       
            GUI_DispStringAt("系统版本:",30, Win_SysSet_txOrg + (Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6);
            GUI_DispStringAt(VERSION, 180,Win_SysSet_txOrg + (Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6);
            
//            sprintf(pStrBuf, "%s", __TIME__);
//            GUI_DispStringAt(pStrBuf,400,Win_SysSet_txOrg + (Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);
//            sprintf(pStrBuf, "%s", __DATE__);
//            GUI_DispStringAt(pStrBuf,400,Win_SysSet_txOrg + (Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6);  
            
            GUI_SetFont (&GUI_Font32_1);
            GUI_DispStringAt("nm", 140,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3);           
            GUI_DispStringAt("km", 310,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3);
            GUI_RotatePolygon(aEnlargePoints, Points_fish, 11, -1.57);
            GUI_DrawPolygon(aEnlargePoints, 11, 330, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4+18 );				
            GUI_DrawPolygon(Points_boat, 3, 160, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4+18);             

						      GUI_SetFont(&GUI_Font24);
            GUI_DispStringAt("使用",30, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
	      					GUI_DispStringAt("选择选项及调整音量亮度数字",118, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
            GUI_SetColor(pSkin->sldSlider);
            GUI_DispStringAt("左右", 78, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
            
                        
            }break;
       default:
           WM_DefaultProc(pMsg);
           break;
   }
}


WM_HWIN _sub3WinCreate()
{
   WM_HWIN hWin;
   hWin  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
   return hWin;
}


static void sldListener(WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pInfo;
   WM_MESSAGE myMsg;
   
   
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           pInfo  = (WM_KEY_INFO*)(pMsg->Data.p);
           switch(pInfo->Key)
           {
              case GUI_KEY_PWM_INC:       
                   WM_SendMessageNoPara(subWins[3], USER_MSG_DIM);
                   break;
              case GUI_KEY_BACKSPACE:
                   if(Mem_isEqual(&SysConf, &agentConf, sizeof(SysConf)) )
                   {
                      WM_SetFocus(Slideres[0]);
                      WM_SetFocus(menuWin);                      
                   }
                   else
                   {
                      myMsg.hWin  = WM_GetClientWindow(confirmWin);
                      myMsg.hWinSrc  = subWins[3];
                      myMsg.MsgId  = USER_MSG_CHOOSE;
                      myMsg.Data.v  = SYS_SETTING;
                      WM_SendMessage(myMsg.hWin, &myMsg);
                      
                      WM_BringToTop(confirmWin);
                      WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0));                    
                   }
                   break;
                                 
              default:
                   HSD_SLIDER_Callback(pMsg);
                   break;
           }
           break;
           
      default:
           HSD_SLIDER_Callback(pMsg);
           break;
   }
}



static void _OnSkinChanged(WM_MESSAGE * pMsg,int val)
{
   int i  = 0;
	 WM_MESSAGE myMsg;
   if(agentConf.Skin  != val)
   {	 
      agentConf.Skin  = val;
      pSkin  = &(SysWinSkins[val]);
      
      WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
      for(i=0; i<SLD_NUM; i++)
      {
         HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
				     HSD_SLIDER_SetFocusBkColor(Slideres[i], pSkin->sldBk);
         HSD_SLIDER_SetSlotColor(Slideres[i],pSkin->sldSlot);
         HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
         HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot);
      }
      
      
      
	     myMsg.hWin = WM_GetClientWindow(menuWin);		
      myMsg.Data.v = val;
      myMsg.MsgId = USER_MSG_SKIN;
      WM_SendMessage (myMsg.hWin, &myMsg);
      
      myMsg.hWin  = confirmWin;
      WM_SendMessage (myMsg.hWin, &myMsg);
			
   }
}


static void _OnBrtChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentConf.Brt != val)
   {
      agentConf.Brt  = val;      
      PWM_SET(val);
   }
}

static void _OnVolChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentConf.Vol != val)
   {
      agentConf.Vol  = val;
      SND_SetVol(val);
      GUI_Delay(100);
      SND_Play(val);
   }
}


static void _OnUnitChanged(WM_MESSAGE * pMsg,int val)
{
   agentConf.Unit  = val;
}

static void _OnShapeChanged(WM_MESSAGE * pMsg,int val)
{
   agentConf.Shape  = val;
}



static void sldReviveCallback(WM_MESSAGE* pMsg)
{
   const WM_KEY_INFO* pKeyInfo;
   
   WM_MESSAGE  myMsg;
   
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           pKeyInfo  = (WM_KEY_INFO*)pMsg->Data.p;
           switch(pKeyInfo->Key)
           {
              case GUI_KEY_RIGHT:
              case GUI_KEY_ENTER:
                   myMsg.hWin      = WM_GetClientWindow(confirmWin);            
                   myMsg.hWinSrc   = pMsg->hWin;
                   myMsg.MsgId     = USER_MSG_CHOOSE;
                   myMsg.Data.v    = SYS_REVIVE;
                   WM_SendMessage(myMsg.hWin, &myMsg);
 
                   WM_BringToTop(confirmWin);
                   WM_SetFocus(WM_GetDialogItem(confirmWin, GUI_ID_BUTTON0));
                   break;
              
              case GUI_KEY_BACKSPACE:
                   if(Mem_isEqual(&SysConf, &agentConf, sizeof(SysConf)) )
                   {
                      WM_SetFocus(Slideres[0]);
                      WM_SetFocus(menuWin);                      
                   }
                   else
                   {
                      myMsg.hWin  = WM_GetClientWindow(confirmWin);
                      myMsg.hWinSrc  = subWins[3];
                      myMsg.MsgId  = USER_MSG_CHOOSE;
                      myMsg.Data.v  = SYS_SETTING;
                      WM_SendMessage(myMsg.hWin, &myMsg);
                      
                      WM_BringToTop(confirmWin);
                      WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0));                    
                   }
                   break;
                   
             default:
                   HSD_SLIDER_Callback(pMsg);
                   break;
           }
           break;
           
      case USER_MSG_REPLY:
           WM_SetFocus(Slideres[0]);
           WM_BringToTop(subWins[3]);
           if(pMsg->Data.v == REPLY_OK )
           {
              if(SysConf.Skin != DEFAULT_SKIN) 
              {
                 myMsg.MsgId  = USER_MSG_SKIN;
                 myMsg.Data.v  = DEFAULT_SKIN;
                 WM_BroadcastMessage(&myMsg);
                 agentConf.Skin  = SysConf.Skin;
              }
              
              if(SysConf.Shape != DEFAULT_SHAPE)
              {
                 myMsg.hWin  = mapWin;
                 myMsg.MsgId  = USER_MSG_SHAPE;
                 myMsg.Data.v  = DEFAULT_SHAPE;
                 WM_SendMessage(myMsg.hWin, &myMsg);
                 agentConf.Shape  = SysConf.Shape;
              } 
              sysRevive();
              
              HSD_SLIDER_SetValue(Slideres[0], SysConf.Skin);
              HSD_SLIDER_SetValue(Slideres[5], 0);
              GUI_Delay(1000);
              HSD_SLIDER_SetValue(Slideres[1], SysConf.Brt);
              HSD_SLIDER_SetValue(Slideres[5], 1);
              GUI_Delay(1000);
              
              HSD_SLIDER_SetValue(Slideres[2], SysConf.Vol);
              HSD_SLIDER_SetValue(Slideres[5], 2);
              GUI_Delay(1000);
              
              HSD_SLIDER_SetValue(Slideres[3], SysConf.Unit);
              HSD_SLIDER_SetValue(Slideres[5], 3);
              GUI_Delay(1000);
              
              HSD_SLIDER_SetValue(Slideres[4], SysConf.Shape);
              HSD_SLIDER_SetValue(Slideres[5], 4);
              GUI_Delay(1000);
              
              HSD_SLIDER_SetValue(Slideres[5], 0);
              
              NVIC_SystemReset();
              
           } 

           break;
           
    default:
           HSD_SLIDER_Callback(pMsg);
           break;
   }
}

//static void myButtonListener(WM_MESSAGE* pMsg)
//{
//   const WM_KEY_INFO* pKeyInfo;
//   WM_MESSAGE myMsg;   
//   switch(pMsg->MsgId)
//   {
//      case WM_SET_FOCUS:
//           if(pMsg->Data.v)
//           {
//              BUTTON_SetBkColor(pMsg->hWin, BUTTON_BI_UNPRESSED, pSkin->sldFocusSlider);
//           }
//           else
//           {
//              BUTTON_SetBkColor(pMsg->hWin, BUTTON_BI_UNPRESSED, pSkin->sldSlider);
//           }
//           BUTTON_Callback(pMsg);
//           break;
//      
//      case WM_KEY:        
//           pKeyInfo  = (WM_KEY_INFO*)pMsg->Data.p;
//           switch(pKeyInfo->Key)
//           {
//              case GUI_KEY_UP:             
//                   WM_SetFocusOnPrevChild(WM_GetParent(pMsg->hWin));
//                   break;                   
//              
//              case GUI_KEY_DOWN:
//                   WM_SetFocusOnNextChild(WM_GetParent(pMsg->hWin));
//                   break;                   
// 
//              case GUI_KEY_RIGHT:
//              case GUI_KEY_LEFT:                
//                   myMsg.hWin     = WM_GetClientWindow(confirmWin);
//                   myMsg.hWinSrc  = pMsg->hWin;
//                   myMsg.MsgId    = USER_MSG_CHOOSE;
//                   myMsg.Data.v   = SYS_REVIVE;
//                   WM_SendMessage(myMsg.hWin, &myMsg);
//                   
//                   WM_BringToTop(confirmWin);
//                   WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON1));     
//                   break;
//              
//              case GUI_KEY_BACKSPACE:             
//                   if(Mem_isEqual(&SysConf, &agentConf, sizeof(SysConf)) )
//                   {
//                      WM_SetFocus(Slideres[0]);
//                      WM_SetFocus(menuWin);                      
//                   }
//                   else
//                   {
//                      myMsg.hWin  = WM_GetClientWindow(confirmWin);
//                      myMsg.hWinSrc  = subWins[3];
//                      myMsg.MsgId  = USER_MSG_CHOOSE;
//                      myMsg.Data.v  = SYS_SETTING;
//                      WM_SendMessage(myMsg.hWin, &myMsg);
//                      
//                      WM_BringToTop(confirmWin);
//                      WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0));                    
//                   }
//                   break;                                     
//                          
//              default:
//                   BUTTON_Callback(pMsg);
//                   break;
//           }
//           break;
//           
//           
//      case USER_MSG_REPLY:
//           if(pMsg->Data.v == REPLY_OK) 
//           {  
//              if(SysConf.Skin != DEFAULT_SKIN) 
//              {
//                 myMsg.MsgId  = USER_MSG_SKIN;
//                 myMsg.Data.v  = DEFAULT_SKIN;
//                 WM_BroadcastMessage(&myMsg);
//              }
//              
//              if(SysConf.Shape != DEFAULT_SHAPE)
//              {
//                 myMsg.hWin  = mapWin;
//                 myMsg.MsgId  = USER_MSG_SHAPE;
//                 myMsg.Data.v  = DEFAULT_SHAPE;
//                 WM_SendMessage(myMsg.hWin, &myMsg);
//              }
//               
//              sysRevive();
//              
//              HSD_SLIDER_SetValue(Slideres[0], SysConf.Skin);
//              HSD_SLIDER_SetValue(Slideres[1], SysConf.Brt);
//              HSD_SLIDER_SetValue(Slideres[2], SysConf.Vol);
//              HSD_SLIDER_SetValue(Slideres[3], SysConf.Unit);
//              HSD_SLIDER_SetValue(Slideres[4], SysConf.Shape);
//           }
//           else
//           {
//              ;  
//           }
//           WM_SetFocus(Slideres[0]);
//           WM_SetFocus(menuWin);
//           break;     
//           
//     default:
//           BUTTON_Callback(pMsg);
//           break;
//   }
//}




































