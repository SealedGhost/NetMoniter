#ifndef _SNAP_H
#define _SNAP_H

#include "Setting.h"

long SNAP_getSnapObjMMSI(void);
void SNAP_reset(void);
void SNAP_getPara(long* pLg, long* pLt);
void SNAP_searchNearestObj(short Dir_x, short Dir_y);
#endif


