#include "GUI.h"
#include "DIALOG.h"
#include "MainTask.h"
#include "exfuns.h"
#include "sysConf.h"
#include "Setting.h"
#include "logo.h"
#include "120.h"
#include "28.h"
#include "dlg.h"



void MainTask(void)
{
   GUI_MEMDEV_Handle hMem0;
   
   
   GUI_Init();	
   WM_SetCreateFlags(WM_CF_MEMDEV);
   //开机界面
   hMem0 = GUI_MEMDEV_Create(0,0,800,480);
   GUI_MEMDEV_Select(hMem0);
   GUI_SetBkColor (GUI_WHITE);
   GUI_Clear();
   
   /// Welcome logo 
   GUI_SetColor (0xb58400);
   GUI_SetFont (&GUI_Font120);
   GUI_DispStringAt("AM-128",220,100);
   GUI_PNG_Draw (&_accover_colo_1,sizeof(_accover_colo_1),300,LCD_GetYSize()/2);  
   GUI_MEMDEV_Select(0);
   GUI_MEMDEV_CopyToLCD(hMem0);
   GUI_MEMDEV_Delete(hMem0);
   
   sysInit();
   MNT_initSetting();
   
   GUI_Delay(1000);
   GUI_Clear();
   //创建字体
   GUI_UC_SetEncodeUTF8();	
   //字体设置	//GUI_SetDefaultFont (&SIF_Font);
   GUI_SetDefaultFont (&GUI_Font30);	
   TEXT_SetDefaultFont(&GUI_Font30);
   BUTTON_SetDefaultFont (&GUI_Font30);
   HEADER_SetDefaultFont(&GUI_Font30);
   HEADER_SetDefaultBkColor(GUI_BLUE);
   //EDIT_SetDefaultFont(&GUI_Font30);
   WIDGET_SetDefaultEffect(&WIDGET_Effect_None);
   //创建窗口 
//   sldWinCreate();
//     _sldWinCreate();
//     WM_SetFocus(_sldWinCreate());
   confirmWin  = confirmWinCreate();
	 WM_ShowWindow (confirmWin);
   mntSettingWin  = mntSettingWinCreate(); 
//  _mntSettingWin  = _mntSettingWinCreate();
   subWins[0]  = sub0WinCreate(); 
   subWins[1]  = sub1WinCreate();
   subWins[2]  = sub2WinCreate();
   subWins[3]  = _sub3WinCreate();
   menuWin  = menuWinCreate();
   mapWin = mapWinCreate();


   while(1)
   {
      GUI_Delay(200);
   }
}

/*************************** End of file ****************************/
