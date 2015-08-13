#include "GUI.h"
#include "FRAMEWIN.h"
#include <stddef.h>
#include "DIALOG.h"
#include "MainTask.h"
//#include "GUIDEMO.h"
#include "exfuns.h"
#include "cover_colo_1.c"
#include "SystemConfig.h"
#include "Setting.h"


/*---------------------- external variables -------------------------------------*/


GUI_RECT lvRect  = {7, 106, 320, 369};


WM_HWIN mapWin;
WM_HWIN hDlg_FishMap;
WM_HWIN menuWin;
WM_HWIN confirmWin;
WM_HWIN loadingWin;
extern WM_HWIN subWins[4];

extern WM_HWIN loadingWinCreate(void);
extern WM_HWIN menuWinCreate(void);
extern WM_HWIN subCreateWindow(void);
extern WM_HWIN confirmWinCreate(void);
extern WM_HWIN mapWinCreate(void);
extern GUI_CONST_STORAGE GUI_FONT GUI_Font28;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font120;

void MainTask(void) {
	int i  = 0;
  WM_MESSAGE myMsg;
 
	GUI_MEMDEV_Handle hMem0;
  GUI_Init();	
  WM_SetCreateFlags(WM_CF_MEMDEV);
	//开机界面
	hMem0 = GUI_MEMDEV_Create(0,0,800,480);
	GUI_MEMDEV_Select(hMem0);
	GUI_SetBkColor (GUI_WHITE);
	GUI_Clear();
	GUI_SetColor (0xb58400);
	GUI_SetFont (&GUI_Font120);
	GUI_DispStringAt("AM-128",220,100);
 GUI_PNG_Draw (&_accover_colo_1,sizeof(_accover_colo_1),300,LCD_GetYSize()/2);
	GUI_MEMDEV_Select(0);
	GUI_MEMDEV_CopyToLCD(hMem0);
	GUI_MEMDEV_Delete(hMem0);
	GUI_Delay(1000);
	 GUI_Clear();
	//创建字体
 	GUI_UC_SetEncodeUTF8();	
	//字体设置	//GUI_SetDefaultFont (&SIF_Font);
	GUI_SetDefaultFont (&GUI_Font28);	
	TEXT_SetDefaultFont(&GUI_Font28);
	BUTTON_SetDefaultFont (&GUI_Font28);
	HEADER_SetDefaultFont(&GUI_Font28);
	HEADER_SetDefaultBkColor(DEEPBLUE);
	EDIT_SetDefaultFont(&GUI_Font28);
 WIDGET_SetDefaultEffect(&WIDGET_Effect_None);
	//创建窗口 

	loadingWin  = loadingWinCreate();

 confirmWin  = confirmWinCreate();
 menuWin  = menuWinCreate();
 hDlg_FishMap = WM_CreateWindowAsChild (0, 0, 800, 480, WM_HBKWIN, WM_CF_SHOW, &_cbWindowAllFishMap, 0);
 

// WM_SetFocus(menuWin);
// WM_SetFocus(hDlg_FishMap);

	//焦点设置
//mapWin  = mapWinCreate();
	
	//WM_BringToTop (hDlg_FishMap);
	//WM_SetFocus (hDlg_FishMap );
 
 

	while(1)
	{
//	  WM_Invalidate(subWins[2]);
//		WM_InvalidateWindow(subWins[2]);
//		WM_InvalidateWindow(WM_GetDialogItem(subWins[2],GUI_ID_USER + 0x01));
//printf("\r\nUI task");
		GUI_Delay(200);
	}
	
}

/*************************** End of file ****************************/
