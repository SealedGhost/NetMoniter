
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

#include "MainTask.h"
#include "DIALOG.h"
#include "Config.h"
#include "WM.h"
#include "sysConf.h"
#include "dlg.h"
#include "skinColor.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_BUTTON_2 (GUI_ID_USER + 0x03)
#define ID_BUTTON_3 (GUI_ID_USER + 0x04)
#define ID_TEXT_0   (GUI_ID_USER + 0x05)


/*---------------------------- Global variables -------------------------------*/
WM_HWIN menuWin;
WM_HWIN subWins[4];


static const MenuWin_COLOR * pSkin  = &menuWinSkins[0];


/*********************************************************************
*
*       Static data
*
**********************************************************************/
static WM_HWIN hButtons[4];


static void myButtonListener(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, MenuLabel_WIDTH, 480, 0, 0x0, 0 },
  
  { TEXT_CreateIndirect,   "MainMenu", ID_TEXT_0, 0, 0, MenuLabel_WIDTH, 40, 0, 0, 0},
  
  { BUTTON_CreateIndirect, "bt_0", ID_BUTTON_0, MenuLabel_X, MenuLabel_Y,                    MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "bt_1", ID_BUTTON_1, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT,  MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "bt_2", ID_BUTTON_2, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT*2,MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "bt_3", ID_BUTTON_3, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT*3,MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
};






static int btIndex  = 0;

/*********************************************************************
*
*       Static code
*
**********************************************************************/

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	const WM_KEY_INFO * pInfo;
  int     NCode;
  int     Id;
 	char     i;
  
  switch (pMsg->MsgId) { 
    case USER_MSG_BRING:
         BUTTON_SetBkColor(hButtons[pMsg->Data.v], BUTTON_CI_UNPRESSED, pSkin->Bt_Sel);
         break;    
    case USER_MSG_SKIN: 
         pSkin  = &(menuWinSkins[pMsg->Data.v]);
         
         WINDOW_SetBkColor(pMsg->hWin,pSkin->BackGround);         
         
         TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), pSkin->Menu_Label);
         
         for(i=0;i<4;i++)
         {
            BUTTON_SetBkColor(hButtons[i],  BUTTON_CI_PRESSED,  pSkin->Bt_Sel);
            BUTTON_SetBkColor(hButtons[i]  ,BUTTON_CI_UNPRESSED,pSkin->Bt_Unsel);
            BUTTON_SetTextColor(hButtons[i],BUTTON_CI_UNPRESSED,pSkin->Bt_Text);             
         }

  case WM_INIT_DIALOG:
		 
    //
    // Initialization of 'text'
    //
    TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "主菜单");
    
   
    //
    // Initialization of 'bt_0'
    //
    hButtons[0] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hButtons[0], "监控列表");
    //
    // Initialization of 'bt_1'
    //
    hButtons[1] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hButtons[1], "监控设置");
    //
    // Initialization of 'bt_2'
    //
    hButtons[2] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetText(hButtons[2], "船舶列表");
    //
    // Initialization of 'bt_3'
    //
    hButtons[3] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetText(hButtons[3], "系统设置");	
		
  
    WINDOW_SetBkColor(pMsg->hWin,pSkin->BackGround);  
    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), pSkin->Menu_Label);
    
		  for (i = 0; i < 4; i++)
	  	{		
       WIDGET_SetEffect(hButtons[i],&WIDGET_Effect_Simple);
       BUTTON_SetBkColor(hButtons[i],  BUTTON_CI_PRESSED,  pSkin->Bt_Sel);
       BUTTON_SetBkColor(hButtons[i]  ,BUTTON_CI_UNPRESSED,pSkin->Bt_Unsel);
       BUTTON_SetTextColor(hButtons[i],BUTTON_CI_UNPRESSED,pSkin->Bt_Text); 
       BUTTON_SetFocusColor(hButtons[i], pSkin->Bt_Sel);
       WM_SetCallback(hButtons[i],&myButtonListener);
		  }
    break;
  case WM_NOTIFY_PARENT:
		
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'bt_0'
				
      switch(NCode) {     

      case WM_NOTIFICATION_CLICKED:
			
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'bt_1'
		
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'bt_2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'bt_3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;

    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
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
WM_HWIN menuWinCreate(void) {
  WM_HWIN hWin;
	
	
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
  return hWin;
}

// USER START (Optionally insert additional public code)

static void myButtonListener(WM_MESSAGE * pMsg)
{
 static 
	const WM_KEY_INFO * pInfo;
	WM_HWIN thisButton  = pMsg->hWin;
	WM_HWIN handle;
	
	
	switch(pMsg->MsgId)
	{
 
 case WM_SET_FOCUS:
      if(pMsg->Data.v)
      {   
         for(btIndex=0;btIndex<4;btIndex++)
         {
            if(pMsg->hWin == hButtons[btIndex])
               break;
         }
         if(btIndex<4)
         {
            WM_BringToTop(subWins[btIndex]);
            if(btIndex == 1)
            {
               WM_BringToTop(mntSettingWin);
//               WM_BringToTop(_mntSettingWin);
            }
            if(btIndex<2)
            {
               WM_SendMessageNoPara(subWins[btIndex],  USER_MSG_BRING);
            }
         }
         
         BUTTON_SetBkColor(thisButton, BUTTON_CI_UNPRESSED, pSkin->Bt_Sel);
      }
      else
      {
         BUTTON_SetBkColor(thisButton, BUTTON_CI_UNPRESSED, pSkin->Bt_Unsel);
      }
      BUTTON_Callback(pMsg);
      break;

		case WM_KEY:
			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
		  switch(pInfo->Key)
			{
				case GUI_KEY_DOWN:    
//         BUTTON_SetBkColor(thisButton, BUTTON_CI_UNPRESSED, pSkin->Bt_Unsel);
         GUI_StoreKeyMsg(GUI_KEY_TAB,1);		
				
					break;
				
				case GUI_KEY_UP:         
				    	GUI_StoreKeyMsg(GUI_KEY_BACKTAB,1);
					break;
				
				case GUI_KEY_RIGHT:
          WM_SetFocus(subWins[btIndex]);
         
				     break;
				case GUI_KEY_MENU:       
    case GUI_KEY_BACKSPACE:
							WM_BringToBottom(menuWin);
							WM_HideWindow(subWins[0]);
							WM_HideWindow(subWins[1]);
							WM_HideWindow(subWins[2]);
							WM_HideWindow(subWins[3]);
							WM_HideWindow(mntSettingWin);     
//       WM_HideWindow(_mntSettingWin);
							WM_SetFocus(mapWin);
       GUI_CURSOR_Show();
       
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

// USER END

/*************************** End of file ****************************/
