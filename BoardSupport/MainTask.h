#ifndef MAINTASK_H
#define MAINTASK_H

#include "uart.h"
#include "Drawinformation.h"
#include "WinAllFishMap.h"


#include "lpc177x_8x_uart.h"
#include "boat_struct.h"
#include "map_draw.h"
#include "string.h"
#include "Config.h"

#define USER_MSG_ID_CHOOSE (WM_USER + 0x01)
#define USER_MSG_ID_REPLY  (WM_USER + 0x02)
#define USER_MSG_SKIN      (WM_USER + 0x03)
#define USER_MSG_SHAPE     (WM_USER + 0x04)
#define USER_MSG_UNIT      (WM_USER + 0x05)
#define USER_MSG_BRING     (WM_USER + 0x06)

#define STORE_SETTING   1
#define CANCEL_MONITED  2
#define REPLY_OK        3
#define REPLY_CANCEL    4
#define ADD_MONITED     5
#define SYS_SETTING     6


extern WM_HWIN hDlg_FishMap;
extern WM_HWIN hDlg_MonitorMap;
extern WM_HWIN hDlg_AllList;
extern WM_HWIN hDlg_MonitorList;
extern WM_HWIN hDlg_SysSet;
extern WM_HWIN hDlg_MonitSet;
extern WM_HWIN hDlg_Messagebox;
extern WM_HWIN hDlg_Menu;

extern WM_HWIN    _hLastFrame;
extern GUI_FONT    SIF_Font;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font26;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font28;
extern void _DemoHardkey(void);
extern WM_HWIN _CreateFrame(WM_CALLBACK* cb);
void _cbWindowMenu(WM_MESSAGE * pMsg);
extern unsigned char acCompass;
extern  void _cbEditAddKey(EDIT_Handle hObj, int Key);

   


#define GUI_KEY_F1                40
#define GUI_KEY_F2                41
#define GUI_KEY_MENU              42
#define GUI_KEY_LARGE             43
#define GUI_KEY_REDUCE            44
#define GUI_KEY_MONITORING        45
#define GUI_KEY_CANCEL            46
#define DEEPBLUE                      0x9A2D11
#define LIGHTBLUE                 0xCB9537


//extern char menu;       //判断焦点是否在菜单选择上
#endif
