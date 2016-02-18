#ifndef _SETTING_H
#define _SETTING_H

#include "Config.h"
#include "boat_struct.h"
#include "lpc_types.h"


typedef struct _DSP_SETTING DSP_SETTING;
struct _DSP_SETTING
{
   unsigned char isEnable;
};

typedef struct _BGL_SETTING BGL_SETTING;
struct _BGL_SETTING
{
   unsigned char isEnable;
   unsigned char isSndEnable;
   unsigned int  Dist;
};

typedef struct _DRG_SETTING DRG_SETTING;
struct _DRG_SETTING
{
   unsigned char isEnable;
   unsigned char isSndEnable;
   unsigned int  Dist;
};

typedef struct _MNT_SETTING MNT_SETTING;
struct _MNT_SETTING
{
   DSP_SETTING DSP_Setting;
   BGL_SETTING BGL_Setting;
   DRG_SETTING DRG_Setting;
};

typedef struct _MNT_BOAT MNT_BOAT;
struct _MNT_BOAT
{
   long mmsi;
   long lg;
   long lt;
   char name[20]; 
  MNT_SETTING mntSetting;   
};

typedef struct _MNT_BERTH MNT_BERTH;
struct _MNT_BERTH
{
  MNT_BOAT mntBoat;
  long          snapLg;
  long          snapLt;
  int           snapDist;
  unsigned char chsState;
  unsigned char trgState;
  unsigned char cfgState;
  unsigned char flsState;
           char nickName[3];
  BERTH * pBerth;  
  MNT_BERTH * pNext;
};

extern MNT_BERTH MNT_Berthes[MNT_NUM_MAX];
extern MNT_BERTH * pMntHeader;
extern MNT_SETTING mntSetting; 

int  MNT_getAddrOffset(uint8_t * addr);
Bool MNT_add(BERTH * pBerth);
void MNT_load(void);
void MNT_storeBoatInfo(MNT_BERTH* pMntBerth);
int  MNT_makeSettingUp(MNT_SETTING * pMNT_Setting);
void printSetting(MNT_SETTING * p_setting);
void MNT_printSetting(void);void MNT_DumpSetting(MNT_BERTH * pBerth);
Bool MNT_removeById(long Id);
int  MNT_getDefaultNum(void);
void MNT_Disable(void);
void MNT_Enable(void);
MNT_BERTH* MNT_fetchNextPlayBerth(void);
MNT_BERTH* MNT_fetchPlayBerth(void);
void MNT_snapOnMiss(BERTH* pBerth);


//__inline void MNT_resetIterator()
//{
//   Next  = pMntHeader;
//}
#endif

