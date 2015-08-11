
#include "Check.h"
#include <math.h>
/*----------------- Macro      defines --------------------------*/
#define MYABS(x)      ( (x)>0?(x):-(x) )


/*----------------- external variables ------------------------*/
extern SIMP_BERTH SimpBerthes[BOAT_LIST_SIZE_MAX];
extern MNT_BOAT   MNT_Boats[MNT_NUM_MAX];
extern int N_boat;

/*----------------- global   variables ------------------------*/
INVADER Invaders[INVD_NUM_MAX];
INVADER * pInvdHeader  = NULL;

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
   unsigned char isGood = 0;
   unsigned char trgState  = 0;
   float r   = 0.0;
   long diff_lt  = 0;
   long diff_lg  = 0;
   
   /*****************************************************************************
   
                                      DSP check
   
   ******************************************************************************/   
   if(pMntBoat->mntSetting.DSP_Setting.isEnable)
   {
INFO("N_boat:%d",N_boat);
      trgState  |= (0x01<<7);
      for(i=0;i<N_boat;i++)
      {
         if(SimpBerthes[i].pBoat->user_id == pMntBoat->mmsi)
         {
            trgState  &= (~(0x01<<7));
            if(pMntBoat->mntState != MNTState_Default)
               pMntBoat->mntState  = MNTState_Monited;
               
            if(pMntBoat->pBoat != SimpBerthes[i].pBoat)
            {
               pMntBoat->pBoat  = SimpBerthes[i].pBoat;
               SimpBerthes[i].pBoat->mntStates  = MNTState_Monited;
            }
            break;
         }
         else
         {
            ;
         }
      }
      
      if(trgState )
      {
         pMntBoat->pBoat  = NULL;
INFO("\aThis boy is gone%09ld",pMntBoat->mmsi);         
      }
//      pMntBoat->mntState  = MNTState_Triggered;
   }
   
   /*****************************************************************************
   
                                      BGL check
   
   ******************************************************************************/   
   if(pMntBoat->mntSetting.BGL_Setting.isEnable && (trgState&(0x01<<7)) == 0 )
   {  
      for(i=0;i<N_boat;i++)
      {      
         /// Closing
         if (    ( (MYABS(SimpBerthes[i].latitude - pMntBoat->pBoat->latitude)) <= pMntBoat->mntSetting.BGL_Setting.Dist) 
             &&  ( (MYABS(SimpBerthes[i].longitude - pMntBoat->pBoat->longitude)) <= pMntBoat->mntSetting.BGL_Setting.Dist )   )
         {
            /// Invader
            if(SimpBerthes[i].pBoat->mntStates == MNTState_None)
            {            
INFO("\a %09ld Closing:%09ld",SimpBerthes[i].pBoat->user_id,pMntBoat->mmsi);            
               trgState  |= (0x01<<6);
               pMntBoat->mntState  = MNTState_Triggered;
               SimpBerthes[i].pBoat->isInvader  = pMntBoat->mmsi;
               INVD_add(SimpBerthes[i].pBoat->user_id);
            }
            else
            {
               SimpBerthes[i].pBoat->isInvader  = 0;
            }
         }
         /// Not closing 
         else
         {
            /// blace  --> white.
            if(SimpBerthes[i].pBoat->isInvader == pMntBoat->mmsi)
            {
               SimpBerthes[i].pBoat->isInvader  = 0;
               INVD_deleteById(SimpBerthes[i].pBoat->user_id);
            }
         }
      }
   }
   
   
   
   /*****************************************************************************
   
                                      DRG check 
                                      
   ******************************************************************************/ 
   if(pMntBoat->mntSetting.DRG_Setting.isEnable && (trgState&(0x01<<7)) == 0)
   {
      diff_lt  = pMntBoat->pBoat->latitude - pMntBoat->lt;
      diff_lg  = pMntBoat->pBoat->longitude - pMntBoat->lg;
      
      diff_lt  = MYABS(diff_lt);
      diff_lg  = MYABS(diff_lg);
      
      if( (diff_lt+2*diff_lt/5 > pMntBoat->mntSetting.DRG_Setting.Dist)
         ||(diff_lg+2*diff_lg/5 > pMntBoat->mntSetting.DRG_Setting.Dist) )
       {
           r  = sqrt(diff_lt*diff_lt + diff_lg*diff_lg);
       } 

      
       if(r >= pMntBoat->mntSetting.DRG_Setting.Dist) 
       {
          trgState  |= (0x01<<5);
          pMntBoat->mntState  = MNTState_Triggered;
INFO( "\aThis boy offset %09ld",pMntBoat->mmsi );   
       } 
       
       else
       {
          pMntBoat->mntState  = MNTState_Monited;
       }       
   }
   
   
   pMntBoat->mntState  &= 0x1f;
   pMntBoat->mntState  |= trgState;
   
//   pMntBoat->mntState  |= isTrigged;
}

/**@brief  INVD_allocOneInvader
 *
 * @dscr   Find an element in Invaders[] to place a invader.
 *
 *         @input  :  
 *         @return : The pointer pointing to address of alloced element.
 *                   If Invaders is full ,return will be NULL.
 */

INVADER * INVD_allocOneInvader()
{
   int i  = 0;
   for(i=0;i<INVD_NUM_MAX;i++)
   {
      if(Invaders[i].MMSI == 0)
      {
         return &(Invaders[i]);
      }
   }
   return NULL;
}


/**@brief INVD_deleteById
 *
 * @dscr  Delete one by MMSI.
 *
 *        @input  : The MMSI 
 *        @return : If delete successfully return TRUE or FALSE.
 */
Bool INVD_deleteById(long mmsi)
{
   INVADER * pIterator  = NULL;
   INVADER * pBC        = NULL;   
   
   if(pInvdHeader->MMSI == mmsi)
   {
      pBC  = pInvdHeader;
      pInvdHeader  = pInvdHeader->pNext;
      memset((void*)pBC, 0, sizeof(INVADER));
      return TRUE;
   }
   
   pBC  = pInvdHeader;
   pIterator  = pInvdHeader->pNext;
   
   while(pIterator)
   {
      if(pIterator->MMSI == mmsi)
      {
         pBC->pNext  = pIterator->pNext;
         memset((void*)pIterator, 0, sizeof(INVADER));
         return TRUE;
      }
      else
      {
         pBC  = pIterator;
         pIterator  = pIterator->pNext;
      }
   }
   
   return FALSE;
}



int INVD_add(long mmsi)
{
   INVADER * buf  = NULL;
   INVADER * pIterator  = NULL;
   

   
   if(pInvdHeader != NULL)
   {
      pIterator  = pInvdHeader;
      while(pIterator->pNext)
      {
         /// Find mmsi 
         if(pIterator->MMSI == mmsi)
         {
             return 0;
         }
         pIterator  = pIterator->pNext;
      }
      if(pIterator->MMSI == mmsi)
      {
         return 0;
      }
      
      /// Add 
      buf  = INVD_allocOneInvader();
      if(buf == NULL)
      {
INFO("alloc invd failed!"); 
          return  -1;  
      }      
      buf->MMSI = mmsi;  
      
      pIterator->pNext  = buf;
      return 1;
   }  
   else
   {
      buf  = INVD_allocOneInvader();
      if(buf == NULL)
      {
INFO("alloc invd failed!"); 
          return  -1;  
      }      
      buf->MMSI = mmsi;   
      pInvdHeader  = buf;
      return 1;
   }
   
   
   
   
   
}