#include "Setting.h"
#include "Config.h"

int N_monitedBoat  = 0;

/**@brief  MNT_insert
 *
 * @func   Put choosen boats on AllListView  into MNT_Boats[]
 * @input  pMNT_Boat  Store array(MNT_Boats[]).
 *         mmsi       Choosen boat's mmsi
           name       Choosen boat's name
 * @output 
 * @return The position index of boat been inserted.
 */
int MNT_insert(MNT_BOAT * pMNT_Boat, boat * pBoat, long mmsi, char * name)
{
   int i  = 0;
   int k  = 0; 
   for(i=0;i<MNT_NUM_MAX;i++)
   {
      if(pMNT_Boat[i].mmsi == 0)
      {
         pMNT_Boat[i].mmsi  = mmsi;
         pMNT_Boat[i].pBoat  = pBoat;
         pMNT_Boat[i].mntState  = MNTState_Default;
         for(k=0;k<20;k++)
         {
            pMNT_Boat[i].name[k]  = name[k];
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


