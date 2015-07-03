#include "MainTask.h"
#define GUI_ID_BUTTON10 0x800
#define GUI_ID_BUTTON11 0x801
#define GUI_ID_BUTTON12 0x802
#define GUI_ID_BUTTON13 0x803
#define GUI_ID_BUTTON14 0x804
#define GUI_ID_BUTTON15 0x805
#define GUI_ID_BUTTON16 0x806


char i=0;
static const char *Dropdownyear[] = {"2015","2016","2017","2018","2019","2020","2021","2022","2023","2024","2025","2026","2027","2028","2029","2030"};
		
void PaintDialog (WM_MESSAGE * pMsg) 
{
	GUI_SetBkColor (GUI_GRAY);
	GUI_SetColor (GUI_WHITE);                                                                                           
	GUI_Clear ();
	GUI_SetTextMode (GUI_TM_TRANS);
	GUI_SetColor (GUI_RED);
	GUI_DispStringAt ("系统设置", 5, 5);
	GUI_SetColor (GUI_BLACK);
	GUI_DispStringAt ("语言设置:", 200, 60);
	GUI_DispStringAt ("时间设置:", 200, 152);
	GUI_DispStringAt ("软件版本:", 200, 250);
	GUI_DispStringAt ("年", 355, 155);
	GUI_DispStringAt ("月", 440, 155);                                                                                                                                                                                       
	GUI_DispStringAt ("日", 525, 155);
}

void _cbWindowSetting(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	WM_HWIN _edit0;
	WM_HWIN _edit1;
  int edit_id = 256;
	int     VALUE;
	int      i = 0;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:

 			WIDGET_SetDefaultEffect (&WIDGET_Effect_None);
		
			EDIT_SetDefaultTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
			EDIT_SetDefaultFont(&GUI_Font28);
 			EDIT_CreateEx(200,60,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT0,1);//夜间模式
		
 			EDIT_CreateEx(150,110,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT1,1);//音量
		
			EDIT_CreateEx(150,160,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT2,1);//亮度
		
 			EDIT_CreateEx(150,210,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT3,2);//时区
			
 			EDIT_CreateEx(240,260,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT4,1);//报警音1
			
 			EDIT_CreateEx(170,310,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT5,1);//按键音1
 			EDIT_CreateEx(200,360,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT6,1);//单位显示
 			EDIT_CreateEx(200,410,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT7,1);//船形显示
			
			for (i = 0; i < 8; i++) {
				_edit0 = WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i);

 				EDIT_SetText(_edit0,"0");
 				EDIT_SetpfAddKeyEx (_edit0,_cbEditAddKey);
				EDIT_SetBkColor(_edit0,1,GUI_LIGHTBLUE);
				EDIT_SetBkColor(_edit0,0,GUI_WHITE);
		}
		break;
		
		case WM_PAINT:
 			GUI_SetBkColor (GUI_LIGHTBLUE);
 			GUI_Clear();
 		
				for (i = 0; i < 8; i++) {
				_edit0 = WM_GetDialogItem(hWin, GUI_ID_EDIT0 + i);
				EDIT_SetTextColor(_edit0,0,GUI_BLACK);
			}
			EDIT_SetTextColor (WM_GetDialogItem(hWin, edit_id),0,GUI_WHITE);
			
			GUI_SetTextMode(GUI_TEXTMODE_TRANS);
			GUI_SetColor (GUI_BLACK);
			GUI_SetFont (&GUI_Font28);
			GUI_DispStringAt ("系统设置",230,10);
			GUI_DispStringAt ("01:夜间模式:",5,60);
			GUI_DispStringAt ("02:音量:",5,110);
			GUI_DispStringAt ("03:亮度:",5,160);
			GUI_DispStringAt ("04:时区:",5,210);
			GUI_DispStringAt ("05:报警音选择:",5,260);
			GUI_DispStringAt ("06:按键音:",5,310);
			GUI_DispStringAt ("07:单位设置:",5,360);
			GUI_DispStringAt ("08:船位显示:",5,410);
			GUI_DispStringAt ("09:船迹存储时间:",330,60);
			GUI_DispStringAt ("10:软件更新:",330,110);
			GUI_DispStringAt ("11:系统版本:",330,160);
			GUI_SetColor (GUI_WHITE);
			GUI_SetPenSize(2);
			GUI_DrawLine(1,50,1,470);
			GUI_DrawLine(1,50,673,50);
			GUI_DrawLine(673,50,673,470);
			GUI_DrawLine(1,470,673,470);
		break;

 		case WM_KEY:
 			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key)
 			{
				case GUI_KEY_TAB:
					//判断当前焦点所在的位置，将焦点移至下一个窗口
					downlistfocus++;
					if (downlistfocus == 369)
							downlistfocus = 384;
					if (downlistfocus == 388)
						  downlistfocus = 256;
					if (downlistfocus == 257)
							downlistfocus = 368;
					//设置焦点
						WM_SetFocus(WM_GetDialogItem (hWin, downlistfocus));

				break;

				case GUI_KEY_BACKTAB:
					//判断当前焦点所在的位置，将焦点移至上一个窗口
							downlistfocus--;
					if (downlistfocus == 383)
							downlistfocus = 368;
					if (downlistfocus == 367)
							downlistfocus = 256;
					if (downlistfocus == 255)
							downlistfocus = 387;
					//设置焦点		
 						WM_SetFocus (WM_GetDialogItem(hWin, downlistfocus));					

				break;
				
				case GUI_KEY_RIGHT:	
				break;
				
				case GUI_KEY_F1:	
				break;
				
				case GUI_KEY_F2:
				break;
				
				case GUI_KEY_SPACE:
					printf ("ddsf");
				break;
				
				case GUI_KEY_ENTER:
				break;
				
				case GUI_KEY_UP:
					
				break;
				
				case GUI_KEY_DOWN:
 					WM_SetFocusOnNextChild(hWin);
					//edit_id ++;
				break;
				
 				case GUI_KEY_LEFT:
					menufocus -= 5;
					focuschange = 0;
					WM_SetFocus(WM_GetDialogItem (hDlg_Menu, menufocus));
					WM_Paint(hDlg_Menu);
				break;

 				case GUI_KEY_MENU:
					WM_BringToTop (hDlg_FishMap);
					WM_SetFocus (hDlg_FishMap);
					menufocus = 368;
				break;
			}
		break;

		default:
		WM_DefaultProc(pMsg);
	}
}
