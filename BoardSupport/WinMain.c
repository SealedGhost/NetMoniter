/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.26 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : Dialog_All.c
Purpose     : Example demonstrating DIALOG and widgets
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "DROPDOWN.h"
#include "MainTask.h"
//#include "DrawBMP.h"




// void _PaintFrame(void) 
// {
// 	GUI_RECT r;
// 	WM_GetClientRect(&r);
// 	GUI_SetBkColor(GUI_GRAY);
// 	GUI_SetColor(GUI_WHITE);
// 	//GUI_SetFont(FRAME_FONT);
// 	GUI_SetTextMode(GUI_TM_TRANS);
// 	GUI_ClearRectEx(&r);
// }

void _DeleteFrame(void) 
{
	WM_DeleteWindow(_hLastFrame);
	_hLastFrame = 0;
}

void _cbWindow_1(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			WM_SetFocus(hWin);
		
			break;
		case WM_KEY:
			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
			{
				case GUI_KEY_F1:
				printf("11");
				_CreateFrame(&_cbWindow2);
					//hDlg2 = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), &_cbWindow2, hDlg1, 0, 0);
				break;

				case GUI_KEY_F2:
				printf("12");
					//GUI_EndDialog(hWin, 0);
				_DeleteFrame();
				
				break;

			}
    break;
		case WM_PAINT:
			_PaintFrame();
		
			GUI_DrawCircle(250, 210, 100);
		break;
// 		case WM_NOTIFY_PARENT:
// 			if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) 
// 			{
// 				int Id = WM_GetId(pMsg->hWinSrc);
// 				switch (Id) 
// 				{
// 					case GUI_ID_BUTTON0:
// 						_Language = 0;
// 						break;
// 					case GUI_ID_BUTTON1:
// 						_Language = 1;
// 						break;
// 				}
// 				/* ????,???? */
// 				_hTitle = TEXT_CreateEx(0, 0, LCD_GetXSize(), 32, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_TEXT0, _GetLang(TEXT_ID_GELDAUTOMAT));
// 				TEXT_SetTextAlign(_hTitle, GUI_TA_HCENTER);
// 				TEXT_SetFont(_hTitle, MAIN_FONT);
// 				/* ????????? */
// 				_DeleteFrame();
// 				_CreateFrame(&_cbInsertCard);
// 			}
// 			break;
		default:
		WM_DefaultProc(pMsg);
	}
}

