#ifndef MAINTASK_H
#define MAINTASK_H

#include "uart.h"
#include "Drawinformation.h"
#include "WinAllFishMap.h"
#include "WinAllList.h"
#include "WinMonitorList.h"
#include "WinMonitorMap.h"
#include "WinSetting.h"
#include "WinMonitSet.h"
#include "lpc177x_8x_uart.h"
#include "boat_struct.h"
#include "map_draw.h"
#include "string.h"
#include "Config.h"

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
extern char dropdown0flag;        //DROPDOWN小工具打开/关闭
extern int downlistfocus;        //DROPDOWN小工具的焦点设置
extern char SysSetOpenflag;            //判断设置界面是否在最顶层
extern char Doubleclick;
extern char MonitSetOpenflag;     //监控设置窗口打开标记变量
extern char Messageopenflag;      //确认提示窗口
extern char IsChange;
extern int menufocus;   //主菜单选择标记变量
extern char focuschange;   //系统设置界面里的焦点切换标记变量
extern int sysitem_ID;  //小工具ID号
extern char editchange;     //音量
extern char editcolor;
extern char radius;  //圆的半径
extern char focus_monitset;  //edit焦点切换标记变量

//extern char menu;       //判断焦点是否在菜单选择上
#endif
