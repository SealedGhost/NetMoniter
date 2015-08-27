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
#include "SystemConfig.h"

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

extern WM_HWIN btWin;
extern WM_HWIN menuWin;
extern WM_HWIN confirmWin;
extern WM_HWIN btWinCreate(void);

extern int N_boat;
extern short N_monitedBoat;
extern MNT_BOAT MNT_Boats[MNT_NUM_MAX];
extern  char* pStrBuf;
extern MNT_SETTING mntSetting;           // Struct contains etWin  content.

extern MNT_BERTH MNT_Berthes[MNT_NUM_MAX];
//extern MNT_BERTH * pMntHeader;
//extern MNT_BOAT mntBoats[MNT_BOAT_NUM_MAX];
extern CONF_SYS SysConf;

/*----------------- external functions --------------------------*/
extern int getSelectedBoatIndex(WM_HWIN thisHandle, int col, int row);


/*------------------ local variables ----------------------------*/



/*------------------ local functions -----------------------------*/
static void myListViewListener(WM_MESSAGE* pMsg);
static void updateListViewContent(WM_HWIN thisHandle);



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
	{ TEXT_CreateIndirect, "监控设置", ID_TEXT_0, LV_MoniteList_X,LV_MoniteList_Y-30, 200, 30, 0, 0x0, 0}

	
	
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
  
  case USER_MSG_BRING:
       updateListViewContent( WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0));
       break;
  
  case USER_MSG_SKIN:
       pLVSkin  = &(lvWinSkins[pMsg->Data.v]);
       
       WINDOW_SetBkColor(pMsg->hWin,pLVSkin->BackGround);
       
       
       
       hItem  = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, pLVSkin->LV_bkUnsel);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL,   pLVSkin->LV_bkSel);
       LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, pLVSkin->LV_bkFocus);
       
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL, pLVSkin->LV_tx_Unsel);
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,   pLVSkin->LV_tx_Sel);
       LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS, pLVSkin->LV_tx_Focus);
       
       hItem  = LISTVIEW_GetHeader(hItem);
       HEADER_SetBkColor(hItem,pLVSkin->LV_Header_Bk);
       HEADER_SetTextColor(hItem,pLVSkin->LV_Header_Text);
       break; 
  
  
  case WM_INIT_DIALOG:
    pLVSkin  = &(lvWinSkins[SysConf.Skin]);
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, pLVSkin->BackGround);
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	   WM_SetCallback(hItem, &myListViewListener);
	
    LISTVIEW_AddColumn(hItem, LV_MoniteSet_Col_0_WIDTH, "船名", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, LV_MoniteSet_Col_1_WIDTH, "MMSI", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, LV_MoniteSet_Col_2_WIDTH, "State", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    LISTVIEW_SetHeaderHeight(hItem,LV_MoniteList_Header_HEIGHT);
    LISTVIEW_SetRowHeight(hItem,LV_MoniteList_Row_HEIGHT);

    LISTVIEW_SetFont(hItem, &GUI_Font24_1);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, pLVSkin->LV_bkUnsel);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL,   pLVSkin->LV_bkSel);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, pLVSkin->LV_bkFocus);
    
    LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL, pLVSkin->LV_tx_Unsel);
    LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,   pLVSkin->LV_tx_Sel);
    LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS, pLVSkin->LV_tx_Focus);
    
    updateListViewContent(hItem);
    
    hItem  = LISTVIEW_GetHeader(hItem);
    HEADER_SetBkColor(hItem,pLVSkin->LV_Header_Bk);
    HEADER_SetTextColor(hItem,pLVSkin->LV_Header_Text);    

    // USER START (Optionally insert additional code for further widget initialization)
  
    // USER 
    break;
    
//  case WM_PAINT:
//       updateListViewContent(WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0));
//       break;
  
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
	 WM_MESSAGE myMsg;
 
  MNT_BERTH * pIterator  = NULL;
 
	 int selectedRow  = -1;
  int index  = -1;
	
	switch(pMsg->MsgId)
	{
		case WM_SET_FOCUS:

       if(LISTVIEW_GetNumRows(pMsg->hWin))
          LISTVIEW_SetSel(pMsg->hWin, 0);
          
       if(pMsg->Data.v)
       {
          myMsg.hWin  = menuWin;
          myMsg.hWinSrc  = thisListView;
          myMsg.MsgId  = USER_MSG_BRING;
          myMsg.Data.v  = 1;
          WM_SendMessage(myMsg.hWin, &myMsg);          
       }
  
       
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
					WM_SetFocus(btWin);
					break;
				
				
				case GUI_KEY_ENTER:
    
         selectedRow  = LISTVIEW_GetSel(thisListView);
         pIterator  = pMntHeader;
         for(index=0;index<selectedRow;index++)
         {
           pIterator  = pIterator->pNext;
         }
         
         switch(pIterator->chsState)
         {
            case MNTState_Choosen:
                 pIterator->chsState  = MNTState_Monited;
                 LISTVIEW_SetItemText(thisListView, 2, selectedRow, "N");
                 break;
            case MNTState_Default:
                 pIterator->chsState  = MNTState_None;
                 LISTVIEW_SetItemText(thisListView, 2, selectedRow, "N");
                 break;
//            case MNTState_Monited:
//                 pIterator->chsState  = MNTState_Choosen;
//                 LISTVIEW_SetItemText(thisListView, 2, selectedRow, "Y");
//                 break;
            case MNTState_None:
                 pIterator->chsState  = MNTState_Default;
                 LISTVIEW_SetItemText(thisListView, 2, selectedRow, "Y");
                 break;
            default:
                 pIterator->chsState  = MNTState_Choosen;
                 LISTVIEW_SetItemText(thisListView, 2, selectedRow, "Y");
                 break;
         }
   
			    		break;
					
				case GUI_KEY_BACKSPACE:   
         while( MNT_Berthes[index].chsState>=MNTState_Monited )
         {
            index++;
            if(index >= MNT_NUM_MAX)
               break;
         }
         if(index<MNT_NUM_MAX)
         {
         
            myMsg.hWin  = WM_GetClientWindow(confirmWin);
   //         myMsg.hWin  = confirmWin;
            myMsg.hWinSrc  = thisListView;
            myMsg.MsgId  = USER_MSG_ID_CHOOSE;
            myMsg.Data.v = STORE_SETTING;
            WM_SendMessage(myMsg.hWin, &myMsg);
                  
            WM_BringToTop(confirmWin);
            WM_SetFocus(confirmWin);
            break;	
         } 
         else
         {
INFO("No boat selected"); 
 
            WM_SetFocus(menuWin); 
            break;           
         } 
         
				
				case GUI_KEY_MENU:
					WM_BringToTop(hDlg_FishMap);
				  WM_SetFocus(hDlg_FishMap);
					break;
			
				default:
					LISTVIEW_Callback(pMsg);
					break;
			}
			break;
		
  case USER_MSG_ID_REPLY:
       switch(pMsg->Data.v)
       {
          case REPLY_OK:
               MNT_makeSettingUp(&mntSetting);        
               WM_SetFocus(menuWin);
               break;
          case REPLY_CANCEL:        
               WM_SetFocus(pMsg->hWin);
               break;
               
           default:
INFO("Something err!");           
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
static void updateListViewContent(WM_HWIN thisHandle)
{
	WM_HWIN thisListView  = thisHandle;
	
	int i  = 0;
 int Cnt = 0;
	int NumOfRows  = 0;
 MNT_BERTH * pIterator  = NULL;
// MNT_BERTH * pIterator  = pMntHeader;

	NumOfRows  = LISTVIEW_GetNumRows(thisListView);

// MNT_resetIterator();
 pIterator  = pMntHeader;
 while(pIterator)
 {

    Cnt++;  
    if(Cnt > NumOfRows)
    {
       LISTVIEW_AddRow(thisListView, NULL);
       NumOfRows  = LISTVIEW_GetNumRows(thisListView);
    }
    
    sprintf(pStrBuf, "%s", pIterator->mntBoat.name);
    LISTVIEW_SetItemText(thisListView, 0, Cnt-1, pStrBuf);
  
    sprintf(pStrBuf, "%09ld", pIterator->mntBoat.mmsi);
    LISTVIEW_SetItemText(thisListView, 1, Cnt-1, pStrBuf);
    
    LISTVIEW_SetItemText(thisListView, 2, Cnt-1, (MNTState_Default==pIterator->chsState)?"Y":"N");  
    
    pIterator  = pIterator->pNext;
 }
	while(NumOfRows > Cnt)
	{
		LISTVIEW_DeleteRow(thisListView, NumOfRows-1);
		NumOfRows  = LISTVIEW_GetNumRows(thisListView);
	}
}


//int getSelectedBoatIndex(WM_HWIN thisHandle, int col, int row)



/*************************** End of file ****************************/
