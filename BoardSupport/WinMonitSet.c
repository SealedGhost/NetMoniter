#include "MainTask.h"


static   char cBuf[12];
static   char * pBuf = cBuf;

WM_HWIN hEdit_0;
WM_HWIN hEdit_1;
WM_HWIN hEdit_2;
WM_HWIN hEdit_3;
WM_HWIN hEdit_4;
WM_HWIN hEdit_5;
WM_HWIN hEdit_6;

WM_HWIN hEdits[7];


char * table[] ={"0.1","0.2","0.3","0.4","0.5","0.6","0.7","0.8","0.9","1.0","1.1","1.2","1.3","1.4","1.5","1.6","1.7","1.8","1.9","2.0"};
static const char * _aTable_1[][4] = {
  { "o", "x", },
  { "1", "5", },
  { "2", "4", },
  { "5", "1", },
  { "3", "6", },
  { "4", "7",  },
  { "33333333333333333333", },
  { "623496-0010008", "9",  },
  { "623497-0010008", "10",  },

};
static char MonitSettingFlags = 0xff;
static  int  burglarAlarm  = 0;
static  int  anchorOffset  = 0;

int 	LEN_AllList = GUI_COUNTOF(_aTable_1);//需要添加的行数
int   RowNum_AllList = 0;  //行数
int   RowNumSel_AllList = 0;//当前所选中的行数


static void updateListViewContent(WM_HWIN thisWin)
{
	int i  = 0;
	
	unsigned int Counter    = 0;
	unsigned int NumOfRows  = 0;
	
	
	WM_HWIN hListView  = WM_GetDialogItem(thisWin,GUI_ID_LISTVIEW0);
	
MYDEBUG("hListView=%ld",hListView);	

	for(i=0;i<N_boat;i++)
	{

			NumOfRows  = LISTVIEW_GetNumRows(hListView);
			
			if(i+1 > NumOfRows)
			{
				
				//LISTVIEW_AddRow(hListView,NULL);
				LISTVIEW_AddRow(hListView,NULL); 
NumOfRows  = LISTVIEW_GetNumRows(hListView);
MYDEBUG("num of rows=%d",NumOfRows);				
			}
			
			LISTVIEW_SetItemText(hListView,0,i,test[i].name);
MYDEBUG("%ld",test[i].user_id);			
			sprintf(pBuf,"%ld",test[i].user_id);
			LISTVIEW_SetItemText(hListView,1,i,pBuf);
			
			if(test[i].isVisible)
				LISTVIEW_SetItemText(hListView,2,i,"o");
			else
				LISTVIEW_SetItemText(hListView,2,i,"x");
	
	}
	
	while(NumOfRows > i)
	{
		LISTVIEW_DeleteRow(hListView,i);
		NumOfRows  = LISTVIEW_GetNumRows(hListView);
	}
//NumOfRows  = LISTVIEW_GetNumRows(hListView);
//MYDEBUG("final num of rows=%d",NumOfRows);
	
}


/*监控设置界面 ListView 的回调*/
static void myListViewCBFunc(WM_MESSAGE * pMsg)
{
  int selectedRow  = -1;
	 char buf[10];
	
	const WM_KEY_INFO * pInfo;
	LISTVIEW_Handle thisListView = pMsg->hWin;
	
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			LISTVIEW_Callback(pMsg);		
		break;
		
		case WM_KEY:				
			pInfo = (WM_KEY_INFO*)pMsg->Data.p;
		
			switch(pInfo->Key)
			{
				case GUI_KEY_UP:
				  LISTVIEW_Callback(pMsg);
					break;
				
				case GUI_KEY_DOWN:
				  LISTVIEW_Callback(pMsg);
					break;
				
				case GUI_KEY_ENTER:				
				  selectedRow  = LISTVIEW_GetSel(thisListView);			
				  LISTVIEW_GetItemText(thisListView,2,selectedRow,pBuf,10);
	
					if(pBuf[0] == 'x' )
					{					
						test[selectedRow].isVisible = 1;
						LISTVIEW_SetItemText(thisListView,2,selectedRow,"o");
					}
					else
					{			
						test[selectedRow].isVisible = 0;
						LISTVIEW_SetItemText(thisListView,2,selectedRow,"x");
					}
						
					break;
					
        case GUI_KEY_RIGHT:
          WM_SetFocus (WM_GetDialogItem (hDlg_MonitSet, GUI_ID_EDIT0));	
MYDEBUG("case right");				
					break;
				case GUI_KEY_LEFT:
MYDEBUG("case left");		
            menufocus = 369	;			
						WM_SetFocus(WM_GetDialogItem (hDlg_Menu, GUI_ID_BUTTON1));
						WM_Paint(hDlg_Menu);
					break;
//				
				default:
					LISTVIEW_Callback(pMsg);
			}
						
		break;
			
				default:
					LISTVIEW_Callback(pMsg);
	}
}




void _cbEditAddKey_1(EDIT_Handle hObj, int Key) {

MYDEBUG("Entry");	
	 switch (Key) {
		 case GUI_KEY_DOWN:
MYDEBUG("case down");			 
			 focus_monitset++;
				if (focus_monitset == 7)
				focus_monitset = 0;					
				WM_SetFocus(hEdits[focus_monitset]);
			break;
		 
			case GUI_KEY_UP:
MYDEBUG("case up");				
				if (focus_monitset == 0)
					focus_monitset = 7;		
				focus_monitset--;			
				WM_SetFocus(hEdits[focus_monitset]);
			break;
				
		 case GUI_KEY_RIGHT:
MYDEBUG("case right");			 
		 ///消失报警设置项切换
		    if(hObj  ==  hEdits[0])
				{		
					if(MonitSettingFlags &  (0x01<<7))
					{
						EDIT_SetText(hObj,"开启");
						MonitSettingFlags = MonitSettingFlags & (~(0x01<<7));
					}
					else 
					{
						EDIT_SetText(hObj,"关闭");
						MonitSettingFlags = MonitSettingFlags | (0x01<<7);
					}
				}
				///防盗报警设置项切换
				else if(hObj  == hEdits[1])
				{
					if(MonitSettingFlags & (0x01<<6))
					{
						EDIT_SetText(hEdits[1],"开启");
						MonitSettingFlags   &= (~(0x01<<6));						
					}
					else
					{
						EDIT_SetText(hEdits[1],"关闭");
						MonitSettingFlags  |= (0x01<<6);
					}
				}
				///报警距离设置
			 else if(hObj  == hEdits[2])
			 {
				 burglarAlarm ++;
				 burglarAlarm  = burglarAlarm%21;
				 
				 sprintf(pBuf,"%d",burglarAlarm);
				 EDIT_SetText(hEdits[2],pBuf);
			 }
			 ///声音开/关设置
			 else if(hObj  == hEdits[3])
				{
					if(MonitSettingFlags & (0x01<<5))
					{
						EDIT_SetText(hEdits[3],"开启");
						MonitSettingFlags   &= (~(0x01<<5));						
					}
					else
					{
						EDIT_SetText(hEdits[3],"关闭");
						MonitSettingFlags  |= (0x01<<5);
					}
				}
				///走锚报警设置
				else if(hObj  == hEdits[4])
				{
					if(MonitSettingFlags & (0x01<<4))
					{
						EDIT_SetText(hEdits[4],"开启");
						MonitSettingFlags   &= (~(0x01<<4));						
					}
					else
					{
						EDIT_SetText(hEdits[4],"关闭");
						MonitSettingFlags  |= (0x01<<4);
					}
				}
				///走锚报警距离设置
				else if(hObj  == hEdits[5])
			 {
				 anchorOffset ++;
				 anchorOffset  = anchorOffset%21;
				 
				 sprintf(pBuf,"%dm",anchorOffset);
				 EDIT_SetText(hEdits[5],pBuf);
			 }
			 else if(hObj  == hEdits[6])
			 {
				 if(MonitSettingFlags & (0x01<<3))
				 {
					 EDIT_SetText(hEdits[6],"开启");
					 MonitSettingFlags  &= (~(0x01<<3));
				 }
				 else
				 {
					 EDIT_SetText(hEdits[6]," 关闭");
					 MonitSettingFlags  |= (0x01<<3);
				 }
			 }
			break;
				
		 case GUI_KEY_LEFT:
//					
//          _edit  = hObj;
		 ///消失报警设置项切换
		    if(hObj  ==  hEdits[0])
				{		
					if(MonitSettingFlags &  (0x01<<7))
					{
						EDIT_SetText(hObj,"开启");
						MonitSettingFlags = MonitSettingFlags & (~(0x01<<7));
					}
					else 
					{
						EDIT_SetText(hObj,"关闭");
						MonitSettingFlags = MonitSettingFlags | (0x01<<7);
					}
				}
				///防盗报警设置项切换
				else if(hObj  == hEdits[1])
				{
					if(MonitSettingFlags & (0x01<<6))
					{
						EDIT_SetText(hEdits[1],"开启");
						MonitSettingFlags   &= (~(0x01<<6));						
					}
					else
					{
						EDIT_SetText(hEdits[1],"关闭");
						MonitSettingFlags  |= (0x01<<6);
					}
				}
				///报警距离设置
			 else if(hObj  == hEdits[2])
			 {
				 burglarAlarm --;
				 burglarAlarm  = (burglarAlarm+21)%21;
				 
				 sprintf(pBuf,"%d",burglarAlarm);
				 EDIT_SetText(hEdits[2],pBuf);
			 }
			 ///声音开/关设置
			 else if(hObj  == hEdits[3])
				{
					if(MonitSettingFlags & (0x01<<5))
					{
						EDIT_SetText(hEdits[3],"开启");
						MonitSettingFlags   &= (~(0x01<<5));						
					}
					else
					{
						EDIT_SetText(hEdits[3],"关闭");
						MonitSettingFlags  |= (0x01<<5);
					}
				}
				///走锚报警设置
				else if(hObj  == hEdits[4])
				{
					if(MonitSettingFlags & (0x01<<4))
					{
						EDIT_SetText(hEdits[4],"开启");
						MonitSettingFlags   &= (~(0x01<<4));						
					}
					else
					{
						EDIT_SetText(hEdits[4],"关闭");
						MonitSettingFlags  |= (0x01<<4);
					}
				}
				///走锚报警距离设置
				else if(hObj  == hEdits[5])
			 {
				 anchorOffset --;
				 anchorOffset  = (anchorOffset+21)%21;
				 
				 sprintf(pBuf,"%dm",anchorOffset);
				 EDIT_SetText(hEdits[5],pBuf);
			 }
			 else if(hObj  == hEdits[6])
			 {
				 if(MonitSettingFlags & (0x01<<3))
				 {
					 EDIT_SetText(hEdits[6],"开启");
					 MonitSettingFlags  &= (~(0x01<<3));
				 }
				 else
				 {
					 EDIT_SetText(hEdits[6]," 关闭");
					 MonitSettingFlags  |= (0x01<<3);
				 }
			 }
			break;
			
			case GUI_KEY_BACKSPACE:
MYDEBUG("case backspace");				
				WM_BringToTop (hDlg_Messagebox);
				WM_SetFocus (hDlg_Messagebox);
				printf ("cancel");
			break;
		 }
}	

GUI_WIDGET_CREATE_INFO _aDialogCreate[3] = {
  { FRAMEWIN_CreateIndirect,  NULL,           0,        150,  120,  450,200,FRAMEWIN_CF_MOVEABLE,0  },
  { BUTTON_CreateIndirect,   "1:确定",     GUI_ID_OK,       65,   130,  110,  35 },
  { BUTTON_CreateIndirect,   "2:取消", GUI_ID_CANCEL,    270,  130,  110,  35 },
};


void addrow_LISTVIEW_AllList(WM_MESSAGE* pMsg) 
{
		WM_HWIN _hListView = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);  //获得LISTVIEW句柄
		dropdown0flag = 1;
		for (RowNum_AllList = 0; RowNum_AllList < GUI_COUNTOF(_aTable_1); RowNum_AllList++)
				LISTVIEW_AddRow (_hListView,  _aTable_1[RowNum_AllList]);	
}

/* 退出警告对话框的回调 */
void _cbWindowMessage(WM_MESSAGE * pMsg) 
{
 	WM_HWIN hWin;
	int     NCode;
	int     Id;
 	hWin = pMsg->hWin;
	
 	switch (pMsg->MsgId) 
 	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_WHITE);
			GUI_Clear();
			GUI_SetTextMode (GUI_TM_TRANS);
			GUI_SetColor (GUI_BLACK);
			GUI_SetFont (&GUI_Font28);
				GUI_DispStringAt ("确定保存监控设置项？ ", 55, 30);
		break;
		
	case WM_INIT_DIALOG:
		BUTTON_SetSkin(WM_GetDialogItem (hWin, GUI_ID_OK),BUTTON_SKIN_FLEX);
		BUTTON_SetSkin(WM_GetDialogItem (hWin, GUI_ID_CANCEL),BUTTON_SKIN_FLEX);
 		FRAMEWIN_SetTitleHeight(hWin,0);
		
	break;
	
 	case WM_KEY:
		switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
	{
		case GUI_KEY_ENTER:
		break;
		
		case GUI_KEY_LEFT:
			GUI_StoreKeyMsg(GUI_KEY_BACKTAB,1);							
		break;
		
		case GUI_KEY_RIGHT:		
			GUI_StoreKeyMsg(GUI_KEY_TAB,1);		
		break;
	}
    case WM_NOTIFY_PARENT:
			 Id    = WM_GetId(pMsg->hWinSrc);
			 NCode = pMsg->Data.v; 
			switch (NCode) {
				case WM_NOTIFICATION_RELEASED:
					 if (Id == GUI_ID_OK) 
						{ 
						  WM_BringToTop (hDlg_MonitSet);
							WM_SetFocus(WM_GetDialogItem (hDlg_MonitSet, GUI_ID_LISTVIEW0));
							LISTVIEW_SetSel ( WM_GetDialogItem (hDlg_MonitSet, GUI_ID_LISTVIEW0), 0);
							focuschange = 1;
						}
          if (Id == GUI_ID_CANCEL) 
					{        
            WM_BringToTop (hDlg_MonitSet);
						WM_SetFocus(WM_GetDialogItem (hDlg_MonitSet, GUI_ID_LISTVIEW0));
						LISTVIEW_SetSel ( WM_GetDialogItem (hDlg_MonitSet, GUI_ID_LISTVIEW0), 0);
						focuschange = 1;
          }
					break;
				}
		default:
	  	WM_DefaultProc(pMsg);
	}
}


void _cbWindowMonitSet(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	WM_HWIN _hListView;
	WM_HWIN _check;
	WM_HWIN _edit0;
	int index;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			WM_SetFocus (hWin);
		LISTVIEW_CreateEx (7, 106, 410, 369, hWin,WM_CF_SHOW|WM_CF_MEMDEV, 0, GUI_ID_LISTVIEW0);
		_hListView=WM_GetDialogItem (pMsg->hWin, GUI_ID_LISTVIEW0);  //获得LISTVIEW句柄
		WM_SetCallback(_hListView,&myListViewCBFunc);
		
		WIDGET_SetEffect (_hListView, &WIDGET_Effect_None);  //设置小工具效果
		LISTVIEW_SetFont(_hListView,&GUI_Font24B_1);
		WM_SetFocus (_hListView);  //设置窗口焦点
		LISTVIEW_SetHeaderHeight (_hListView, 0);//标题高度
		LISTVIEW_SetGridVis (_hListView, 1);      //网格的可见性

		LISTVIEW_SetRowHeight (_hListView, 41);  //行高
		LISTVIEW_SetAutoScrollV ( _hListView, 0 ); //滚动条
		LISTVIEW_SetFont (_hListView, &GUI_Font24B_1);  //设置字体
		LISTVIEW_SetTextColor (_hListView, LISTVIEW_CI_SELFOCUS,GUI_BLACK); //选中时的文本颜色
		LISTVIEW_SetTextAlign(_hListView,0, GUI_TA_LEFT);
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_SELFOCUS,GUI_WHITE);   //选中时的单元格背景颜色
		LISTVIEW_SetTextColor (_hListView, LISTVIEW_CI_UNSEL,GUI_WHITE);    //未选中的文本颜色		
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_UNSEL,DEEPBLUE);       //未选中时单元格的背景颜色	
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_SEL ,DEEPBLUE);        //选定无焦点		
		//添加列
		LISTVIEW_AddColumn (_hListView, 240, NULL,  GUI_TA_LEFT|GUI_TA_VCENTER);
		LISTVIEW_AddColumn (_hListView, 112, NULL,  GUI_TA_LEFT|GUI_TA_VCENTER);
		LISTVIEW_AddColumn (_hListView, 58, NULL,  GUI_TA_HCENTER|GUI_TA_VCENTER);	
		//排序
//	  LISTVIEW_SetCompareFunc(_hListView,0,LISTVIEW_CompareDec);
//	  LISTVIEW_SetSort(_hListView,0,1);
//		LISTVIEW_EnableSort(_hListView);
//		CHECKBOX_CreateEx(375,10,30,30,_hListView,WM_CF_SHOW,0,GUI_ID_CHECK0);
//		_check = WM_GetDialogItem (pMsg->hWin, GUI_ID_CHECK0);
//		CHECKBOX_SetSkin (_check,CHECKBOX_DrawSkinFlex);
//		CHECKBOX_SetSkinFlexButtonSize (_check,20);
//		
//		 addrow_LISTVIEW_AllList (pMsg); //添加行数据		

		updateListViewContent(hWin);
		
		EDIT_SetDefaultFont(&GUI_Font28);
		EDIT_CreateEx(590,55,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT0,2);  //消失报警
		EDIT_SetText(WM_GetDialogItem (hWin, GUI_ID_EDIT0),"开启");
		EDIT_CreateEx(590,125,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT1,2); //防盗报警
		EDIT_SetText(WM_GetDialogItem (hWin, GUI_ID_EDIT1),"开启");
		EDIT_CreateEx(590,160,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT2,2); //距离
		EDIT_SetText(WM_GetDialogItem (hWin, GUI_ID_EDIT2),"1km");
		EDIT_CreateEx(495,250,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT3,5); //声音
		EDIT_SetText(WM_GetDialogItem (hWin, GUI_ID_EDIT3),"开启");
		EDIT_CreateEx(590,310,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT4,2); //走锚
		EDIT_SetText(WM_GetDialogItem (hWin, GUI_ID_EDIT4),"开启");
		EDIT_CreateEx(590,345,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT5,5); //距离
		EDIT_SetText(WM_GetDialogItem (hWin, GUI_ID_EDIT5),"12km");
		EDIT_CreateEx(495,440,62,30,hWin,WM_CF_SHOW,0,GUI_ID_EDIT6,2); //声音
		EDIT_SetText(WM_GetDialogItem (hWin, GUI_ID_EDIT6),"开启");
		for (index = 0; index < 7; index++) 
		{
			hEdits[index] = WM_GetDialogItem (hWin, GUI_ID_EDIT0+index);
			EDIT_SetpfAddKeyEx (hEdits[index],_cbEditAddKey_1);

		}
		break;
		
		case WM_PAINT:
				GUI_SetBkColor (GUI_LIGHTBLUE);
				GUI_Clear();
				GUI_SetFont (&GUI_Font28);
				GUI_SetPenSize(2);
				GUI_SetColor (DEEPBLUE);
				GUI_FillRect(5,55,417,106);
				//listview 边框线
				GUI_SetColor (GUI_WHITE);
				GUI_DrawLine (5,55,5,475);  //左
				GUI_DrawLine (5,55,417,55);   //上1
				GUI_DrawLine (5,104,417,104); //上2
				GUI_DrawLine (417,55,417,475);  //右
				GUI_DrawLine (5,475,417,475);  //下
				GUI_DrawLine (246,55,246,105);
				GUI_DrawLine (358,55,358,105);
				
				//右侧信息
				GUI_DrawLine (422,5,422,475);
				GUI_DrawLine (422,5,673,5);
				GUI_DrawLine (673,5,673,475);
				GUI_DrawLine (422,475,673,475);
		
				GUI_SetTextMode(GUI_TEXTMODE_TRANS);
				GUI_SetColor(GUI_WHITE);
				GUI_DispStringAt ("船舶监控设置",18,18);
				GUI_DispStringAt ("船名",110,66);
				GUI_DispStringAt ("MMSI",280,66);
				GUI_DispStringAt ("选择",360,66);
				GUI_DispStringAt ("监控设置项",475,20);
				
				GUI_SetFont (&GUI_Font28);
				GUI_DispStringAt ("1.消失报警:",426,55);
				GUI_DispStringAt ("2.防盗报警功能:",426,90);
				GUI_DispStringAt ("距离:",520,160);
				GUI_DispStringAt ("km",590,130);
				GUI_DispStringAt ("声音:",426,250);
				GUI_DispStringAt ("3.走锚报警功能:",426,280);
				GUI_DispStringAt ("距离:",520,345);
				GUI_DispStringAt ("km",590,310);
				GUI_DispStringAt ("声音:",426,440);
				
		break;
		
		case WM_KEY:
			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
			{
				case GUI_KEY_ENTER:
					if (WM_GetFocussedWindow() == WM_GetDialogItem (hWin, GUI_ID_LISTVIEW0))
					{
						if (LISTVIEW_GetSel(WM_GetDialogItem (hWin, GUI_ID_LISTVIEW0)) == 0)
						{
							if (CHECKBOX_IsChecked(WM_GetDialogItem (hWin, GUI_ID_CHECK0)))
								CHECKBOX_SetState(WM_GetDialogItem (hWin, GUI_ID_CHECK0),0);
							else CHECKBOX_SetState(WM_GetDialogItem (hWin, GUI_ID_CHECK0),1);
						}
					}
				break;
				case GUI_KEY_F1:
	
				break;

				case GUI_KEY_F2:

				break;
				
				case GUI_KEY_LEFT:
					if (focuschange == 1)
					{
						menufocus -= 5;
						focuschange = 0;
						WM_SetFocus(WM_GetDialogItem (hDlg_Menu, menufocus));
						WM_Paint(hDlg_Menu);
					}				
				break;
				case GUI_KEY_RIGHT:
MYDEBUG("a a a");
						WM_SetFocus (WM_GetDialogItem (hWin, GUI_ID_EDIT0));
						focuschange = 2;
				break;
				
 				case GUI_KEY_MENU:
						WM_BringToTop (hDlg_FishMap);
						WM_SetFocus (hDlg_FishMap);
						menufocus = 368;
 				break;
				
				case GUI_KEY_DOWN:
           printf ("down");
				break;
				
				case GUI_KEY_UP:

				break;				

				case GUI_KEY_TAB:
					//判断当前焦点所在的位置，将焦点移至下一个窗口
// 					downlistfocus++;
// 					if (downlistfocus == 389)
// 							downlistfocus = 384;
// 					//设置焦点
// 						WM_SetFocus(WM_GetDialogItem (hWin, downlistfocus));
// 				
				WM_SetFocusOnNextChild(hWin);
				break;
				
				case GUI_KEY_BACKTAB:
					WM_SetFocusOnPrevChild(hWin);
// 					printf ("backtab");
// 					//判断当前焦点所在的位置，将焦点移至上一个窗口
// 					if (downlistfocus > 384)
// 							downlistfocus--;
// 					else if (downlistfocus == 384)
// 							downlistfocus = 388;
// 					//设置焦点			
//  						WM_SetFocus (WM_GetDialogItem(hWin, downlistfocus));									
				break;
				
				case GUI_KEY_ESCAPE:    //取消
					if (Messageopenflag == 0 && dropdown0flag == 0)  //提示框关闭时并且下拉没打开
					{
						if (IsChange == 0)                             //监控参数没更改
						{
							WM_BringToTop (hDlg_FishMap);             //回到上个界面
							WM_SetFocus (hDlg_FishMap);
							MonitSetOpenflag = 0;
							Doubleclick = 0;
							downlistfocus = 384;
							GUI_CURSOR_Show();
						}
						else                                           //参数更改
						{   																					 //弹出提示框
							Messageopenflag = 1;
							WM_BringToTop (hDlg_Messagebox);
							WM_SetFocus (hDlg_Messagebox);
							IsChange = 0;
						}
					}
					else if (Messageopenflag == 1)                   //提示框打开时
					{
						WM_BringToTop(hDlg_MonitSet);
						WM_SetFocus (WM_GetDialogItem (hDlg_MonitSet, downlistfocus));
						Messageopenflag = 0;
					}
				break;
				
			case GUI_KEY_MONITORING:   //监控
				if (Messageopenflag == 1)														//提示框打开时
				{
					WM_BringToTop(hDlg_MonitSet);
					WM_SetFocus (WM_GetDialogItem (hDlg_MonitSet, downlistfocus));
					Messageopenflag = 0;
				}
				else if (Messageopenflag == 0)										 //提示框关闭时
				{																									 //监控参数没更改
					if (IsChange == 0)
					{
						WM_BringToTop (hDlg_FishMap);							//直接回到上个界面
						WM_SetFocus (hDlg_FishMap);
						MonitSetOpenflag = 0;
						Doubleclick = 0;
						downlistfocus = 384;
						GUI_CURSOR_Show();
					}
					else 																						//监控参数更改，弹出提示框
					{
						Messageopenflag = 1;
						WM_BringToTop (hDlg_Messagebox);	
						WM_SetFocus (hDlg_Messagebox);
						IsChange = 0;
					}
				}
				break;

			}
		break;
		default:
	  	WM_DefaultProc(pMsg);
	}
}
