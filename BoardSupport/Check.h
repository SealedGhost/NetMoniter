#ifndef _CHECK_H
#define _CHECK_H

#include "Config.h"
#include "MainTask.h"
#include "Setting.h"

typedef struct _INVADER  INVADER;
struct _INVADER 
{
   long MMSI;
   INVADER * pNext;
};



extern INVADER Invaders[INVD_NUM_MAX];
extern INVADER * pInvdHeader;


Bool isDisappear(MNT_BOAT * pMntBoat);
void check(MNT_BOAT * pMntBoat);

int INVD_add(long mmsi);
Bool INVD_deleteById(long mmsi);

#endif