#include "MainTask.h"
#include "Config.h"

extern short isAllBoatVisible;

extern int strtoi(const char*str);
extern void lltostr(long ll,char * str);
extern void GUI_DispStringExAt(const unsigned char *str,const int x, const int y);

static  char cBuf[12];
static  char * pBuf = cBuf;

static GUI_RECT myRect = {347,5,600,673};

static const char * _aTable_1[][4] = {
  { "8.105", "17°", "412517000","渔船" },
  { "1", "5", "8","1" },
  { "2", "4", "4","2" },
  { "5", "1", "2" },
  { "3", "6", "6" },
  { "4", "7", "7" },
  { "623495-0010008", "8", " 8" },
  { "623496-0010008", "9", "Item 9" },
  { "623497-0010008", "10", "Item 10" },
  { "623498-0010008", "11", "Item 11" },
  { "623499-0010008", "12", "Item 12" },
  { "623410-0010008", "13", "Item 13" },
  { "623499-0010008", "14", "Item 14" },
  { "623499-0010008", "15", "Item 15" },
  { "623499-0010008", "16", "Item 16" },
  { "623499-0010008", "17", "Item 16" },
  { "623499-0010008", "18", "Item 16" },
  { "623499-0010008", "19", "Item 16" },
  { "623499-0010008", "20", "Item 16" },
  { "623499-0010008", "26", "Item 16" },
  { "623499-0010008", "37", "Item 16" },
  { "623499-0010008", "46", "Item 16" },
  { "623499-0010008", "56", "Item 16" },
  { "623499-0010008", "66", "Item 16" },
  { "623499-0010008", "76", "Item 16" },
  { "623499-0010008", "86", "Item 16" },
  { "623499-0010008", "96", "Item 16" },
  { "623499-0010008", "06", "Item 16" },
};

int    LEN=GUI_COUNTOF(_aTable_1);//需要添加的行数
int    Rowindex;//行数
int    RowNum;   //当前所选中的行数


static void showSelectedBoatInfo(WM_HWIN thisWin);
static void updateListViewContent(WM_HWIN _hListView);

static int BoatIndex  = 0;


static void myListViewCBFunc(WM_MESSAGE * pMsg)
{

	const WM_KEY_INFO * pInfo;
	WM_HWIN hListView = WM_GetDialogItem (pMsg->hWin, GUI_ID_LISTVIEW1);
	
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			LISTVIEW_Callback(pMsg);		
		break;
		case WM_KEY:
MYDEBUG("case key");		
		
			pInfo = (WM_KEY_INFO*)pMsg->Data.p;
MYDEBUG("key=%d",pInfo->Key);
			switch(pInfo->Key)
			{
				case GUI_KEY_UP:
MYDEBUG("case lv up");
				LISTVIEW_Callback(pMsg);
				showSelectedBoatInfo(pMsg->hWin);
				break;
				
				case GUI_KEY_DOWN:
MYDEBUG("case lv down");
				LISTVIEW_Callback(pMsg);
				showSelectedBoatInfo(pMsg->hWin);
				break;
				
				default:
					LISTVIEW_Callback(pMsg);
			}
						
		break;
			
				default:
					LISTVIEW_Callback(pMsg);
	}
}

static void initInfoText()
{
	
}

void InitLISTVIEW(WM_MESSAGE* pMsg) 
 {
		WM_HWIN hWin = pMsg->hWin;
		WM_HWIN _hListView;
		LISTVIEW_CreateEx (7, 106, 330, 369, hWin,WM_CF_SHOW|WM_CF_MEMDEV, 0, GUI_ID_LISTVIEW1);

		_hListView = WM_GetDialogItem (pMsg->hWin, GUI_ID_LISTVIEW1);  //获得LISTVIEW句柄
WM_SetCallback(_hListView,&myListViewCBFunc);
		WIDGET_SetEffect (_hListView, &WIDGET_Effect_None);  //设置小工具效果
		WM_SetFocus (_hListView);  //设置窗口焦点
		LISTVIEW_SetHeaderHeight (_hListView, 0);//标题高度
		LISTVIEW_SetGridVis (_hListView, 1);      //网格的可见性
		LISTVIEW_SetRowHeight (_hListView, 41);  //行高
		LISTVIEW_SetAutoScrollV ( _hListView, 0); //滚动条
		LISTVIEW_SetFont (_hListView, &GUI_Font28);  //设置字体

		LISTVIEW_SetTextColor (_hListView, LISTVIEW_CI_UNSEL,GUI_WHITE);    //未选中的文本颜色		
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_UNSEL,DEEPBLUE);       //未选中时单元格的背景颜色		
		LISTVIEW_SetTextColor (_hListView, LISTVIEW_CI_SELFOCUS,GUI_BLACK); //选中时的文本颜色
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_SELFOCUS,GUI_WHITE);   //选中时的单元格背景颜色
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_SEL ,DEEPBLUE);        //选定无焦点
		//添加列
		LISTVIEW_AddColumn (_hListView, 90, NULL,  GUI_TA_HCENTER|GUI_TA_VCENTER);
		LISTVIEW_AddColumn (_hListView, 70, NULL,  GUI_TA_HCENTER|GUI_TA_VCENTER);
		LISTVIEW_AddColumn (_hListView, 170,NULL,  GUI_TA_HCENTER|GUI_TA_VCENTER);
		
		updateListViewContent(hWin);

 }

 void addrow_LISTVIEW(WM_MESSAGE* pMsg) 
{
		WM_HWIN _hListView = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1);  //获得LISTVIEW句柄
		//添加行
		for (Rowindex = 0; Rowindex < GUI_COUNTOF (_aTable_1); Rowindex++)
				LISTVIEW_AddRow (_hListView, _aTable_1[Rowindex]);	
}

static void updateListViewContent(WM_HWIN thisWin)
{
	int i  = 0;
	
	unsigned int Counter    = 0;
	unsigned int NumOfRows  = 0;
	
	
	WM_HWIN hListView  = WM_GetDialogItem(thisWin,GUI_ID_LISTVIEW1);
	
MYDEBUG("hListView=%ld",hListView);	

	for(i=0;i<N_boat;i++)
	{
		if(isAllBoatVisible || test[i].isVisible )
		{
			Counter++;
			NumOfRows  = LISTVIEW_GetNumRows(hListView);
			
			if(Counter > NumOfRows)
			{
				
				//LISTVIEW_AddRow(hListView,NULL);
				LISTVIEW_AddRow(hListView,NULL);
NumOfRows  = LISTVIEW_GetNumRows(hListView);
MYDEBUG("num of rows=%d",NumOfRows);				
			}
			
MYDEBUG("%d",test[i].true_heading);			
			sprintf(pBuf,"%d",test[i].true_heading);
			LISTVIEW_SetItemText(hListView,1,Counter-1,pBuf);
			
MYDEBUG("%ld",test[i].user_id);			
			sprintf(pBuf,"%ld",test[i].user_id);
			LISTVIEW_SetItemText(hListView,2,Counter-1,pBuf);			
			
		}
	}
//NumOfRows  = LISTVIEW_GetNumRows(hListView);
//MYDEBUG("final num of rows=%d",NumOfRows);
	
}


static void showSelectedBoatInfo(WM_HWIN thisListView)
{
	int SelectedRow  = -2;
	int i  = 0;
	long SelectedID  = 0;
	
	
	char cBuf[10] ;
	


	SelectedRow  = LISTVIEW_GetSel(thisListView);
	
printf("SelectedRow=%d\r\n",SelectedRow);	
	
	if(SelectedRow >= 0)
	{
		LISTVIEW_GetItemText(thisListView,2,SelectedRow,cBuf,10);
printf("ItemText=\"%s\"\r\n",cBuf);		
		SelectedID  = strtoi(cBuf);
printf("SelectedID=%ld\r\n",SelectedID);	
		
		while((test[i].user_id!=SelectedID) && (i<N_boat))
		{
			i++;
		}
		/// Find the boat 
		if(i < N_boat)
		{	
       BoatIndex  = i;
MYDEBUG("find boat =%d ",i);			
		}

	}
	
	WM_InvalidateRect(hDlg_MonitorList,&myRect);
}

void _PaintFrame1(void) 
                                                                                                                                                                                                                                                                                                    {
	GUI_RECT r;
	WM_GetClientRect (&r);
	GUI_SetBkColor (GUI_LIGHTCYAN);
	GUI_SetColor (GUI_WHITE);
	GUI_SetTextMode (GUI_TM_TRANS);
	GUI_ClearRectEx (&r);
	GUI_SetColor (GUI_BLACK);
//	GUI_DispStringAt ("监控船舶列表gg:", 5, 5);
}

void _cbWindowMonitorList (WM_MESSAGE* pMsg) 
{
	int     NCode;
	int     Id;
	WM_HWIN hWin = pMsg->hWin;
  WM_HWIN _hListView ;

	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
 			WM_SetFocus (hWin);
		
			InitLISTVIEW (pMsg);    //初始化LISTVIEW 小工具
     _hListView  = WM_GetDialogItem(hWin,GUI_ID_LISTVIEW1);	
 //    updateMoniteLVContent(hWin);
		break;
		
		case WM_PAINT:
				GUI_SetBkColor (GUI_LIGHTBLUE);
				GUI_Clear();
				GUI_SetColor (DEEPBLUE);
				GUI_SetFont (&GUI_Font28);
				GUI_SetPenSize(2);
				GUI_FillRect(5,55,337,106);
				//listview 边框线
				GUI_SetColor (GUI_WHITE);
				GUI_DrawLine (5,55,5,475);  //左
				GUI_DrawLine (5,55,337,55);   //上1
				GUI_DrawLine (5,104,337,104); //上2
				GUI_DrawLine (337,55,337,475);  //右
				GUI_DrawLine (5,475,337,475);  //下
				GUI_DrawLine (96,55,96,105);
				GUI_DrawLine (167,55,167,105);		
		
				//右侧信息栏
				GUI_DrawLine (347,5,347,475);
				GUI_DrawLine (347,5,673,5);
				GUI_DrawLine (673,5,673,475);
				GUI_DrawLine (347,475,673,475);
				
				GUI_SetTextMode(GUI_TEXTMODE_TRANS);
				GUI_DispStringAt ("AIS监控船舶",18,18);
				GUI_DispStringAt ("距离",20,65);
				GUI_DispStringAt ("方位",99,65);
				GUI_DispStringAt ("MMSI",220,65);
				
				GUI_DispStringAt ("当前监控船舶信息",350,20);
				
				GUI_DispStringAt ("船名:",352,70);
				
			  
				
				GUI_DispStringAt ("北纬:",352,170);
				GUI_DispStringAt ("东经:",352,220);
				GUI_DispStringAt ("航速:",352,270);
				GUI_DispStringAt ("航向:",485,270);
				GUI_DispStringAt ("消失报警状态:",352,320);
				GUI_DispStringAt ("防盗报警范围:",352,370);
				GUI_DispStringAt ("走锚报警范围:",352,420);
				
				GUI_SetFont(&GUI_Font32_1);
				
				GUI_DispStringAt(test[BoatIndex].name,420,70);

        
				lltostr(test[BoatIndex].latitude,pBuf);
				GUI_DispStringExAt(pBuf,420,170);
		
        lltostr(test[BoatIndex].longitude,pBuf);
				GUI_DispStringExAt(pBuf,420,220);
	
		break;

		case WM_KEY:
			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
			{
				case GUI_KEY_F1:
					printf("monitorlist-->monitormap_F1 ");
					WM_BringToTop (hDlg_MonitorMap); //监控船舶渔区图
					WM_SetFocus (hDlg_MonitorMap);
					LISTVIEW_SetSel ( _hListView, 0 ); //选定第一行		
				break;

				case GUI_KEY_F2:
					printf("monitorlist-->alllist_F2 ");
					WM_BringToTop (hDlg_AllList);//所有船舶列表
					WM_SetFocus (hDlg_AllList);
					LISTVIEW_SetSel ( _hListView, 0 ); //选定第一行		
					dropdown0flag = 1;
					GUI_CURSOR_Hide();
				break;
				
//				case GUI_KEY_UP:
//printf("UP\r\n");					
// 					RowNum = LISTVIEW_GetSel ( _hListView );//返回当前所选的行数
//					if ( RowNum > 0 )			
//							RowNum--;
//					LISTVIEW_SetSel (_hListView, RowNum);	
//showSelectedBoatInfo(hWin);					
//				break;

//				case GUI_KEY_DOWN:
//printf("DOWN\r\n");					
//					RowNum = LISTVIEW_GetSel ( _hListView );//返回当前所选的行数
//					if ( RowNum < LEN )
//							RowNum++;
//					LISTVIEW_SetSel(_hListView, RowNum);
//showSelectedBoatInfo(hWin);				
//				break;

 				case GUI_KEY_MENU:
						WM_BringToTop (hDlg_FishMap);
						WM_SetFocus (hDlg_FishMap);
						menufocus = 368;
 				break;

				case GUI_KEY_LEFT:
					printf ("left");
					menufocus -= 5;
					focuschange = 0;
					WM_SetFocus(WM_GetDialogItem (hDlg_Menu, menufocus));
					WM_Paint(hDlg_Menu);
				break; 

				case GUI_KEY_RIGHT:
					RowNum = LISTVIEW_GetSel ( _hListView );//返回当前所选的行数
					if ( (LEN - RowNum ) > 15 )
							LISTVIEW_SetSel (_hListView, RowNum + 15 );
					else LISTVIEW_SetSel ( _hListView, LEN );
				break;
				
		}
		break;
		default:
		WM_DefaultProc(pMsg);
		break;
	}
}
