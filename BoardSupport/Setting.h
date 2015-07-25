#ifndef _SETTING_H
#define _SETTING_H


#include "boat_struct.h"



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
   boat * pBoat;
   char name[20];
   MNT_States  mntState;
   MNT_SETTING mntSetting;
   
};


int MNT_insert(MNT_BOAT * pMNT_Boat, boat * pBoat, long mmsi, char * name);


#endif

