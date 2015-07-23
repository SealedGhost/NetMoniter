#include "Setting.h"
#include "Config.h"

int N_monitedBoat  = 0;


int MNT_insert(MNT_BOAT * pMNT_Boat, long mmsi, char * name)
{
   int i  = 0;
   int k  = 0;
   for(i=0;i<MNT_NUM_MAX;i++)
   {
      if(pMNT_Boat[i].mmsi == 0)
      {
         pMNT_Boat[i].mmsi  = mmsi;
         pMNT_Boat[i].mntState  = MNTState_Default;
         for(k=0;k<20;k++)
         {
            pMNT_Boat[k].name[k]  = name[k];
            if(name[k] == '\0')
            {
               break;
            }
         }
         pMNT_Boat[i].name[19]  = '\0';
         N_monitedBoat++;
         return i;
      }
   }
   
   return -1;
}


