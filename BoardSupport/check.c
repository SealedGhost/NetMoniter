
#include "Check.h"
#include <math.h>
/*----------------- Macro      defines --------------------------*/
#define ABS(x)      ( (x)>0?(x):-(x) )


/*----------------- external variables ------------------------*/
extern SIMP_BERTH SimpBerthes[BOAT_LIST_SIZE_MAX];
extern MNT_BOAT   MNT_Boats[MNT_NUM_MAX];

/*----------------- global   variables ------------------------*/
float UnitScale  = 1.0;


//boat* BKList[MNT_NUM_MAX][20];


//void gg(double x, double y)
//{
//   double res  = 0;
//   
//   res  = sqrt(x*x + y*y);
//}


//Bool isDisappear(MNT_BOAT * pMntBoat)
//{
//   int i  = 0;
//   for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
//   {   
//      if(SimpBerthes[i].pBoat->user_id == pMntBoat->mmsi)
//         return FALSE;
//   }
//   
//   return TRUE;
//}

void check(MNT_BOAT * pMntBoat)
{
   int i  = 0;
   char isDSP  = 0;
   char isInvd  = 0;
   char isDRG  = 0;
//   long lt0  = 0;
//   long lg0  = 0;
//   long lt  = 0;
//   long lg  = 0;
   float r   = 0.0;
   long diff_lt  = 0;
   long diff_lg  = 0;
   
   isDSP  = 1;
   for(i=0;i<BOAT_LIST_SIZE_MAX;i++)
   {
/// DSP 
      if( (SimpBerthes[i].pBoat->user_id == pMntBoat->mmsi) && isDSP)
      {
         isDSP  = 0; 
      }
      
///DGL
      /// Closing
      if (    ( (ABS(SimpBerthes[i].latitude - pMntBoat->pBoat->latitude)) <= pMntBoat->mntSetting.BGL_Setting.Dist) 
          &&  ( (ABS(SimpBerthes[i].longitude - pMntBoat->pBoat->longitude)) <= pMntBoat->mntSetting.BGL_Setting.Dist )   )
      {
         /// Invader
         if(SimpBerthes[i].pBoat->mntStates == MNTState_None)
         {  
            if(SimpBerthes[i].pBoat->isInvader != pMntBoat->mmsi)
            {
               SimpBerthes[i].pBoat->isInvader  = pMntBoat->mmsi;
            }
            isInvd  = 1;
         }
         else
         {
//            return FALSE;
         }
      }
      
      /// Not closing 
      else
      {
         if(SimpBerthes[i].pBoat->isInvader == pMntBoat->mmsi)
         {
            SimpBerthes[i].pBoat->isInvader  = 0;
         }
//         return FALSE; 
      }
///DRG
//     lt0  = pMntBoat->lt;
//     lg0  = pMntBoat->lg;
//     lt   = pMntBoat->pBoat->latitude;
//     lg   = pMntBoat->pBoat->longitude;
     diff_lt  = pMntBoat->pBoat->latitude - pMntBoat->lt;
     diff_lg  = pMntBoat->pBoat->longitude - pMntBoat->lg;
     
     
     if(  2*(ABS(diff_lt)/3 >= pMntBoat->mntSetting.DRG_Setting.Dist)
        ||(2*ABS(diff_lg)/3 >= pMntBoat->mntSetting.DRG_Setting.Dist)  )  
     {
//        r  = sqrt( (diff_lt+diff_lg)*(diff_lt+diff_lg) -2*diff_lt*diff_lg );
         r  = sqrt(diff_lt*diff_lt + diff_lg*diff_lg);
     }     
     
   }
   
   if(isDSP)
   {
INFO("FBI warning: %ld DSP",pMntBoat->mmsi);   
   }
   if(isInvd)
   {
INFO("invade %ld happend!",pMntBoat->mmsi);   
   }
   
   if(r >= pMntBoat->mntSetting.DRG_Setting.Dist)
   {
INFO( "This boy offset " );   
   }
}