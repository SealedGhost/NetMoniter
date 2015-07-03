#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "DROPDOWN.h"
#include "MainTask.h"
//#include "DrawBMP.h"



void _cbWindow2(WM_MESSAGE* pMsg) 
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
				printf("21");
				
					//hDlg2 = GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), &_cbWindow2, hDlg1, 0, 0);
				break;

				case GUI_KEY_F2:
				printf("22");
					//GUI_EndDialog(hWin, 0);
				_DeleteFrame();
				_CreateFrame(&_cbWindow_1);
				break;

			}
    break;
		case WM_PAINT:
			_PaintFrame();
		
			GUI_DrawCircle(400, 210, 100);
			GUI_DrawCircle(250, 210, 100);
		GUI_DrawHLine(50,520,800);
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
