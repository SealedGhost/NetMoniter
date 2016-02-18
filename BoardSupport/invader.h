#ifndef _INVADER_H
#define _INVADER_H

#include "Config.h"
#include "lpc_types.h"
#include "boat_struct.h"



typedef struct _INVADER  INVADER;
struct _INVADER 
{
   BERTH  * pBerth;
   INVADER * pNext;
};



Bool INVD_add(int row,  BERTH * pBerth);

void INVD_delete(int row, BERTH * pBerth);

void INVD_clear(int row);

Bool INVD_isAllMasked(int row);
Bool INVD_isAlone(int row);

void INVD_updataMask(void);

//void INVD_printf(void);

void INVD_deleteByAddr(BERTH * pBerth);

#endif

