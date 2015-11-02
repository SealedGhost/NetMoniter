
#include "Check.h"
#include <math.h>
#include "sound.h"
#include "invader.h"
#include <ucos_ii.h>

/*----------------- Macro      defines --------------------------*/
#define MYABS(x)   ((x)>0?(x):(-(x)))

/*----------------- external variables ------------------------*/
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
extern int N_boat;
extern MNT_BERTH * pMntHeader;
extern MNT_BERTH MNT_Berthes[MNT_NUM_MAX];

/// Defined in app.c.If key pressed , isKeyTrigged will be TRUE. Your apps must set iskeyTrigged FALSe after using it.
extern int isKeyTrigged;
 
/*----------------- global   variables ------------------------*/
//INVADER Invaders[INVD_NUM_MAX];
//INVADER * pInvdHeader  = NULL;
//INVADER * pInvdTail    = NULL;
//INVADER InvdMaskCursor = {0};


/*-------------------- Local variables ------------------------*/
static uint8_t Numbers[3]   = {0};


MNT_BERTH NULL_Berth  = {0};

/*--------------------- Local functions -----------------------*/
static void MNT_filter(void);
static Bool CHECK_isDsp(MNT_BERTH * pMntBerth);
static Bool CHECK_isDrg(MNT_BERTH * pMntBerth);
static Bool CHECK_isBgl(MNT_BERTH * pMntBerth);
void CHECK_check(MNT_BERTH * pMntBerth);


void check()
{
   static int Cnt  = 0;
   static int Num  = 0;   
   
   int i  = 0;
   unsigned char trgState  = 0;
   

   MNT_BERTH * pMntBerth  = NULL;
   MNT_BERTH * pIterator  = NULL;
   
   /// Delete all MNTState_Delete .

   MNT_filter();
  
   if(isKeyTrigged)
   {
INFO("key trigged");   
      isKeyTrigged   = 0;
     
      /// 将所有触发态掩盖
      pIterator  = pMntHeader;
      while(pIterator)
      {
         if( (pIterator->trgState & 0x1f) == MNTState_Triggered )
         {
            pIterator->trgState  =  (pIterator->trgState & 0xe0) | MNTState_Masked;
         }
         
         pIterator  = pIterator->pNext;
      } 


       INVD_updataMask();
INFO("mask cursor moved");       
    }
  
  
  
/// 找寻本次应该check哪条监控船舶 
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
    
INFO("Check %09ld",pMntBerth->mntBoat.mmsi);     
     CHECK_check(pMntBerth); 
      
   }
   Cnt++;
   Cnt  = Cnt%2;
}



void CHECK_check(MNT_BERTH * pMntBerth)
{
   unsigned char trgState  = 0;
   
   if(CHECK_isDsp(pMntBerth))
   {
      INVD_clear(pMntBerth - MNT_Berthes);
      trgState  = 0x01<<7;
   }
   else if(CHECK_isDrg(pMntBerth))
   {
      INVD_clear(pMntBerth - MNT_Berthes);
      trgState  = 0x01<<6;
   }
   else if(CHECK_isBgl(pMntBerth))
   {
      trgState  = 0x01<<5;
   }
   
   if(trgState  == 0)
   {
      pMntBerth->trgState  = MNTState_None;
      return ;
   }
   else if( (trgState ^ pMntBerth->trgState) & trgState )
   {
      pMntBerth->trgState  = (trgState & 0xe0) | MNTState_Triggered;
   }
   
   if( (pMntBerth->trgState & 0x1f)  ==  MNTState_Triggered)
   {
      Numbers[0]  = (pMntBerth->mntBoat.mmsi%1000) / 100;
      Numbers[1]  = (pMntBerth->mntBoat.mmsi%100) / 10;
      Numbers[2]  = (pMntBerth->mntBoat.mmsi%10);
INFO("Num:%d,%d,%d",Numbers[0], Numbers[1], Numbers[2]);      
      switch(trgState)
      {
        case (0x01<<7):
             if(pMntBerth->mntBoat.mntSetting.DSP_Setting.isEnable)
             {
                SND_SelectID(SND_ID_DSP);

                OSTimeDlyHMSM(0, 0, 3, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[0]);
                OSTimeDlyHMSM(0, 0, 1, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[1]);
                OSTimeDlyHMSM(0, 0, 1, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[2]);
   printf("\a DSP\n");                 
             }
             break;
        case (0x01<<6):
             if(pMntBerth->mntBoat.mntSetting.DRG_Setting.isEnable) 
             {
                SND_SelectID(SND_ID_DRG);
                
                OSTimeDlyHMSM(0, 0, 3, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[0]);
                OSTimeDlyHMSM(0, 0, 1, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[1]);
                OSTimeDlyHMSM(0, 0, 1, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[2]);
   printf("\a DRG\n");                 
             }
             break;
        case (0x01<<5):
             if(pMntBerth->mntBoat.mntSetting.BGL_Setting.isEnable)
             {
                SND_SelectID(SND_ID_BGL);
                
                OSTimeDlyHMSM(0, 0, 3, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[0]);
                OSTimeDlyHMSM(0, 0, 1, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[1]);
                OSTimeDlyHMSM(0, 0, 1, 0);
                SND_SelectID(SND_ID_NUM_BASE+Numbers[2]);                
   printf("\a BGL\n");                 
             }
             break;
        default:
   printf("trgState Err\n");          
             break;
      }
   }
}


static Bool CHECK_isDsp(MNT_BERTH * pMntBerth)
{
   int i;
//INFO("DSP check");   
   for(i=N_boat-1; i>=0; i--)
   {
      /// This boat had been gon but come back now.
      if(SimpBerthes[i].pBerth->Boat.user_id == pMntBerth->mntBoat.mmsi)
      {
         if(SimpBerthes[i].pBerth->mntState == MNTState_None)
         {
INFO("Oh my god , come back");         
            pMntBerth->pBoat  = &(SimpBerthes[i].pBerth->Boat);
            SimpBerthes[i].pBerth->mntState  = MNTState_Monited;
            if(pMntBerth->chsState == MNTState_Init)
            {
               pMntBerth->chsState  = MNTState_Monited;
               pMntBerth->trgState  = MNTState_None;
            }
         }
         return FALSE;
      }
    
   }
   
   if(pMntBerth->chsState == MNTState_Init)
   {
      return FALSE;
   }
   
   if(pMntBerth->pBoat)
   {
      pMntBerth->pBoat  = NULL;   
INVD_printf();      
   }
   
   INFO("This boy is gone:%09ld", pMntBerth->mntBoat.mmsi);   
   return TRUE;
}



static Bool CHECK_isDrg(MNT_BERTH * pMntBerth)
{
   long diff_lon;
   long diff_lat;
   float r  = 0.0;
//INFO("DRG check");   
   if(pMntBerth->pBoat == NULL  ||  pMntBerth->pBoat->dist  > 100000)
   {
 
      return FALSE;
   }  

   diff_lon  = pMntBerth->pBoat->longitude - pMntBerth->mntBoat.lg;
   diff_lat  = pMntBerth->pBoat->latitude  - pMntBerth->mntBoat.lt;
   
   diff_lon  = MYABS(diff_lon);
   diff_lat  = MYABS(diff_lat);
   
   ///  先判断是否在走锚报警区域的内切正方形内，若是就不必勾股了，不是的话再老老实实sqrt吧。
   if(  diff_lon + 2*diff_lon/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist
      ||diff_lat + 2*diff_lat/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
   {
      r  = sqrt(diff_lon*diff_lon + diff_lat*diff_lat);
   }
   
   if(r > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}



static Bool CHECK_isBgl(MNT_BERTH * pMntBerth)
{
   int i ;
//INFO("BGL check");   

   if(pMntBerth->pBoat == NULL  ||  pMntBerth->pBoat->dist  > 100000)
   {
 
      return FALSE;
   } 

   for(i=N_boat-1; i>=0; i--)
   {
      if(SimpBerthes[i].pBerth->mntState == MNTState_None)
      {
         /// Is invader
         if(  (MYABS(SimpBerthes[i].longitude - pMntBerth->pBoat->longitude) <= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist)
            &&(MYABS(SimpBerthes[i].latitude  - pMntBerth->pBoat->latitude)  <= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist))
         {
INFO("find invader");         
            if(INVD_add(pMntBerth-MNT_Berthes, SimpBerthes[i].pBerth))
               SimpBerthes[i].pBerth->isInvader  = pMntBerth->mntBoat.mmsi;        
         }
         
         /// Is not invader but has been.So remove it from black list.
         else if(SimpBerthes[i].pBerth->isInvader == pMntBerth->mntBoat.mmsi)
         {
INFO("black -> white");         
            INVD_delete(pMntBerth-MNT_Berthes, SimpBerthes[i].pBerth);  
            SimpBerthes[i].pBerth->isInvader  = 0;            
         }
      }
   }
   INVD_printf();
   if(INVD_isAllMasked(pMntBerth-MNT_Berthes) == FALSE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

   





/**@brief MNT_filter
 *
 * @dscp Delete mntBoat whose state is MNTState_Delete;
 *       Find all disappear mntBoat.
 *
 */
static void MNT_filter()
{
   MNT_BERTH * pIterator  = NULL;
   MNT_BERTH * pBC        = NULL;

   
   while(pMntHeader && pMntHeader->chsState == MNTState_Delete)
   {
INFO("delete at header");   
      pBC  = pMntHeader;
      pMntHeader  = pMntHeader->pNext;

      INVD_clear(pBC-MNT_Berthes);     
    
      EEPROM_Write(0 , MNT_PAGE_ID+pBC-MNT_Berthes,
                  &(NULL_Berth.mntBoat), MODE_8_BIT, sizeof(MNT_BOAT));
      memset((void*)pBC, 0, sizeof(MNT_BERTH));    
   }
  
   
   if(pMntHeader == NULL)
   {
      return;
   }
   
   if(pMntHeader->pBoat  &&  pMntHeader->pBoat->user_id != pMntHeader->mntBoat.mmsi)
   {
      if(pMntHeader->chsState != MNTState_Init)
      {
         if(pMntHeader->pBoat && pMntHeader->pBoat->user_id != pMntHeader->mntBoat.mmsi)
         {
INFO("filte find dsp %p",pIterator->pBoat);    
            pMntHeader->pBoat  = NULL;   
            INVD_clear(pMntHeader-MNT_Berthes);  
         }
      }   
   }
   
   pBC        = pMntHeader;
   pIterator  = pMntHeader->pNext; 
   while(pIterator)
   {
      if(pIterator->chsState == MNTState_Delete)
      {
INFO("delete at body");      
         pBC->pNext  = pIterator->pNext;

         INVD_clear(pIterator-MNT_Berthes); 
        
         EEPROM_Write(0 , MNT_PAGE_ID+pIterator-MNT_Berthes,
                      &(NULL_Berth.mntBoat), MODE_8_BIT, sizeof(MNT_BOAT));         
         memset((void*)pIterator, 0, sizeof(MNT_BERTH));
         

      }
      else
      {     
         if(pMntHeader->pBoat  &&  pIterator->pBoat->user_id != pIterator->mntBoat.mmsi)
         {
            if(pIterator->chsState != MNTState_Init)
            {
               if(pIterator->pBoat && pIterator->pBoat->user_id != pIterator->mntBoat.mmsi)
               {
INFO("filte find dsp %p",pIterator->pBoat);    
                  pIterator->pBoat  = NULL;   
                  INVD_clear(pIterator-MNT_Berthes);  
               }

            } 

          }              
          pBC  = pIterator;        
      }
      
      pIterator  = pBC->pNext;
      /// Find all disappear mntBoat
//      if(pIterator->pBoat->user_id != pIterator->mntBoat.mmsi)
//      if(pIterator->chsState != MNTState_Init)
//      {
//         if(pIterator->pBoat && pIterator->pBoat->user_id != pIterator->mntBoat.mmsi)
//         {
//INFO("filte find dsp %p",pIterator->pBoat);    
//            pIterator->pBoat  = NULL;   
//            INVD_clear(pIterator-MNT_Berthes);  
//         }

//      }
   } 
   
//   if(pIterator->chsState != MNTState_Init)
//   {
//      if(pIterator->pBoat && pIterator->pBoat->user_id != pIterator->mntBoat.mmsi)
//      {
//INFO("filte find dsp %p",pIterator->pBoat);    
//         pIterator->pBoat  = NULL;   
//         INVD_clear(pIterator-MNT_Berthes);      
//      }

//   }
   
//printf("after filte\n");   
//INVD_printf();
}


