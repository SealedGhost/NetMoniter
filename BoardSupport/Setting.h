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
   short         Dist;
};

typedef struct _DRG_SETTING DRG_SETTING;
struct _DRG_SETTING
{
   unsigned char isEnable;
   unsigned char isSndEnable;
   short         Dist;
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

  unsigned char chsState;
  unsigned char trgState;
  boat * pBoat;  
  MNT_BERTH * pNext;
};


extern MNT_BERTH * pMntHeader;
extern MNT_SETTING mntSetting; 

int MNT_getAddrOffset(uint8_t * addr);
Bool MNT_add(boat * pBoat);
void MNT_initSetting(void);
void MNT_load(void);
int MNT_makeSettingUp(MNT_SETTING * pMNT_Setting);
void MNT_printSetting(void);
Bool MNT_removeById(long Id);


//__inline void MNT_resetIterator()
//{
//   Next  = pMntHeader;
//}
#endif

