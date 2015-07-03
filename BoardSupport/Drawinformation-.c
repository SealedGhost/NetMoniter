#include "MainTask.h"

void _PaintFrame(void) 
{

	GUI_SetColor(0x000000);
	GUI_SetPenSize(2);
	GUI_DrawLine(LINE_X,0,LINE_X,455);
	GUI_DrawHLine(LINE_Y1,LINE_X,800);
	GUI_DrawHLine(LINE_Y2,LINE_X,800);
	GUI_DrawHLine(LINE_Y3,LINE_X,800);
	GUI_DrawHLine(LINE_Y4,LINE_X,800);
	GUI_DrawHLine(455,0,800);

//	GUI_SetFont(&SIF_Font);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringAt("AIS目标船数据", SIZE_X, SIZE_Y0);
	GUI_DispStringAt("MMSI:", SIZE_X, SIZE_Y1+10);
	GUI_DispStringAt("船名:", SIZE_X, SIZE_Y2+10);
	GUI_DispStringAt("北纬:", SIZE_X, SIZE_Y5+5);
	GUI_DispStringAt("东经:", SIZE_X, SIZE_Y6+15);
	GUI_DispStringAt("航速:", SIZE_X, SIZE_Y8);
	GUI_DispStringAt("航向:", SIZE_X2, SIZE_Y8);
	GUI_DispStringAt("距离:", SIZE_X, SIZE_Y9);
	GUI_DispStringAt("方位:", SIZE_X2, SIZE_Y9);
	GUI_DispStringAt("监控信息:", SIZE_X,SIZE_Y10+15);
	GUI_DispStringAt("网位仪移动报警距离:", SIZE_X, SIZE_Y12);
	GUI_DispStringAt("其他靠近报警距离:", SIZE_X, SIZE_Y13);
	GUI_DispStringAt("报警航速设置:", SIZE_X, SIZE_Y14);
	GUI_DispStringAt("本船位置:", SIZE_X, SIZE_Y15+15);

	GUI_DispStringAt("所有船舶/监控船舶",510,460);

	GUI_DispStringAt("", SIZE_X, SIZE_Y18);
	GUI_DispStringAt("", SIZE_X2, SIZE_Y19);
	GUI_DispStringAt("", SIZE_X, SIZE_Y20);
	GUI_DispStringAt("提示信息:", SIZE_X, SIZE_Y20+10);	
	GUI_SetFont(&GUI_Font32_ASCII);
	GUI_DispStringAt("N", SIZE_X, SIZE_Y16+15);
	GUI_DispStringAt("E", SIZE_X, SIZE_Y18+5);

	
}


void Drawinformation(WM_MESSAGE* pMsg){

	WM_HWIN hWin = pMsg->hWin;
	_PaintFrame();
	TEXT_CreateEx(SIZE_X1,SIZE_Y4+15,160,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT4,NULL);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT4),&GUI_Font32_ASCII);
	TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT4),TEXT_CF_RIGHT);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4),"24°36.690N");

	TEXT_CreateEx(SIZE_X1,SIZE_Y5+25,160,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT5,NULL);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT5),&GUI_Font32_ASCII);
	TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT5),TEXT_CF_RIGHT);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5),"118°54.209E");

	TEXT_CreateEx(SIZE_X,SIZE_Y16+15,160,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT13,NULL);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT13),&GUI_Font32_ASCII);
	TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT13),TEXT_CF_RIGHT);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT13),"24°41.184");

	TEXT_CreateEx(SIZE_X,SIZE_Y18+5,160,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT14,NULL);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT14),&GUI_Font32_ASCII);
	TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT14),TEXT_CF_RIGHT);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT14),"118°41.491");	

	TEXT_CreateEx(180,460,100,20,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT15,NULL);
//	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT15),&SIF_Font);
	TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT15),GUI_DARKRED);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT15),"渔区图/列表");	

//	TEXT_SetDefaultFont(&SIF_Font);//默认字体设置
	TEXT_CreateEx(SIZE_X+150,SIZE_Y0,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT0,"[7:00]");
	TEXT_CreateEx(SIZE_X1,SIZE_Y1+10,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT1,"411371444[A]");
	TEXT_CreateEx(SIZE_X1,SIZE_Y2+10,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT2,"MAN DE LI 66");
	TEXT_CreateEx(SIZE_X1,SIZE_Y3+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT3,"曼德力66");
	TEXT_CreateEx(SIZE_X1,SIZE_Y8,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT6,"7.2");
	TEXT_CreateEx(SIZE_X3,SIZE_Y8,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT7,"7.2");
	TEXT_CreateEx(SIZE_X1,SIZE_Y9,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT8,"2.2mm");
	TEXT_CreateEx(SIZE_X3,SIZE_Y9,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT9,"195°");

	TEXT_CreateEx(SIZE_X4,SIZE_Y12,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT10,"1.1海里");
	TEXT_CreateEx(SIZE_X4-10,SIZE_Y13,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT11,"25.0海里");
	TEXT_CreateEx(SIZE_X2+15,SIZE_Y14,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT12,"2节");

	TEXT_CreateEx(510,460,100,20,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT16,"所有船舶/监控船舶");
	
}
