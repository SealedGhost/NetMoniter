/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.24                          *
*        Compiled Jan 27 2014, 11:28:24                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "Config.h"
#include "MainTask.h"
#include "Setting.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_LISTVIEW_0   (GUI_ID_USER + 0x01)
#define ID_TEXT_0       (GUI_ID_USER + 0x02)

/*------------------ external variables --------------------*/
extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];

extern WM_HWIN etWin;
extern WM_HWIN menuWin;
extern WM_HWIN confirmWin;
extern WM_HWIN etWinCreate(void);

extern short N_boat;
extern short N_monitedBoat;
extern MNT_BOAT MNT_Boats[MNT_NUM_MAX];
extern unsigned char* pStrBuf;

//extern MNT_BOAT mntBoats[MNT_BOAT_NUM_MAX];

/*----------------- external functions --------------------------*/
extern int getSelectedBoatIndex(WM_HWIN thisHandle, int col, int row);


/*------------------ local variables ----------------------------*/



/*------------------ local functions -----------------------------*/
static void myListViewListener(WM_MESSAGE* pMsg);
static void UpdateListViewContent(WM_HWIN thisHandle);



// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, SubWin_X, SubWin_Y, SubWin_WIDTH, SubWin_HEIGHT, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, LV_MoniteSet_X, LV_MoniteSet_Y, LV_MoniteSet_WIDTH, LV_MoniteSet_HEIGHT, 0, 0x0, 0 },	
	{ TEXT_CreateIndirect, "Monite Set", ID_TEXT_0, LV_MoniteList_X,LV_MoniteList_Y-30, 200, 30, 0, 0x0, 0}

	
	
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x000080FF);
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	   WM_SetCallback(hItem, &myListViewListener);
	
    LISTVIEW_AddColumn(hItem, LV_MoniteSet_Col_0_WIDTH, "Name", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, LV_MoniteSet_Col_1_WIDTH, "MMSI", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, LV_MoniteSet_Col_2_WIDTH, "State", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    LISTVIEW_SetHeaderHeight(hItem,LV_MoniteList_Header_HEIGHT);
    LISTVIEW_SetRowHeight(hItem,LV_MoniteList_Row_HEIGHT);

    LISTVIEW_SetFont(hItem, &GUI_Font24B_1);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL,GUI_WHITE);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL,DEEPBLUE);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SELFOCUS,GUI_BLACK);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL, DEEPBLUE);
    UpdateListViewContent(hItem);
    // USER START (Optionally insert additional code for further widget initialization)
	  	hItem  = etWinCreate();
    // USER 
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN sub1WinCreate(void);
WM_HWIN sub1WinCreate(void) {
  WM_HWIN hWin;
		

//  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, menuWin, 0, 0);
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END



/**监控设置 LISTVIEW 的监听器（回调函数）
*
*/
static void myListViewListener(WM_MESSAGE* pMsg)
{
	 const WM_KEY_INFO * pInfo;
	 WM_HWIN thisListView  = pMsg->hWin; 
	
	 int selectedRow  = -1;
  int index  = -1;
	
	switch(pMsg->MsgId)
	{
		case WM_SET_FOCUS:

      if(LISTVIEW_GetNumRows(pMsg->hWin) && (LISTVIEW_GetSel(pMsg->hWin)<0))
         LISTVIEW_SetSel(pMsg->hWin, 0);
      UpdateListViewContent(thisListView);
      LISTVIEW_Callback(pMsg);
      break;		
    case WM_KEY:
			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
		
		  switch(pInfo->Key)
			{
				case GUI_KEY_UP:
				case GUI_KEY_DOWN:
					LISTVIEW_Callback(pMsg);
     
					break;
				
				case GUI_KEY_LEFT:				
					WM_SetFocus(menuWin);
					break;
				
				case GUI_KEY_RIGHT:
					WM_SetFocus(etWin);
					break;
				
				
				case GUI_KEY_ENTER:
         index  = LISTVIEW_GetSel(thisListView);
         
         if(index >= 0)
         {
           switch(MNT_Boats[index].mntState)
           {
              case MNTState_Choosen:          
                   MNT_Boats[index].mntState  = MNTState_Monited;
                   LISTVIEW_SetItemText(thisListView, 2, index, "N");
                   break;
              case MNTState_Default:  
                   MNT_Boats[index].mntState  = MNTState_None;
                   LISTVIEW_SetItemText(thisListView, 2, index, "N");
                   break;
              case MNTState_Monited:
                   MNT_Boats[index].mntState  = MNTState_Choosen;
                   LISTVIEW_SetItemText(thisListView, 2, index, "Y");
                   break;
              case MNTState_None:
                   MNT_Boats[index].mntState  = MNTState_Default;
                   LISTVIEW_SetItemText(thisListView, 2, index, "Y");
                   break;
               default:      
                   break;
           }
           LISTVIEW_Callback(pMsg);
         }   
			    		break;
					
				case GUI_KEY_BACKSPACE:   
					WM_BringToTop(confirmWin);
				  WM_SetFocus(confirmWin);
          break;					
				
				case GUI_KEY_MENU:
					WM_BringToTop(hDlg_FishMap);
				  WM_SetFocus(hDlg_FishMap);
					break;
			
				default:
					LISTVIEW_Callback(pMsg);
					break;
			}
			break;
		
		default:
			LISTVIEW_Callback(pMsg);
			break;
	}
}



/* 更新监控设置的 LISTVIEW 条目
*
*/
static void UpdateListViewContent(WM_HWIN thisHandle)
{
	WM_HWIN thisListView  = thisHandle;
	
	int i  = 0;
	int NumOfRows  = 0;

	NumOfRows  = LISTVIEW_GetNumRows(thisListView);
 
 for(i=0;i<N_monitedBoat;i++)
 {
    if(i+1 > NumOfRows)
    {
       LISTVIEW_AddRow(thisListView, NULL);
       NumOfRows  = LISTVIEW_GetNumRows(thisListView);
    }
INFO("\r\nmnt name:%s",MNT_Boats[i].name);    
    sprintf(pStrBuf, "%s", MNT_Boats[i].name);
    LISTVIEW_SetItemText(thisListView, 0, i, pStrBuf);
    
    sprintf(pStrBuf, "%09ld", MNT_Boats[i].mmsi);
    LISTVIEW_SetItemText(thisListView, 1, i, pStrBuf);
    
    if(MNTState_Default == MNT_Boats[i].mntState)
    {
       LISTVIEW_SetItemText(thisListView, 2, i, "Y");
    }
    else
    {
       LISTVIEW_SetItemText(thisListView, 2, i, "N");
    }
 }
	while(NumOfRows > N_monitedBoat)
	{
		LISTVIEW_DeleteRow(thisListView, NumOfRows-1);
		NumOfRows  = LISTVIEW_GetNumRows(thisListView);
	}
}


//int getSelectedBoatIndex(WM_HWIN thisHandle, int col, int row)

/*************************** End of file ****************************/
