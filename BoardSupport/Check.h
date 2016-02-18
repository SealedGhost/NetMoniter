#ifndef _CHECK_H
#define _CHECK_H

#include "Config.h"
#include "MainTask.h"
#include "Setting.h"

//typedef struct _INVADER  INVADER;
//struct _INVADER 
//{
//   long MMSI;
//   long targetMMSI;
//   INVADER * pNext;
//};



//extern INVADER Invaders[INVD_NUM_MAX];
//extern INVADER * pInvdHeader;


//Bool isDisappear(MNT_BOAT * pMntBoat);
void check(void);
void CHECK_checkNickName(MNT_BERTH* pMntBerth);

//int INVD_add(long MMSI,long targetMMSI);
//Bool INVD_deleteByMMSI(long MMSI);
//static void INVD_deleteByTargetMMSI(long targetMMSI);
//static void INVD_print(void);

#endif

