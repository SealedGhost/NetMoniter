#ifndef _DLG_H
#define _DLG_H

extern WM_HWIN mapWin;
extern WM_HWIN menuWin;
extern WM_HWIN subWins[4];
extern WM_HWIN confirmWin;
extern WM_HWIN mntSettingWin;

WM_HWIN mapWinCreate(void);
WM_HWIN menuWinCreate(void);
WM_HWIN confirmWinCreate(void);
WM_HWIN mntSettingWinCreate(void);
WM_HWIN sub0WinCreate(void);
WM_HWIN sub1WinCreate(void);
WM_HWIN sub2WinCreate(void);
WM_HWIN sub3WinCreate(void);

#endif