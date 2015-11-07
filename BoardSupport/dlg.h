#ifndef _DLG_H
#define _DLG_H
#include "WM.h"
extern WM_HWIN mapWin;
extern WM_HWIN menuWin;
extern WM_HWIN subWins[4];
extern WM_HWIN confirmWin;
extern WM_HWIN mntSettingWin;
extern WM_HWIN _mntSettingWin;
extern WM_HWIN _sldWin;

WM_HWIN mapWinCreate(void);
WM_HWIN menuWinCreate(void);
WM_HWIN confirmWinCreate(void);
WM_HWIN mntSettingWinCreate(void);
WM_HWIN _mntSettingWinCreate(void);
WM_HWIN sub0WinCreate(void);
WM_HWIN sub1WinCreate(void);
WM_HWIN sub2WinCreate(void);
WM_HWIN sub3WinCreate(void);
WM_HWIN _sub3WinCreate(void);

#endif

