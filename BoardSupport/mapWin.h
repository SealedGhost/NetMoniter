#ifndef _MAP_WIN_H
#define _MAP_WIN_H

#include "WM.h"
#include "boat_struct.h"

#define ID_TIMER_MAP_REFRESH  0x01
#define ID_TIMER_CURSOR       0x02


/*------------------------ Global variables extern ---------------------------*/
extern boat mothership;
extern char scale_choose;



/*------------------------ Global functions -----------------------------------*/
WM_HWIN mapWinCreate(void);


#endif
