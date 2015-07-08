#include "GUI.h"
#include "FRAMEWIN.h"
#include <stddef.h>
#include "DIALOG.h"
#include "MainTask.h"
//#include "GUIDEMO.h"
#include "exfuns.h"

GUI_RECT lvRect  = {7, 106, 320, 369};



WM_HWIN hDlg_FishMap;
WM_HWIN menuWin;
WM_HWIN confirmWin;
extern WM_HWIN subWins[4];

extern WM_HWIN menuWinCreate(void);
extern WM_HWIN subCreateWindow(void);
extern WM_HWIN ConfirmWinCreateFrm_Confirm(void);


void MainTask(void) {
  GUI_Init();	
  WM_SetCreateFlags(WM_CF_MEMDEV);
	
	//创建字体
 	GUI_UC_SetEncodeUTF8();	
	//字体设置	//GUI_SetDefaultFont (&SIF_Font);
	GUI_SetDefaultFont (&GUI_Font28);	
	
	//创建窗口 

	

	  menuWin  = menuWinCreate();
    hDlg_FishMap = WM_CreateWindowAsChild (0, 0, 800, 480, WM_HBKWIN, WM_CF_SHOW, &_cbWindowAllFishMap, 0);
	//焦点设置

	
	WM_BringToTop (hDlg_FishMap);
	WM_SetFocus (hDlg_FishMap );

	while(1)
	{
//	  WM_Invalidate(subWins[2]);
//		WM_InvalidateWindow(subWins[2]);
//		WM_InvalidateWindow(WM_GetDialogItem(subWins[2],GUI_ID_USER + 0x01));
		GUI_Delay(200);
	}
	
}

/*************************** End of file ****************************/
