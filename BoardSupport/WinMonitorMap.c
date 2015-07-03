#include "MainTask.h"

static int v;
void _cbWindowMonitorMap(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		
		case WM_USER+1:
		{
			 long data = 0;
			 long degree;
				float minute;
			 
			
//			 data = *((long*)pMsg->Data.p);
//				degree = data/(distance<<1);
//				minute = data%(distance<<1)/mul_pow;
//				GUI_DispDecAt(degree,SIZE_X+8,SIZE_Y5,3);
//				GUI_DispCharAt(176,SIZE_X+33,SIZE_Y5);
//				GUI_GotoX(SIZE_X+40);
//				GUI_DispFloat(minute,6);
			   GUI_DispDecAt(pMsg->Data.v,SIZE_X,SIZE_Y5,3);
			   WM_InvalidateWindow(hDlg_MonitorMap);

		}
		break;
		
		 
		case WM_CREATE:
			WM_SetFocus (hWin);
		break;
		
		case WM_PAINT:
			GUI_SetBkColor (GUI_GRAY);
			GUI_Clear ();
			GUI_SetColor (GUI_BLACK);
			GUI_SetTextMode (GUI_TM_TRANS);
			GUI_DispStringAt ("监控船舶渔区图:", 5, 5);
			GUI_DispStringAt ("abc", 200, 200);

			PaintFrame();
// 			InitText (pMsg);		
			GUI_DrawCircle (600, 210, 100);
		////////////
			GUI_SetFont (&GUI_Font16_ASCII);
			v = GUI_ALLOC_GetNumFreeBytes();
			GUI_DispDecAt (2008, 150, 5, 10);
			v = GUI_ALLOC_GetNumUsedBytes();
			GUI_DispDecAt (1992, 250, 5, 10);
		//////////////////
		break;
		
		case WM_KEY:
			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
			{
				case GUI_KEY_F1:
					printf("allmonitormap-->monitorlist_F1 ");
					WM_BringToTop (hDlg_MonitorList); //监控船舶列表
					WM_SetFocus (hDlg_MonitorList);
					dropdown0flag = 1;
					GUI_CURSOR_Hide();
				break;

				case GUI_KEY_F2:
					printf("allmonitormap-->allfishmap_F2 ");
					WM_BringToTop (hDlg_FishMap); 
					WM_SetFocus (hDlg_FishMap);
					GUI_CURSOR_Show();
				break;

 				case GUI_KEY_MENU:
						WM_BringToTop (hDlg_SysSet);
						WM_SetFocus (WM_GetDialogItem (hDlg_SysSet, GUI_ID_DROPDOWN0));
						dropdown0flag = 0;
						SysSetOpenflag = 1;
						Doubleclick = 1;
						GUI_CURSOR_Hide();
 				break;
			}
		break;
		default:
	  	WM_DefaultProc(pMsg);
	}
}
