
#include "Check.h"
#include <math.h>
/*----------------- Macro      defines --------------------------*/
#define MYABS(x)   ((x)>0?(x):(-(x)))

/*----------------- external variables ------------------------*/
extern SIMP_BERTH SimpBerthes[BOAT_LIST_SIZE_MAX];
extern MNT_BOAT   MNT_Boats[MNT_NUM_MAX];
extern int N_boat;
extern MNT_BERTH * pMntHeader;

/// If key pressed , isKeyTrigged will be TRUE. Your apps must set iskeyTrigged FALSe after using it.
extern int isKeyTrigged;
 
/*----------------- global   variables ------------------------*/
INVADER Invaders[INVD_NUM_MAX];
INVADER * pInvdHeader  = NULL;
INVADER * pInvdTail    = NULL;
INVADER InvdMaskCursor = {0};


/*--------------------- Local functions -----------------------*/



float UnitScale  = 1.0;

  
static Bool removeById(long Id);
static void func(void);

void check()
{
   int i  = 0;
   unsigned char isGood = 0;
   unsigned char trgState  = 0;
   float r   = 0.0;
   long diff_lt  = 0;
   long diff_lg  = 0;
   MNT_BERTH * pMntBerth  = NULL;
   static int Cnt  = 0;
   static int Num  = 0;
   
   int isDSP  = 0;
   int isBGL  = 0;
   int isDRG  = 0;
   
   /// Delete all MNTState_Delete .

   func();
  
   if(pMntHeader == NULL)
   {
      return;
   }
   
  
   pMntBerth  = pMntHeader;
   
   if(Cnt==0)
   {   
      for(i=Num;i>0;i--)
      {
         if(pMntBerth->pNext)
         {
            pMntBerth  = pMntBerth->pNext;
         }
         else
         {
            pMntBerth  = pMntHeader;
            Num  = 0;
         }
      }
      
      Num++;

INFO("check %09ld",pMntBerth->mntBoat.mmsi);   

   
  /*****************************************************************************
   
                                      DSP check
   
   ******************************************************************************/   
   isDSP  = 1;
   for(i=N_boat-1; i>=0; i--)   
   {  
      /// Not disappear.
      if(SimpBerthes[i].pBoat->user_id == pMntBerth->mntBoat.mmsi)
      {
         isDSP  = 0;
         /// This boat had been gone but come back now.      
         if(SimpBerthes[i].pBoat->mntStates == MNTState_None)
         {
INFO("come back");            
            pMntBerth->mntBoat.pBoat  = SimpBerthes[i].pBoat;
            SimpBerthes[i].pBoat->mntStates  = MNTState_Monited;
            
            if(pMntBerth->mntBoat.mntState != MNTState_Default)
            {
               pMntBerth->mntBoat.mntState  = MNTState_Monited;
            }
         }
         
         break;
      }
   }
   /// Disappear.
   if(isDSP)
   {
      pMntBerth->mntBoat.pBoat  = NULL;
INFO("\a This boy is gone %09ld",pMntBerth->mntBoat.mmsi);      
   }
  
   /*****************************************************************************
   
                                      BGL check
   
   ******************************************************************************/      
   if(   pMntBerth->mntBoat.mntSetting.BGL_Setting.isEnable  
      && isDSP==FALSE 
      && pMntBerth->mntBoat.pBoat->latitude  )
   {
      for(i=N_boat-1; i>=0; i--)
      {
         if(  SimpBerthes[i].pBoat->mntStates == MNTState_None )
         {
            /// Closing.
            if(   (MYABS(SimpBerthes[i].latitude - pMntBerth->mntBoat.pBoat->latitude) <= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist)
               && (MYABS(SimpBerthes[i].longitude- pMntBerth->mntBoat.pBoat->longitude)<= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist)  )
            {
//INFO("\a%09ld invader %09ld", SimpBerthes[i].pBoat->user_id, pMntBerth->mntBoat.mmsi);            
               isBGL  = TRUE;
               SimpBerthes[i].pBoat->isInvader  = pMntBerth->mntBoat.mmsi;
               INVD_add(SimpBerthes[i].pBoat->user_id, pMntBerth->mntBoat.mmsi);
            }
            else
            {          
               /// Black --> Whiit.
               if(SimpBerthes[i].pBoat->isInvader  == pMntBerth->mntBoat.mmsi)
               {
INFO("%09ld --> White",SimpBerthes[i].pBoat->user_id);                
                  /// INVD_delete.
                  SimpBerthes[i].pBoat->isInvader  = 0;
                  INVD_deleteByMMSI(SimpBerthes[i].pBoat->user_id);
               }
            }
         }
         else
         {
        
            /// INVD_delete.
            if(SimpBerthes[i].pBoat->isInvader)
            {
INFO("invader %09ld become mnt",SimpBerthes[i].pBoat->user_id);  
              INVD_deleteByMMSI(SimpBerthes[i].pBoat->user_id);           
              SimpBerthes[i].pBoat->isInvader  = 0;
            }
         }
      }
   }
   else
   {
INVD_deleteByTargetMMSI(pMntBerth->mntBoat.mmsi);   
      for(i=N_boat-1; i>=0; i--)
      {
         if(SimpBerthes[i].pBoat->isInvader == pMntBerth->mntBoat.mmsi)
         {
INFO("clear follower:%09ld",SimpBerthes[i].pBoat->user_id);         
            SimpBerthes[i].pBoat->isInvader  = 0;
         }
      }
   }
   
 
   /*****************************************************************************
   
                                      DRG check 
                                      
   ******************************************************************************/   
   if(   pMntBerth->mntBoat.mntSetting.DRG_Setting.isEnable  
      && isDSP==FALSE 
      && pMntBerth->mntBoat.pBoat->latitude  )
   {
      diff_lt  = pMntBerth->mntBoat.pBoat->latitude - pMntBerth->mntBoat.lt;
      diff_lg  = pMntBerth->mntBoat.pBoat->longitude- pMntBerth->mntBoat.lg;
      
      diff_lt  = MYABS(diff_lt);
      diff_lg  = MYABS(diff_lg);
      
      if(   diff_lt+2*diff_lt/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist
          ||diff_lg+2*diff_lg/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
      {
         r  = sqrt(diff_lt*diff_lt + diff_lg*diff_lg);
      }
      
      if(r >= pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
      {
INFO("\aThis boy offset");      
         isDRG  = TRUE;
      }
   }
   
   if(isKeyTrigged)
   {
      isKeyTrigged   = 0;
    
      InvdMaskCursor.pNext  = pInvdTail;
      if(pInvdTail)
      {
         InvdMaskCursor.MMSI  = pInvdTail->MMSI;
      }
   }
   
   INVD_print();
   
 }
   Cnt++;
   Cnt  = Cnt%2;
   
   

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


int INVD_add(long MMSI, long targetMMSI)
{
   INVADER * buf  = NULL;
   INVADER * pIterator  = NULL;
   

//INFO("invd add");
   if(pInvdHeader != NULL)
   {
      pIterator  = pInvdHeader;
//      while(pIterator->pNext)
       while(pIterator != pInvdTail)
      {
         /// Find mmsi 
         if(pIterator->MMSI == MMSI)
         {
             pIterator->targetMMSI  = targetMMSI;
             return 0;
         }
         pIterator  = pIterator->pNext;
      }
//      if(pIterator->MMSI == MMSI)
      if(pInvdTail->MMSI == MMSI)
      {
         pIterator->targetMMSI  = targetMMSI;
         return 0;
      }
      
      /// Add at new tail.
      buf  = INVD_allocOneInvader();
      if(buf == NULL)
      {
INFO("alloc invd failed!"); 
          return  -1;  
      }      
      buf->MMSI = MMSI;  
      buf->targetMMSI  = targetMMSI;
//      pIterator->targetMMSI  = targetMMSI;
//      pIterator->pNext  = buf;
      pInvdTail->pNext  = buf;
      pInvdTail  = buf;
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
      buf->MMSI = MMSI;  
      buf->targetMMSI  = targetMMSI;    
      pInvdHeader  = buf;
      pInvdTail  = buf;
      
      InvdMaskCursor.MMSI   = 0;
      InvdMaskCursor.pNext  = NULL;
      return 1;
   } 
}


/**@brief INVD_deleteById
 *
 * @dscr  Delete one by MMSI.
 *
 *        @input  : The MMSI 
 *        @return : If delete successfully return TRUE or FALSE.
 */
Bool INVD_deleteByMMSI(long mmsi) 
{
   INVADER * pIterator  = NULL;
   INVADER * pBC        = NULL;
  
   if(pInvdHeader == NULL)
   {
      return FALSE;
   }
   pIterator  = pInvdHeader;
   
   /// 头节点要删除
   if(pInvdHeader->MMSI == mmsi)
   {
INFO("invd delete");    
      pInvdHeader  = pInvdHeader->pNext;
      /// 头节点和尾节点相同，即链表中只有一个节点。
      if(pIterator == pInvdTail)
      {
         pInvdTail  = NULL;
      }
      /// 头节点为掩码节点
      if(pIterator == InvdMaskCursor.pNext)
      {
         InvdMaskCursor.pNext  = NULL;
         InvdMaskCursor.MMSI  = 0;
      }
      
      memset((void*)pIterator, 0, sizeof(INVADER));
      return TRUE;
   }
   else
   {
      while(pIterator != pInvdTail)
      {
         if(pIterator->pNext->MMSI == mmsi)
         {
INFO("invd delete");          
            pBC  = pIterator->pNext;
            pIterator->pNext  = pBC->pNext;
            
            if(pBC == pInvdTail)
            {
               pInvdTail  = pIterator;
            }
            if(pBC == InvdMaskCursor.pNext)
            {
               InvdMaskCursor.pNext  = pIterator;
               InvdMaskCursor.MMSI   = pIterator->MMSI;
            }
            
            memset((void*)pBC, 0, sizeof(pBC));
            return TRUE;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
      return FALSE;
   }
}



/** @brief INVD_deleteByTargetMMSI
 *
 *  @dscrp Delete all invaderes which of target equal targetMMSI.   
 *
 */
 static void INVD_deleteByTargetMMSI(long targetMMSI)
{
   INVADER * pIterator  = NULL;
   INVADER * pBC        = NULL;
   
   
   while(pInvdHeader && pInvdHeader->targetMMSI==targetMMSI)
   {
      if(pInvdHeader == pInvdTail)
      {
         pInvdTail  = NULL;
      }
      if(pInvdHeader == InvdMaskCursor.pNext)
      {
         InvdMaskCursor.pNext  = NULL;
         InvdMaskCursor.MMSI   = 0;
      }
      pBC  = pInvdHeader;
      pInvdHeader  = pInvdHeader->pNext;  
INFO("invader %09ld delete,target:%09ld",pBC->MMSI,pBC->targetMMSI);      
      memset((void*)pBC, 0, sizeof(INVADER));
   }
   
   
   if(pInvdHeader)
   {
      pIterator  = pInvdHeader;
	  
      while( pIterator && (pIterator!=pInvdTail) )
      {
         if(pIterator->pNext->MMSI == targetMMSI)
         {
             pBC  = pIterator->pNext;
             pIterator->pNext  = pBC->pNext;
             
             if(pBC == pInvdTail)
             {
                pInvdTail  = pIterator;
             }
             if(pBC  == InvdMaskCursor.pNext)
             {
                InvdMaskCursor.pNext  = pIterator;
                InvdMaskCursor.MMSI   = pIterator->MMSI;
             }
INFO("invader %09ld delete,target:%09ld",pBC->MMSI,pBC->targetMMSI);             
             memset((void*)pBC, 0, sizeof(pBC));
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
   }
}
 
 
 
 /*
static void INVD_deleteByTargetMMSI(long targetMMSI)
{
   INVADER * pIterator  = NULL;
   INVADER * pBC        = NULL;

   
   while(pInvdHeader && pInvdHeader->targetMMSI==targetMMSI)
   {
      if(pInvdHeader == pInvdTail)
      {
         pInvdTail  = NULL;
      }
      if(pInvdHeader == InvdMaskCursor.pNext)
      {
         InvdMaskCursor.pNext  = NULL;
         InvdMaskCursor.MMSI   = 0;
      }
      pBC  = pInvdHeader;
      pInvdHeader  = pInvdHeader->pNext;  
INFO("invader %09ld delete,target:%09ld",pBC->MMSI,pBC->targetMMSI);      
      memset((void*)pBC, 0, sizeof(INVADER));
   }
   
   
   if(pInvdHeader)
   {
      pIterator  = pInvdHeader;
      pBC        = pInvdHeader->pNext;
      
      while(pBC)
      {
         if(pBC->targetMMSI == targetMMSI)
         {
            if(InvdMaskCursor.pNext == pBC)
            {
               InvdMaskCursor.pNext  = pIterator;
               InvdMaskCursor.MMSI   = pIterator->MMSI;
            }
            pIterator->pNext  = pBC->pNext;
INFO("invader %09ld delete,target:%09ld",pBC->MMSI,pBC->targetMMSI);             
            memset((void*)pBC, 0, sizeof(INVADER));
            pBC  = pIterator->pNext;
         }
         else
         {
            pIterator  = pInvdHeader->pNext;
            pBC        = pBC->pNext;
         }
      }
   }
}
*/






static Bool removeById(long Id)
{
   MNT_BERTH * pIterator  = NULL;
   MNT_BERTH * pBC        = NULL;
   
   if(pMntHeader->mntBoat.mmsi == Id)
   {
      pBC  = pMntHeader;
      pMntHeader  = pMntHeader->pNext;

      memset((void*)pBC, 0, sizeof(MNT_BERTH));
      return TRUE;
   }
   else
   {
      pIterator = pMntHeader;
      pBC  = pMntHeader->pNext;
      while(pBC)
      {
         if(pBC->mntBoat.mmsi == Id)
         {
            pIterator->pNext  = pBC->pNext;
            memset((void*)pBC, 0, sizeof(MNT_BERTH));
            return TRUE;
         }
         else
         {
            pIterator  = pBC;
            pBC  = pBC->pNext;
         }
      }
      return FALSE;
   }
}








static void func()
{
   MNT_BERTH * pIterator  = NULL;
   MNT_BERTH * pBC        = NULL;
   int i  = 0;
   
   while(pMntHeader && pMntHeader->mntBoat.mntState == MNTState_Delete)
   {
INFO("delete at header");   
      pBC  = pMntHeader;
      pMntHeader  = pMntHeader->pNext;

      for(i=N_boat-1;i>=0;i--)
      {
         if(SimpBerthes[i].pBoat->isInvader == pBC->mntBoat.mmsi)
            SimpBerthes[i].pBoat->isInvader  = 0;
      } 
      INVD_deleteByTargetMMSI(pBC->mntBoat.mmsi);      
      memset((void*)pBC, 0, sizeof(MNT_BERTH)); 
   }
   
   if(pMntHeader == NULL)
   {
      return;
   }
   
   pIterator  = pMntHeader;
   pBC        = pMntHeader->pNext; 
   while(pBC)
   {
      if(pBC->mntBoat.mntState == MNTState_Delete)
      {
INFO("delete at body");      
         pIterator->pNext  = pBC->pNext;
         for(i=N_boat-1;i>=0;i--)
         {
            if(SimpBerthes[i].pBoat->isInvader == pBC->mntBoat.mmsi)
               SimpBerthes[i].pBoat->isInvader  = 0;
         } 
         INVD_deleteByTargetMMSI(pBC->mntBoat.mmsi);  
         memset((void*)pBC, 0, sizeof(MNT_BERTH));
      }
      else
      {
        pIterator  = pIterator->pNext;
      }
      
      pBC  = pIterator->pNext;
   }
   
}




static void INVD_print()
{
   INVADER * pIterator  = NULL;
   int i  = 0;
   pIterator  = pInvdHeader;
   
   while(pIterator)
   {
      if( (i%5) == 0 )
         printf("\n\r");
      if(pIterator == InvdMaskCursor.pNext)
         printf("**|%09ld|**",pIterator->MMSI);
      else
         printf("-- %09ld --",pIterator->MMSI);
      i++;
      pIterator  = pIterator->pNext;
   }
   printf("\n\r");
}