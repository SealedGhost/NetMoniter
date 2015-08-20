#ifndef _SETTING_H
#define _SETTING_H


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
   boat * pBoat;   
   char name[20];   
};

typedef struct _MNT_BERTH MNT_BERTH;
struct _MNT_BERTH
{
  MNT_BOAT mntBoat;
  MNT_SETTING mntSetting;
  unsigned char chsState;
  unsigned char trgState;
  MNT_BERTH * pNext;
};


//static MNT_BERTH * pMntHeader  = NULL;
//static MNT_BERTH * Next  = NULL;




int MNT_insert(MNT_BOAT * pMNT_Boat, boat * pBoat, long mmsi, char * name);
void MNT_makeSettingUp(MNT_SETTING * pMNT_Setting);
int MNT_deleteByIndex(MNT_BOAT * pMNT_Boat,int index, long id);
int MNT_deleteById(MNT_BOAT * pMNT_Boat, long id);
void MNT_init(MNT_SETTING * pMntSetting);
void printMoniteSetting(MNT_BOAT * pMNT_Boat);
void printSetting(MNT_SETTING * p_setting);
void MNT_printSetting(void);
Bool MNT_add(boat * pBoat);
Bool MNT_removeById(long Id);
void MNT_testReset(void);
//void MNT_resetIterator(void);
MNT_BERTH * MNT_getNext(void);

//__inline void MNT_resetIterator()
//{
//   Next  = pMntHeader;
//}
#endif

