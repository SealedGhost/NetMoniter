/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.24                          *
*        Compiled Jan 27 2014, 11:28:24                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "Config.h"
#include "MainTask.h"
#include "SystemConfig.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0         (GUI_ID_USER + 0x00)

#define ID_TEXT_TITLE       (GUI_ID_USER + 0x10)
#define ID_TEXT_NIGHT       (GUI_ID_USER + 0x11)
#define ID_TEXT_VOL         (GUI_ID_USER + 0x12)
#define ID_TEXT_BRIGHT      (GUI_ID_USER + 0x13)
#define ID_TEXT_ARMSND      (GUI_ID_USER + 0x14)
#define ID_TEXT_KEYSND      (GUI_ID_USER + 0x15)
#define ID_TEXT_UNIT        (GUI_ID_USER + 0x16)
#define ID_TEXT_SHAPE       (GUI_ID_USER + 0x17)
#define ID_TEXT_UPDATE      (GUI_ID_USER + 0x18)
#define ID_TEXT_VERSION     (GUI_ID_USER + 0x19)
#define ID_TEXT_VER         (GUI_ID_USER + 0x1a)

#define ID_BUTTON_NIGHT       (GUI_ID_USER + 0x21)
#define ID_BUTTON_VOL         (GUI_ID_USER + 0x22)
#define ID_BUTTON_BRIGHT      (GUI_ID_USER + 0x23)
#define ID_BUTTON_ARMSND      (GUI_ID_USER + 0x24)
#define ID_BUTTON_KEYSND      (GUI_ID_USER + 0x25)
#define ID_BUTTON_UNIT        (GUI_ID_USER + 0x26)
#define ID_BUTTON_SHAPE       (GUI_ID_USER + 0x27)
#define ID_BUTTON_UPDATE      (GUI_ID_USER + 0x28)
#define ID_BUTTON_VERSION     (GUI_ID_USER + 0x29)
#define ID_BUTTON_VER         (GUI_ID_USER + 0x2a)



/*-------------------- external variables -----------------------------*/
/// Window
extern WM_HWIN menuWin;
extern WM_HWIN confirmWin;
extern WM_HWIN subWins[4];
extern WM_HWIN btWin;

/// 
extern CONF_SYS SysConf;
extern MntSetWin_COLOR MntSetWinSkins[2];


extern char * pStrBuf;

/*------------------- external func -----------------------------*/
extern  WM_HWIN messageCreate(void);



/*------------------- Local func ------------------------------*/
static void myButtonListener(WM_MESSAGE * pMsg);


/*------------------ Local variables -------------------------*/
/// Widget
WM_HWIN Buttons[9];
WM_HWIN Texts[10];

///
CONF_SYS agentConf;



// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
 { WINDOW_CreateIndirect, "Window", ID_WINDOW_0,  SubWin_X, SubWin_Y, SubWin_WIDTH, SubWin_HEIGHT, 0, 0x0, 0 },
 
 { TEXT_CreateIndirect, "系统设置", ID_TEXT_TITLE,  0,   0,                                                             120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "夜间模式", ID_TEXT_NIGHT,  0,   Win_SysSet_txOrg,                                              120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "音量设置", ID_TEXT_VOL  ,  0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap),   120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "亮度设置", ID_TEXT_BRIGHT, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "报警  音", ID_TEXT_ARMSND, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "按键  音", ID_TEXT_KEYSND, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "单位设置", ID_TEXT_UNIT,   0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "船位设置", ID_TEXT_SHAPE,  0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "软件更新", ID_TEXT_UPDATE, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "系统版本", ID_TEXT_VERSION,0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*8, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "0.0",       ID_TEXT_VER,    120, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*8, 120,  30, 0,  0, 0},
 
 { BUTTON_CreateIndirect, "夜间模式", ID_BUTTON_NIGHT,  120,   40,                                                            120,  30,  0, 0, 0},
 { BUTTON_CreateIndirect, "音量设置", ID_BUTTON_VOL  ,  120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap),   120,  30,  0, 0, 0},
 { BUTTON_CreateIndirect, "亮度设置", ID_BUTTON_BRIGHT, 120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2, 120,  30,  0, 0, 0},
 { BUTTON_CreateIndirect, "报警  音", ID_BUTTON_ARMSND, 120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3, 120,  30,  0, 0, 0},
 { BUTTON_CreateIndirect, "按键  音", ID_BUTTON_KEYSND, 120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4, 120,  30,  0, 0, 0},
 { BUTTON_CreateIndirect, "单位设置", ID_BUTTON_UNIT,   120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5, 120,  30,  0, 0, 0},
 { BUTTON_CreateIndirect, "船位设置", ID_BUTTON_SHAPE,  120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6, 120,  30,  0, 0, 0},
 { BUTTON_CreateIndirect, "软件更新", ID_BUTTON_UPDATE, 120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7, 120,  30,  0, 0, 0}
// { BUTTON_CreateIndirect, "系统版本", ID_BUTTON_VERSION,120,   280,  120,  30,  0, 0, 0}
  // USER START (Optionally insert additional widgets)
  // USER END
};




/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	
	const WM_KEY_INFO* pInfo;
 WM_MESSAGE myMsg;
 
  WM_HWIN hItem;
	int     xSize;
  int     ySize;
  int     NCode;
  int     Id;
	char    i;
  switch (pMsg->MsgId) 
  {		
  case WM_SET_FOCUS: 
       if(pMsg->Data.v)  
       {
          myMsg.hWin  = menuWin;
          myMsg.hWinSrc  = pMsg->hWin;
          myMsg.MsgId  = USER_MSG_BRING;
          myMsg.Data.v  = 3;
          WM_SendMessage(myMsg.hWin, &myMsg);  
       }
       WINDOW_Callback(pMsg);     
       break;
		case USER_MSG_SKIN:
       pMntSetWinSkin  = &(MntSetWinSkins[pMsg->Data.v]);
       
       WINDOW_SetBkColor(pMsg->hWin, pMntSetWinSkin->MntSetWin_BackGround);
       
       for(i=0;i<10;i++)
       {
          TEXT_SetTextColor(Texts[i], pMntSetWinSkin->MntSetWin_Label);
          TEXT_SetBkColor(Texts[i], pMntSetWinSkin->MntSetWin_bkUnpressed);
       }

       for(i=0;i<8;i++)       
       {
          BUTTON_SetBkColor(Buttons[i], BUTTON_CI_UNPRESSED, pMntSetWinSkin->MntSetWin_bkUnpressed);
          BUTTON_SetTextColor(Buttons[i], BUTTON_CI_UNPRESSED, pMntSetWinSkin->MntSetWin_Text);          
       }
       break;
			
  case WM_INIT_DIALOG:
       pMntSetWinSkin  = &(MntSetWinSkins[SysConf.Skin]);
//     //
//     // Initialization of 'Button'
//     //
     
     Buttons[0]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_NIGHT);
     if(SysConf.Skin == SKIN_Night)
        BUTTON_SetText(Buttons[0], "关闭");
     else
        BUTTON_SetText(Buttons[0], "开启");
     
 
     Buttons[1]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_VOL);
     BUTTON_SetFont(Buttons[1], &GUI_Font24_1);   
     sprintf(pStrBuf, "%d", SysConf.Snd.Vol);
     BUTTON_SetText(Buttons[1], pStrBuf); 
  
   
   
     Buttons[2]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BRIGHT);
     BUTTON_SetFont(Buttons[2], &GUI_Font24_1);    
     sprintf(pStrBuf, "%d", SysConf.Brt);
     BUTTON_SetText(Buttons[2], pStrBuf); 


     Buttons[3]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_ARMSND);
     BUTTON_SetFont(Buttons[3], &GUI_Font24_1);        
     sprintf(pStrBuf, "%d", SysConf.Snd.ArmSnd);
     BUTTON_SetText(Buttons[3], pStrBuf);   


     Buttons[4]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_KEYSND);
     BUTTON_SetFont(Buttons[4], &GUI_Font24_1);    
     sprintf(pStrBuf, "%d", SysConf.Snd.KeySnd);
     BUTTON_SetText(Buttons[4], pStrBuf);


     Buttons[5]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_UNIT);
     BUTTON_SetFont(Buttons[5], &GUI_Font24_1);         
     if(SysConf.Unit == UNIT_nm)
        BUTTON_SetText(Buttons[5], "nm");      
     else 
        BUTTON_SetText(Buttons[5], "km");


     Buttons[6]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_SHAPE);
     BUTTON_SetFont(Buttons[6], &GUI_Font24_1);      
     if(SysConf.Shape == SHAPE_Fish)
        BUTTON_SetText(Buttons[6], "fish");
     else 
        BUTTON_SetText(Buttons[6], "boat");


     Buttons[7]  = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_UPDATE);
     BUTTON_SetFont(Buttons[7], &GUI_Font24_1);            
     BUTTON_SetText(Buttons[7], "OK");

///  Initialization of skin.
     WINDOW_SetBkColor(pMsg->hWin, pMntSetWinSkin->MntSetWin_BackGround);
     
     for(i=0;i<10;i++)
    {
       Texts[i]  = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE+i+1);
       TEXT_SetTextColor(Texts[i], pMntSetWinSkin->MntSetWin_Label);
       TEXT_SetBkColor(Texts[i], pMntSetWinSkin->MntSetWin_bkUnpressed);
    }
    
    TEXT_SetText(Texts[9], GUI_GetVersionString());
     
     for(i=0;i<8;i++)       
     {
        BUTTON_SetBkColor(Buttons[i], BUTTON_CI_UNPRESSED, pMntSetWinSkin->MntSetWin_bkUnpressed);
        BUTTON_SetTextColor(Buttons[i], 0, pMntSetWinSkin->MntSetWin_Text);
        WM_SetCallback(Buttons[i], &myButtonListener);
     }
     
     agentConf.Skin              = SysConf.Skin;
     agentConf.Brt               = SysConf.Brt;
     agentConf.Snd.Vol           = SysConf.Snd.Vol;
     agentConf.Snd.ArmSnd        = SysConf.Snd.ArmSnd;
     agentConf.Snd.KeySnd        = SysConf.Snd.KeySnd;
     agentConf.Unit              = SysConf.Unit;
     agentConf.Shape             = SysConf.Shape;
     break;
     
  case USER_MSG_ID_REPLY:
       if(pMsg->Data.v == REPLY_OK)
       { 
          /// Notify all window to change skin & change system config.
          if(agentConf.Skin != SysConf.Skin)
          {
//             myMsg.hWin   = WM_GetClientWindow(etWin);
             myMsg.MsgId  = USER_MSG_SKIN;
             myMsg.Data.v  = agentConf.Skin;
//             WM_SendMessage(myMsg.hWin, &myMsg);
             WM_BroadcastMessage(&myMsg); 
          }  
          
          if(agentConf.Shape != SysConf.Shape)
          {
             myMsg.hWin  = hDlg_FishMap;
             myMsg.MsgId  = USER_MSG_SHAPE;
             myMsg.Data.v = agentConf.Shape;
             WM_SendMessage(myMsg.hWin, &myMsg);
          }  

          if(agentConf.Unit != SysConf.Unit)          
          {
             myMsg.MsgId  = USER_MSG_UNIT;
             myMsg.Data.v  = agentConf.Unit;
             WM_SendMessage(hDlg_FishMap, &myMsg);
             WM_SendMessage(btWin, &myMsg);
          }
             SysConf.Brt  = agentConf.Brt;
             SysConf.Shape  = agentConf.Shape;
             SysConf.Skin   = agentConf.Skin;
             SysConf.Snd.Vol  = agentConf.Snd.Vol;
             SysConf.Snd.ArmSnd  = agentConf.Snd.ArmSnd;
             SysConf.Snd.KeySnd  = agentConf.Snd.KeySnd;
             SysConf.Unit        = agentConf.Unit;
             
             sysStore(); 
                   
       }
       else
       { 
          /// Roll back;
          agentConf.Skin  = SysConf.Skin;
          agentConf.Brt   = SysConf.Brt;
          agentConf.Snd.Vol  = SysConf.Snd.Vol;
          agentConf.Snd.ArmSnd  = SysConf.Snd.ArmSnd;
          agentConf.Snd.KeySnd  = SysConf.Snd.KeySnd;
          agentConf.Unit        = SysConf.Unit;
          agentConf.Shape       = SysConf.Shape;    
          
          if(SysConf.Skin == SKIN_Night)
             BUTTON_SetText(Buttons[0], "关闭");
          else
             BUTTON_SetText(Buttons[0], "开启");
       
          sprintf(pStrBuf, "%d", SysConf.Snd.Vol);
          BUTTON_SetText(Buttons[1], pStrBuf); 
        
          sprintf(pStrBuf, "%d", SysConf.Brt);
          BUTTON_SetText(Buttons[2], pStrBuf); 
            
          sprintf(pStrBuf, "%d", SysConf.Snd.ArmSnd);
          BUTTON_SetText(Buttons[3], pStrBuf);   
        
          sprintf(pStrBuf, "%d", SysConf.Snd.KeySnd);
          BUTTON_SetText(Buttons[4], pStrBuf);
              
          if(SysConf.Unit == UNIT_nm)
             BUTTON_SetText(Buttons[5], "nm");      
          else 
             BUTTON_SetText(Buttons[5], "km");
         
          if(SysConf.Shape == SHAPE_Fish)
             BUTTON_SetText(Buttons[6], "fish");
          else 
             BUTTON_SetText(Buttons[6], "boat");          
          
          myMsg.Data.v  = SysConf.Skin;
          myMsg.MsgId  = USER_MSG_SKIN;          
          WM_SendMessage(pMsg->hWin, &myMsg);
       }      
       
       WM_SetFocus(menuWin);
       break;
  
  default:
    WM_DefaultProc(pMsg);
    break;		
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END

 
  // USER START (Optionally insert additional message handling)
  // USER END

  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN sub3WinCreate(void);
WM_HWIN sub3WinCreate(void) {
  WM_HWIN hWin;
	
//  hWin  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, menuWin, 0, 0);
  hWin  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);

  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END


static void myButtonListener(WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pInfo;
   int Id  = 0;
   int Step  = 0;
   int index  = 0;
   WM_MESSAGE myMsg;
   
   WM_HWIN nextButton;

   WM_HWIN thisButton  = pMsg->hWin; 
   WM_HWIN focussedButton  = 0;

   switch(pMsg->MsgId)   
   {  
      case WM_SET_FOCUS:
           index  = WM_GetId(pMsg->hWin) - ID_BUTTON_NIGHT;
           
           if(pMsg->Data.v == 0)
           {
              BUTTON_SetBkColor(Buttons[index], BUTTON_CI_UNPRESSED, pMntSetWinSkin->MntSetWin_bkUnpressed);
              TEXT_SetBkColor(Texts[index], pMntSetWinSkin->MntSetWin_bkUnpressed);
           }
           else
           {
             BUTTON_SetBkColor(Buttons[index], BUTTON_CI_UNPRESSED, GUI_DARKMAGENTA);
             TEXT_SetBkColor(Texts[index], GUI_DARKMAGENTA); 
           }
           
           BUTTON_Callback(pMsg);
           break;
      case WM_KEY:
           pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
           
           switch(pInfo->Key)
           {
              case GUI_KEY_UP:       
                   nextButton  = WM_SetFocusOnPrevChild(WM_GetParent(pMsg->hWin)); 
                   break;
              case GUI_KEY_DOWN:
                   nextButton  = WM_SetFocusOnNextChild(WM_GetParent(pMsg->hWin)); 
                   break;
              case GUI_KEY_BACKSPACE:
                   myMsg.hWin  = WM_GetClientWindow(confirmWin);
                   myMsg.hWinSrc  = subWins[3];
                   myMsg.MsgId  = USER_MSG_ID_CHOOSE;
                   myMsg.Data.v  = SYS_SETTING;
                   WM_SendMessage(myMsg.hWin, &myMsg);
                   
                   
                   WM_BringToTop(confirmWin);
                   WM_SetFocus(confirmWin);
                   break;
              case GUI_KEY_RIGHT:
                   Step  = 1;
              case GUI_KEY_LEFT:    
                   Step  = Step>0?1:-1;
                   
                   Id  = WM_GetId(pMsg->hWin);
                   switch(Id)
                   {
                      case ID_BUTTON_NIGHT:

                           myMsg.MsgId  = USER_MSG_SKIN;
                           
                           if(agentConf.Skin == SKIN_Night)
                           { 
                              agentConf.Skin  = SKIN_Day;
                              BUTTON_SetText(thisButton,"开启");
                              myMsg.Data.v  = SKIN_Day;
INFO("skin night->day");                              
                           }
                           else
                           {
                              agentConf.Skin  = SKIN_Night;
                              BUTTON_SetText(thisButton, "关闭"); 
                              myMsg.Data.v  = SKIN_Night;                              
INFO("skin day->night");                              
                           }
                           WM_SendToParent(thisButton, &myMsg);

                           index  = Id - ID_BUTTON_NIGHT;
                           TEXT_SetBkColor(Texts[index], GUI_WHITE);
                           BUTTON_SetBkColor(Buttons[index], BUTTON_CI_UNPRESSED, GUI_WHITE);
                           break;
                      case ID_BUTTON_VOL:
                           agentConf.Snd.Vol  += Step;
INFO("VOL: %d",agentConf.Snd.Vol);                            
                           agentConf.Snd.Vol   = (agentConf.Snd.Vol+7)%7; 
                           sprintf(pStrBuf, "%d", agentConf.Snd.Vol);
                           BUTTON_SetText(thisButton, pStrBuf);
                           break;
                      case ID_BUTTON_BRIGHT:
                           agentConf.Brt  += Step;
INFO("Brt: %d", agentConf.Brt);                           
                           agentConf.Brt  = (agentConf.Brt+5)%5;                           
                           sprintf(pStrBuf, "%d", agentConf.Brt);
                           BUTTON_SetText(thisButton, pStrBuf);
                           break;
                      case ID_BUTTON_ARMSND:
//                           agentConf.Snd.ArmSnd  += Step;
INFO("ArmSnd: %d", agentConf.Snd.ArmSnd);                            
                           agentConf.Snd.ArmSnd  = (agentConf.Snd.ArmSnd&0x01) +1;                          
                           sprintf(pStrBuf, "%d", agentConf.Snd.ArmSnd);
                           BUTTON_SetText(thisButton, pStrBuf);
                           break;
                      case ID_BUTTON_KEYSND:
                           agentConf.Snd.KeySnd  += Step;
INFO("KeySnd: %d", agentConf.Snd.KeySnd);                           
                           agentConf.Snd.KeySnd  = (agentConf.Snd.KeySnd+3)%3;                           
                           sprintf(pStrBuf, "%d", agentConf.Snd.KeySnd);
                           BUTTON_SetText(thisButton, pStrBuf);
                           break;
                      case ID_BUTTON_UNIT:
                           if(agentConf.Unit == UNIT_nm)
                           {
                              agentConf.Unit  = UNIT_km;
                              BUTTON_SetText(thisButton, "km");
                           }
                           else if(agentConf.Unit == UNIT_km)
                           {
                              agentConf.Unit  = UNIT_nm;
                              BUTTON_SetText(thisButton, "nm");
                           }
                           break;
                      case ID_BUTTON_SHAPE:
                           if(agentConf.Shape == SHAPE_Boat)
                           {
                              agentConf.Shape  = SHAPE_Fish;
                              BUTTON_SetText(thisButton, "Fish");
                           }
                           else
                           {
                              agentConf.Shape  = SHAPE_Boat;
                              BUTTON_SetText(thisButton, "Boat ");
                           }                      
                           break;
                      case ID_BUTTON_UPDATE:
                           break;
                      default:
                           break;
                   }
                   break;
              
              
              default:
                   BUTTON_Callback(pMsg);
                   break;
           }
           break; 

                      
           
      default:
           BUTTON_Callback(pMsg);
           break;
   }
}


static void myButtonAddKeyListener(BUTTON_Handle hItem, int i)
{
   ;
}

/*************************** End of file ****************************/
