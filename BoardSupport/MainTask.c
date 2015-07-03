#include "GUI.h"
#include "FRAMEWIN.h"
#include <stddef.h>
#include "DIALOG.h"
#include "MainTask.h"
//#include "GUIDEMO.h"
#include "exfuns.h"

 GUI_RECT lvRect  = {7, 106, 320, 369};
 GUI_RECT Rect = {0,0,800,480};
 
WM_HWIN		hDlg_FishMap;
WM_HWIN 	hDlg_MonitorMap;
WM_HWIN 	hDlg_AllList;
WM_HWIN 	hDlg_MonitorList;
WM_HWIN 	hDlg_SysSet;
WM_HWIN   hDlg_MonitSet;
WM_HWIN 	hDlg_Messagebox;
WM_HWIN   hDlg_Menu;

WM_HWIN   _hLastFrame;

char dropdown0flag = 0;
int downlistfocus = 384;
char SysSetOpenflag = 0;
char Doubleclick = 0;
char MonitSetOpenflag = 0;
char Messageopenflag = 0;
char IsChange = 0;
int menufocus = 368;
char focuschange = 0;
int sysitem_ID = 0;
char	editchange = 0; 
char editcolor = 0;
char focus_monitset = 0;
static const char *Dropdownmonth[] = {"1","2","3","4","5","6","7"};

WM_HWIN _CreateFrame(WM_CALLBACK* cb) 
{
	_hLastFrame = WM_CreateWindowAsChild(122, 0, 678, 480, WM_HBKWIN, WM_CF_SHOW, cb, 0);
	return _hLastFrame;
}
// edit 按键回调
void _cbEditAddKey(EDIT_Handle hObj, int Key) {
		int i;
	 switch (Key) {
		 case GUI_KEY_DOWN:
			 if (editchange == 0)
				WM_SetFocusOnNextChild(hDlg_SysSet);
			 if (editchange == 1)
				 editchange = 0;
			  i = 0;
			break;
		 
		 case GUI_KEY_RIGHT:
			  
				EDIT_SetText(hObj,Dropdownmonth[i]);
				i++;
				if (i == 7)
				i = 0;
			break;
		 }
}	

//static void btCallback_0(WM_MESSAGE* pMsg)
//{
//	const WM_KEY_INFO * pInfo;
//	
//	switch(pMsg->MsgId)
//	{
//		case WM_KEY:
//			pInfo  = (WM_KEY_INFO*)(pMsg->Data.p);
//		
//		  switch(pInfo->Key)
//			{
//				case GUI_KEY_UP:
//MYDEBUG("button 0 up");					
//          break;
//        case GUI_KEY_DOWN:
//MYDEBUG("button 0 down");					
//          break;
//        case GUI_KEY_RIGHT:
//MYDEBUG("button 0 right");					
//          break;
//        default:
//          BUTTON_Callback(pMsg);					
//			}
//			
//			break;
//			
//				default:
//					BUTTON_Callback(pMsg);
//	}
//}

//static void btCallback_1(WM_MESSAGE* pMsg)
//{
//	;
//}

//static void btCallback_2(WM_MESSAGE* pMsg)
//{
//	;
//}

//static void btCallback_3(WM_MESSAGE* pMsg)
//{
//	;
//}


void _cbWindowMenu(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;			
	WM_HWIN _button0;
	WM_HWIN _button1;
	WM_HWIN _button2;
	WM_HWIN _button3;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			BUTTON_CreateEx(0, 50, 120, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
			_button0 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON0);
//		 WM_SetCallback(_button0,&btCallback_0);
		
			BUTTON_CreateEx(0, 100, 120, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
			_button1 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON1);
//		 WM_SetCallback(_button1,&btCallback_1);
		
			BUTTON_CreateEx(0, 150, 120, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON2);
			_button2 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON2);
//		WM_SetCallback(_button2,&btCallback_2);
		
			BUTTON_CreateEx(0, 200, 120, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON3);
			_button3 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON3);
//		WM_SetCallback(_button3,&btCallback_3);
		
		


			WIDGET_SetDefaultEffect(&WIDGET_Effect_Simple);
		break;
		case WM_PAINT:
			GUI_SetBkColor (GUI_LIGHTBLUE);
			GUI_Clear();
			GUI_SetPenSize(2);
			//菜单分割线
 			GUI_DrawLine (120,0,120,480);

			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_SetColor (GUI_WHITE);
			GUI_SetFont (&GUI_Font28);
			GUI_DispStringAt ("主菜单",15,10);
			BUTTON_SetTextColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON0),BUTTON_CI_UNPRESSED,GUI_WHITE);
			BUTTON_SetTextColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON1),BUTTON_CI_UNPRESSED,GUI_WHITE);
			BUTTON_SetTextColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON2),BUTTON_CI_UNPRESSED,GUI_WHITE);
			BUTTON_SetTextColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON3),BUTTON_CI_UNPRESSED,GUI_WHITE);
			BUTTON_SetText(WM_GetDialogItem (hWin, GUI_ID_BUTTON0),"监控列表");
			BUTTON_SetText(WM_GetDialogItem (hWin, GUI_ID_BUTTON1),"监控设置");
			BUTTON_SetText(WM_GetDialogItem (hWin, GUI_ID_BUTTON2),"船舶列表");
			BUTTON_SetText(WM_GetDialogItem (hWin, GUI_ID_BUTTON3),"系统设置");
			BUTTON_SetBkColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON0),BUTTON_CI_UNPRESSED,DEEPBLUE);
			BUTTON_SetBkColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON1),BUTTON_CI_UNPRESSED,DEEPBLUE);
			BUTTON_SetBkColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON2),BUTTON_CI_UNPRESSED,DEEPBLUE);
			BUTTON_SetBkColor(WM_GetDialogItem (hWin, GUI_ID_BUTTON3),BUTTON_CI_UNPRESSED,DEEPBLUE);
			BUTTON_SetBkColor (WM_GetDialogItem (hWin, menufocus),BUTTON_CI_UNPRESSED,GUI_WHITE);
			BUTTON_SetTextColor(WM_GetDialogItem (hWin, menufocus),BUTTON_CI_UNPRESSED,GUI_BLACK);

		break;
		
		case WM_KEY:
 			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
 			{
				case GUI_KEY_MENU:
					WM_BringToTop (hDlg_FishMap);
					WM_SetFocus (hDlg_FishMap);
					menufocus = 368;
				break;
				
				case GUI_KEY_LEFT:
;
				break;
				
				case GUI_KEY_RIGHT:	
					printf ("right\r\n");
					if (menufocus == 368) //监控列表
					{
						WM_SetFocus(WM_GetDialogItem (hDlg_MonitorList, GUI_ID_LISTVIEW1));
						LISTVIEW_SetSel ( WM_GetDialogItem (hDlg_MonitorList, GUI_ID_LISTVIEW1), 0);
						focuschange = 1;
						menufocus += 5;
						WM_Paint(hWin);
					}
					else if (menufocus == 369) //监控设置
					{
						WM_SetFocus(WM_GetDialogItem (hDlg_MonitSet, GUI_ID_LISTVIEW0));
						LISTVIEW_SetSel ( WM_GetDialogItem (hDlg_MonitSet, GUI_ID_LISTVIEW0), 0);
						focuschange = 1;						
						menufocus += 5;
						WM_Paint(hWin);
					}
					else if (menufocus == 370) //船舶列表
					{
						WM_SetFocus(WM_GetDialogItem (hDlg_AllList, GUI_ID_LISTVIEW0));
						LISTVIEW_SetSel ( WM_GetDialogItem (hDlg_AllList, GUI_ID_LISTVIEW0), 0);
						focuschange = 1;							
						menufocus += 5;
						WM_Paint(hWin);
					}
					else if (menufocus == 371) //系统设置
					{
						WM_SetFocus(WM_GetDialogItem (hDlg_SysSet, GUI_ID_EDIT0));
						focuschange = 1;							
						menufocus += 5;
						WM_Paint(hWin);
						
					}
					
				break;
				
				case GUI_KEY_UP:
						menufocus--;
						if (menufocus == 367)
							menufocus = 371;
						WM_SetFocusOnPrevChild(hWin);
						WM_Paint(hWin);	
						if (menufocus == 368)
							WM_BringToTop (hDlg_MonitorList);
						else if (menufocus == 369)
							WM_BringToTop (hDlg_MonitSet);
						else if (menufocus == 370)
							WM_BringToTop (hDlg_AllList);
						else if (menufocus == 371)
							WM_BringToTop (hDlg_SysSet);
				break;

				case GUI_KEY_DOWN:
						menufocus++;
						if (menufocus == 372)
							menufocus = 368;
						WM_SetFocusOnNextChild(hWin);
						WM_Paint(hWin);
						if (menufocus == 368)
							WM_BringToTop (hDlg_MonitorList);
						else if (menufocus == 369)
							WM_BringToTop (hDlg_MonitSet);
						else if (menufocus == 370)
							WM_BringToTop (hDlg_AllList);
						else if (menufocus == 371)
							WM_BringToTop (hDlg_SysSet);						
				break;

			}
		break;
	
		default:
		WM_DefaultProc(pMsg);
	}
}

void MainTask(void) {
  GUI_Init();
  WM_SetCreateFlags(WM_CF_MEMDEV);
	
	//创建字体
 	GUI_UC_SetEncodeUTF8();	
	//字体设置	//GUI_SetDefaultFont (&SIF_Font);
	GUI_SetDefaultFont (&GUI_Font28);
	DROPDOWN_SetDefaultFont (&GUI_Font28);
	TEXT_SetDefaultFont (&GUI_Font32B_1);
	BUTTON_SetDefaultFont(&GUI_Font28);
	//皮肤设置
	//BUTTON_SetDefaultSkin (BUTTON_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin (DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin (FRAMEWIN_SKIN_FLEX);
	BUTTON_SetDefaultFocusColor(GUI_RED);
	
	//创建窗口 
	hDlg_Menu = WM_CreateWindowAsChild(0, 0, 122, 480, WM_HBKWIN, WM_CF_SHOW, &_cbWindowMenu, 0); 
	hDlg_Messagebox = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate),&_cbWindowMessage, WM_HBKWIN, 0, 0);
  hDlg_FishMap = WM_CreateWindowAsChild (0, 0, 800, 480, WM_HBKWIN, WM_CF_SHOW, &_cbWindowAllFishMap, 0);  
	hDlg_AllList = _CreateFrame (&_cbWindowAllList);
	hDlg_MonitorList = _CreateFrame (&_cbWindowMonitorList);
	hDlg_SysSet = _CreateFrame (&_cbWindowSetting);
	hDlg_MonitSet = _CreateFrame (&_cbWindowMonitSet);
	
	//焦点设置
	WM_BringToTop (hDlg_FishMap);
	WM_SetFocus (hDlg_FishMap);

	
	while(1)
	{
	  WM_Invalidate(hDlg_AllList);
//		 WM_Invalidate(hDlg_FishMap);
		GUI_Delay(200);
	}
}

/*************************** End of file ****************************/
