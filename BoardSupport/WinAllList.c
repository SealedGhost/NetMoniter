#include "MainTask.h"

extern int isAllBoatVisible;
extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];



static  char cBuf[12];
static  char * pBuf = cBuf;


static WM_HTIMER hTimer;
static int v;
static const char * _aTable_2[][4] = {
  { "8.105", "17°", "是" },
  { "1", "522222222","否" },
  { "2", "4", "4","2" },
  { "5", "1", "2" },
  { "3", "6", "6" },
  { "4", "7", "7" },
};

static int BoatIndex  = 0;

static GUI_RECT myRect = {347,5,600,673};
static GUI_RECT lvRect  = {7, 106, 320, 369};


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
		LISTVIEW_GetItemText(thisListView,1,SelectedRow,cBuf,10);
printf("ItemText=\"%s\"\r\n",cBuf);		
		SelectedID  = strtoi(cBuf);
printf("SelectedID=%ld\r\n",SelectedID);	
		
		while((boat_list_p[i]->user_id!=SelectedID) && (i<N_boat))
		{
			i++;
		}
		/// Find the boat 
		if(i < N_boat)
		{	
       BoatIndex  = i;

		}

	}
	
	WM_InvalidateRect(hDlg_AllList,&myRect);
}


static void myListViewCBFunc(WM_MESSAGE * pMsg)
{

	const WM_KEY_INFO * pInfo;
	WM_HWIN hListView = WM_GetDialogItem (pMsg->hWin, GUI_ID_LISTVIEW0);
	
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
				showSelectedBoatInfo(pMsg->hWin);
				break;
				
				case GUI_KEY_DOWN:

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

static void updateListViewContent(WM_HWIN thisWin)
{
	int i  = 0;
	
	unsigned int Counter    = 0;
	unsigned int NumOfRows  = 0;
	
	
	WM_HWIN hListView  = WM_GetDialogItem(thisWin,GUI_ID_LISTVIEW0);
	
	for(i=0;i<N_boat;i++)
	{

			NumOfRows  = LISTVIEW_GetNumRows(hListView);
			
		///  如果当前LISTVIEW中的行数不够用来显示则添加行
			if(i+1 > NumOfRows)
			{
				
				//LISTVIEW_AddRow(hListView,NULL);
				LISTVIEW_AddRow(hListView,NULL); 
NumOfRows  = LISTVIEW_GetNumRows(hListView);			
			}
				
			sprintf(pBuf,"%ld",boat_list_p[i]->user_id);
			LISTVIEW_SetItemText(hListView,1,i,pBuf);	
			
			if(boat_list_p[i]->isVisible)
				LISTVIEW_SetItemText(hListView,2,i,"是");
			else
				LISTVIEW_SetItemText(hListView,2,i,"否");
	}
	
	/// 如果LISTVIEW中的行数有剩余则删除多余的行条目
	while(NumOfRows > i+1)
	{
		LISTVIEW_DeleteRow(hListView,i);
		NumOfRows  = LISTVIEW_GetNumRows(hListView);		
	}
		
}





 void InitLISTVIEW_AllList (WM_MESSAGE* pMsg) 
 {
		WM_HWIN hWin = pMsg->hWin;
		WM_HWIN _hListView;
		LISTVIEW_CreateEx (7, 106, 320, 369, hWin,WM_CF_SHOW|WM_CF_MEMDEV, 0, GUI_ID_LISTVIEW0);
		_hListView=WM_GetDialogItem (pMsg->hWin, GUI_ID_LISTVIEW0);  //获得LISTVIEW句柄
	 
WM_SetCallback(_hListView,&myListViewCBFunc);	 
		WIDGET_SetEffect (_hListView, &WIDGET_Effect_None);  //设置小工具效果
		LISTVIEW_SetFont(_hListView,&GUI_Font28);
		WM_SetFocus (_hListView);  //设置窗口焦点
		LISTVIEW_SetHeaderHeight (_hListView, 0);//标题高度
		LISTVIEW_SetGridVis (_hListView, 1);      //网格的可见性

		LISTVIEW_SetRowHeight (_hListView, 41);  //行高
		LISTVIEW_SetAutoScrollV ( _hListView, 0 ); //滚动条
		LISTVIEW_SetTextColor (_hListView, LISTVIEW_CI_SELFOCUS,GUI_BLACK); //选中时的文本颜色
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_SELFOCUS,GUI_WHITE);   //选中时的单元格背景颜色
		LISTVIEW_SetTextColor (_hListView, LISTVIEW_CI_UNSEL,GUI_WHITE);    //未选中的文本颜色		
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_UNSEL,DEEPBLUE);       //未选中时单元格的背景颜色		
		LISTVIEW_SetBkColor (_hListView, LISTVIEW_CI_SEL ,DEEPBLUE);        //选定无焦点	 
		//添加列
		LISTVIEW_AddColumn (_hListView, 90, NULL,  GUI_TA_HCENTER|GUI_TA_VCENTER);
		LISTVIEW_AddColumn (_hListView, 150, NULL,  GUI_TA_HCENTER|GUI_TA_VCENTER);
		LISTVIEW_AddColumn (_hListView, 80, NULL,  GUI_TA_HCENTER|GUI_TA_VCENTER);
		//排序
//	  LISTVIEW_SetCompareFunc(_hListView,0,LISTVIEW_CompareDec);
//	  LISTVIEW_SetSort(_hListView,0,1);
//		LISTVIEW_EnableSort(_hListView);
		updateListViewContent(hWin);
		
 }
 //
 //添加行列表数据
 //
void addrow_LISTVIEW_AllList_1(WM_MESSAGE* pMsg) 
	{
		int i ;
		WM_HWIN _hListView = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);  //获得LISTVIEW句柄
		for (i = 0; i< 5; i++)
		LISTVIEW_AddRow(_hListView,  _aTable_2[i]);	
}



void _cbWindowAllList(WM_MESSAGE* pMsg) 
{
	WM_HWIN hWin = pMsg->hWin;
	WM_HWIN _button0;
	WM_HWIN _button1;
	WM_HWIN _button2;
	WM_HWIN _button3;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			WM_SetFocus (hWin);
			InitLISTVIEW_AllList (pMsg);    //初始化LISTVIEW 小工具
//			addrow_LISTVIEW_AllList_1 (pMsg); //添加行数据	
//  hTimer  = WM_CreateTimer(pMsg->hWin, 1, 1000, 0);		
		break;
		
//		case WM_TIMER:
//MYDEBUG("entry timer and refresh alllist");		
//			WM_InvalidateRect(pMsg->hWin,&myRect);

//		 WM_RestartTimer(hTimer, 1000);
//	
//			break;
		
		case WM_PAINT:
				updateListViewContent(hWin);
		
				GUI_SetBkColor (GUI_LIGHTBLUE);
				GUI_Clear();
				GUI_SetColor (DEEPBLUE);
				GUI_SetFont (&GUI_Font28);
				GUI_SetPenSize(2);
				GUI_FillRect(5,55,327,106);
				//listview 边框线				
				GUI_SetColor (GUI_WHITE);
				GUI_DrawLine (5,55,5,475);  //左
				GUI_DrawLine (5,55,327,55);   //上1
				GUI_DrawLine (5,104,327,104); //上2
				GUI_DrawLine (327,55,327,475);  //右
				GUI_DrawLine (5,475,327,475);  //下
				GUI_DrawLine (97,60,97,102);
				GUI_DrawLine (248,60,248,102);
				
				//右侧信息栏
				GUI_DrawLine (337,5,337,475);
				GUI_DrawLine (337,5,673,5);
				GUI_DrawLine (673,5,673,475);
				GUI_DrawLine (337,475,673,475);
				GUI_SetTextMode(GUI_TEXTMODE_TRANS);
				GUI_DispStringAt ("AIS船舶列表",18,18);
				GUI_DispStringAt ("距离",20,66);
				GUI_DispStringAt ("MMSI",150,66);
				GUI_DispStringAt ("监控",260,66);
				GUI_SetFont (&GUI_Font28);
				GUI_DispStringAt ("当前船舶信息",346,20);
				GUI_DispStringAt ("船名:",346,70);
				GUI_DispStringAt ("北纬:",346,170);
				GUI_DispStringAt ("东经:",346,220);
				GUI_DispStringAt ("航速:",346,270);
				GUI_DispStringAt ("航向:",510,270);
				GUI_DispStringAt ("提示:按“监控”键对网位仪",346,350);
				GUI_DispStringAt ("进行监控",346,380);
				
				GUI_SetFont(&GUI_Font32_1);
				
				GUI_DispStringAt(test[BoatIndex].name,420,70);
        
				lltostr(boat_list_p[BoatIndex]->latitude,pBuf);
				GUI_DispStringExAt(pBuf,420,170);		
        lltostr(boat_list_p[BoatIndex]->longitude,pBuf);
				GUI_DispStringExAt(pBuf,420,220);
				
				WM_InvalidateWindow(hWin);
		break;				

		break;

		case WM_KEY:
 			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
 			{
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
		WM_DefaultProc (pMsg);
		break;
	}
}
