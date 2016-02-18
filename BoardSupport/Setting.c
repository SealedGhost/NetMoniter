#include "Setting.h"
#include <string.h>
#include "sysConf.h"
#include "invader.h"



#define MNT_BERTH_SIZE  sizeof(MNT_BERTH)


extern int getSphereDist(long lg_1,long lt_1, long lg_2, long lt_2);
extern boat mothership;

MNT_BERTH MNT_Berthes[MNT_NUM_MAX];


MNT_BERTH * pMntHeader  = NULL;

static MNT_BERTH* pPlayBerth  = NULL;

//MNT_SETTING mntSetting; 


/** @brief  MNT_addrCalculate
 *
 *  @dscrp  Calculate flash address based on memory address.
 *  @input  Address in memory.
 *  @output 
 *  @return Address in flash.
 */
int MNT_getAddrOffset(uint8_t * addr)
{
   int offset  = 0;
   
   offset  = addr - (uint8_t*)MNT_Berthes;
   return offset;
}


void MNT_storeBoatInfo(MNT_BERTH* pMntBerth)
{
    EEPROM_Write(0, MNT_PAGE_ID+(pMntBerth-MNT_Berthes),
                   &(pMntBerth->mntBoat), MODE_8_BIT, sizeof(MNT_BOAT));
    
//             EEPROM_Write( 0, MNT_PAGE_ID+(pIterator-MNT_Berthes),
//                       &(pIterator->mntBoat),MODE_8_BIT,sizeof(MNT_BOAT));  
}



/**@brief  MNT_makeSettingUp
 * 
 * @dscrp  
 * @input 
 *         
 *         
 * @Output 
 * @return 
 */
int MNT_makeSettingUp (MNT_SETTING * pMNT_Setting)
{
   int Cnt  = 0;
   MNT_BERTH * pIterator  = pMntHeader;
   
   if(pMNT_Setting->BGL_Setting.Dist == 0)
   {
      pMNT_Setting->BGL_Setting.isEnable  = DISABLE;
   }
   
   if(pMNT_Setting->DRG_Setting.Dist == 0)
   {
      pMNT_Setting->DRG_Setting.isEnable  = DISABLE;
   }
   
   while(pIterator)
   {    
      if(   (pIterator->chsState == MNTState_Choosen)
          ||(pIterator->chsState == MNTState_Default)  )
      {
         pIterator->chsState  = MNTState_Monitored;
         pIterator->trgState  = MNTState_None;        
 
         pIterator->mntBoat.mntSetting.BGL_Setting.Dist  = pMNT_Setting->BGL_Setting.Dist;
         pIterator->mntBoat.mntSetting.DRG_Setting.Dist  = pMNT_Setting->DRG_Setting.Dist;
         
         pIterator->mntBoat.mntSetting.DSP_Setting.isEnable  = 
                   pMNT_Setting->DSP_Setting.isEnable;
         
         pIterator->mntBoat.mntSetting.BGL_Setting.isSndEnable  = 
                   pMNT_Setting->BGL_Setting.isSndEnable;
                   
//          
         if(pIterator->mntBoat.mntSetting.BGL_Setting.isEnable == DISABLE)
         {
            pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = pMNT_Setting->BGL_Setting.isEnable;
         }
         else
         {
           if(pMNT_Setting->BGL_Setting.isEnable == DISABLE)
           {
              pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = DISABLE;
              INVD_clear(pIterator-MNT_Berthes);
           }
         }
          
         pIterator->mntBoat.mntSetting.DRG_Setting.isSndEnable  = 
                   pMNT_Setting->DRG_Setting.isSndEnable;
  
      
         pIterator->mntBoat.mntSetting.DRG_Setting.isEnable  = pMNT_Setting->DRG_Setting.isEnable;

         EEPROM_Write( 0, MNT_PAGE_ID+(pIterator-MNT_Berthes),
                       &(pIterator->mntBoat),MODE_8_BIT,sizeof(MNT_BOAT));  

       /*********************************************************************************************
        *
        *   Have no boat         ---->Init
        *   Hvae no lg & lt      ---->Pending
        *   Hvae lg & lt         ---->Monitored
        *
        **/
         if(pIterator->pBerth == NULL || pIterator->pBerth->Boat.user_id != pIterator->mntBoat.mmsi)   
         {
            pIterator->cfgState  = MNTState_Init;
         }      
         else if(pIterator->pBerth->Boat.dist > 100000)
         {
            pIterator->cfgState  = MNTState_Pending;
         }
         else 
         {
            if(pMNT_Setting->DRG_Setting.isEnable)
            {
               pIterator->mntBoat.lg  = pIterator->pBerth->Boat.longitude;
               pIterator->mntBoat.lt  = pIterator->pBerth->Boat.latitude;
   //INFO("update drg:(%ld,%ld)",pIterator->mntBoat.lg, pIterator->mntBoat.lt);            
            }
            pIterator->cfgState  = MNTState_Monitored;
         }
             
//#if DEBUG_LEVEL >= DEBUG_LEVEL_TAG             
//         MNT_DumpSetting(pIterator);                       
//#endif         
      }
      
      else if(pIterator->chsState == MNTState_None)
      {
         pIterator->chsState  = MNTState_Default;
         
         pIterator->mntBoat.mntSetting.DSP_Setting.isEnable  = ENABLE;   
                
         Cnt++;
      }
      

      
      pIterator  = pIterator->pNext;
   }
   
   MNT_printSetting();   
   return Cnt;
}


/** @brief   MNT_allocOneBerth
 *
 *  @dscrp   Alloc a node for MNT_BERTH link list
 *  @input 
 *  @output
 *  @return  The pointer of new memory. 
 *
 */
static MNT_BERTH * MNT_allocOneBerth(MNT_BERTH * mntBerthes)
{
   int i  = 0;
   for(i=0;i<MNT_NUM_MAX;i++)
   {
      if(mntBerthes[i].mntBoat.mmsi  == 0)
      {
         return &(mntBerthes[i]);
      }
   }
   return NULL;
}


/** @brief   MNT_add
 *
 *  @dscrp   Add new boat to mnt link list
 *  @input 
 *  @output
 *  @return   
 *
 */
Bool MNT_add(BERTH * pBerth)
{
   int i  = 0;
   MNT_BERTH * buf  = NULL;
   MNT_BERTH * pIterator  = NULL; 
   
   pIterator  = pMntHeader;
   
   /// If exist ,none
   while(pIterator)
   {
      if(pIterator->mntBoat.mmsi == pBerth->Boat.user_id)
      {
         return TRUE;
      }
      else
      {
         pIterator  = pIterator->pNext;
      }
   }
   
   buf  = MNT_allocOneBerth(MNT_Berthes);
   if(buf == NULL)
   {
INFO("allco mnt berth failed!");   
      return FALSE;
   }
   
   for(i=0;i<19;i++)
   {
      buf->mntBoat.name[i]  = pBerth->Boat.name[i];
      if(pBerth->Boat.name[i] == '\0')
      {
         break;
      }
   }
   
   buf->mntBoat.name[19]  = '\0';  
   buf->pBerth  = pBerth;
   buf->mntBoat.mmsi  = pBerth->Boat.user_id;
   buf->mntBoat.lt    = pBerth->Boat.latitude;
   buf->mntBoat.lg    = pBerth->Boat.longitude;
   buf->chsState      = MNTState_Default;
//   buf->cfgState      = MNTState_Init;
   buf->trgState      = MNTState_Monitored;
   buf->mntBoat.mntSetting.DSP_Setting.isEnable  = ENABLE;
   buf->mntBoat.mntSetting.DRG_Setting.isSndEnable  = ENABLE;
   buf->mntBoat.mntSetting.BGL_Setting.isSndEnable  = ENABLE;
   
   if(pBerth->Boat.dist > 99999)
   {
      buf->cfgState  = MNTState_Pending;
   }
   else 
   {
      buf->cfgState  = MNTState_Monitored;
   }
   
   if(pMntHeader != NULL)
   {
      pIterator  = pMntHeader;
      
      while(pIterator->pNext)
      {
         pIterator  = pIterator->pNext;
      }
      
      pIterator->pNext  = buf;                     
   }
   else
   {
      pMntHeader  = buf;  
   }  
//   EEPROM_Write( 0,MNT_PAGE_ID+(buf-MNT_Berthes),
//               buf, MODE_8_BIT, sizeof(MNT_BERTH));   
   EEPROM_Write( 0, MNT_PAGE_ID+(buf-MNT_Berthes),
                 &(buf->mntBoat),MODE_8_BIT,sizeof(MNT_BOAT));
   return TRUE;
}



/** @brief  MNT_removeById
 *
 *  @dscrp  Remove a mnt record.
 *  @input  The MMSI of mnt boat which is going to removed.
 *  @output TRUE if successful , or FALSE.
 */
Bool MNT_removeById(long Id)
{
   MNT_BERTH * pIterator  = pMntHeader; 
   while(pIterator)
   {
      if(pIterator->mntBoat.mmsi == Id)
      {
         pIterator->chsState  = MNTState_Cancel;
         return TRUE;
      }
      else
      {
         pIterator  = pIterator->pNext;
      }
   } 
   return  FALSE;
}


/** @brief 监控开 按键的响应
 *
 *
 *
 */
void MNT_Enable(void)
{
   MNT_BERTH* pIterator  = pMntHeader;
   
   while(pIterator)
   {
      /*********************************************************************************************
      *
      *   Have no boat         ---->Init
      *   Have no lg & lt      ---->Pending
      *   Have lg & lt         ---->Monitored
      *
      **/
       if(pIterator->pBerth == NULL || pIterator->pBerth->Boat.user_id != pIterator->mntBoat.mmsi)   
       {
          pIterator->cfgState  = MNTState_Init;
       }      
       else if(pIterator->pBerth->Boat.dist > 100000)
       {
          pIterator->cfgState  = MNTState_Pending;
       }
       else 
       {
          if(pIterator->mntBoat.mntSetting.DRG_Setting.isEnable)
          {
             pIterator->mntBoat.lg  = pIterator->pBerth->Boat.longitude;
             pIterator->mntBoat.lt  = pIterator->pBerth->Boat.latitude;
   //INFO("update drg:(%ld,%ld)",pIterator->mntBoat.lg, pIterator->mntBoat.lt);            
          }
          pIterator->cfgState  = MNTState_Monitored;
       }
         
       pIterator  = pIterator->pNext;  
   }
}

/** @brief 监控关 按键的响应
 *
 *
 *
 */
void MNT_Disable(void)
{
   MNT_BERTH* pIterator  = pMntHeader;
   
   while(pIterator)
   {
      INVD_clear(pIterator-MNT_Berthes);
      pIterator->trgState  = MNTState_None;
      pIterator->cfgState  = MNTState_Init;
      
      pIterator  = pIterator->pNext;
   }
//INFO("invd print");   
//   INVD_printf();
}


/** @brief MNT_getDefaultNum
 *
 *  @dscrp Get the number of mntBoat with MNTState_Init state.
 *
 */
 int MNT_getDefaultNum()
 {
    MNT_BERTH * pIterator  = pMntHeader;
    int cnt  = 0;
    
    while(pIterator)
    {
       if(pIterator->chsState == MNTState_Default)
          cnt++;
      
       pIterator  = pIterator->pNext;
    }
    
    return cnt;
 }


/** @brief  MNT_initSetting
 *
 *  @dscrp  Initialize a mntSetting.
 *  @input  The pointer pointing mntSetting.
 *  @output
 *  @return 
 */
//void MNT_initSetting()
//{  
////   memset((void*)&mntSetting, 0, sizeof(mntSetting));
//   mntSetting.DSP_Setting.isEnable  = ENABLE;
//   
//   mntSetting.BGL_Setting.Dist      = 50;
//   mntSetting.BGL_Setting.isSndEnable  = ENABLE;
//   mntSetting.BGL_Setting.isEnable     = DISABLE;
//   
//   mntSetting.DRG_Setting.Dist      = 100; 
//   mntSetting.DRG_Setting.isSndEnable  = ENABLE;   
//   mntSetting.DRG_Setting.isEnable     = DISABLE;
//}


/** @brief   printSetting
 *
 *  @dscrp   Print a mntSetting to terminal.
 *  @input   The pointer pointing mntSetting.
 *  @output
 *  @return 
 */
void printSetting(MNT_SETTING * p_setting)
{
   if(p_setting)
   {
      printf("   DSP     %s\r\n",p_setting->DSP_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("   BGL     %s\r\n",p_setting->BGL_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("       snd %s\r\n",p_setting->BGL_Setting.isSndEnable>DISABLE?"Enable":"Disable");
      printf("      dist %d\r\n",p_setting->BGL_Setting.Dist);
      printf("   DRG     %s\r\n",p_setting->DRG_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("       snd %s\r\n",p_setting->DRG_Setting.isSndEnable>DISABLE?"Enable":"Disable");
      printf("      dist %d\r\n",p_setting->DRG_Setting.Dist);
   }
   else
   {
      printf(" NULL setting\n\r");
   }
}



/** @brief  MNT_printSetting
 *
 *  @dscrp  Print all mntBoat's mntSetting.
 *  @input  
 *  @output
 *  @return 
 */
void MNT_printSetting()
{
   MNT_BERTH * pIterator  = pMntHeader;
   
   while(pIterator)
   {
         printf("\r\n");
         printf("mmsi %ld\r\n",pIterator->mntBoat.mmsi);
         printSetting(&(pIterator->mntBoat.mntSetting));
         printf("rn State:%d\n\r",pIterator->chsState);
         pIterator  = pIterator->pNext;
   }
}


MNT_BERTH* MNT_fetchPlayBerth(void)
{
   return pPlayBerth;
}



MNT_BERTH* MNT_fetchNextPlayBerth(void)
{ 
   MNT_BERTH* pIterator ;
   
   if(pPlayBerth == NULL)
   {
      pIterator  = pMntHeader;
      while(pIterator)
      {
         if( (pIterator->trgState&0x0f) == MNTState_Triggered)
         {
             pPlayBerth  = pIterator;
            return pPlayBerth;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
      pPlayBerth  = NULL;
      return pPlayBerth;
   }
   else
   {
      pIterator  = pPlayBerth->pNext;
      
      while(pIterator)
      {
         if( (pIterator->trgState&0x0f) == MNTState_Triggered)
         {
            pPlayBerth  = pIterator;
            return pPlayBerth;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
      
      pIterator  = pMntHeader;
      while(pIterator)
      {
         if( (pIterator->trgState&0x0f) == MNTState_Triggered)
         {
            pPlayBerth  = pIterator;
            return pPlayBerth;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
      
      pPlayBerth  = NULL;
      return pPlayBerth;
   }
}


void MNT_snapOnMiss(BERTH* pBerth)
{
   MNT_BERTH* pIterator  = pMntHeader;
   
   while(pIterator)
   {
      if(pIterator->pBerth == pBerth)
      {
         pIterator->pBerth  = NULL;
         
         if(pIterator->cfgState == MNTState_Monitored)
         {
            pIterator->snapLg  = pBerth->Boat.longitude;
            pIterator->snapLt  = pBerth->Boat.latitude;
            pIterator->snapDist  = pBerth->Boat.dist;
            
            pIterator->trgState  = ((0x01<<7) &0xf0) | MNTState_Triggered;
   //         pIterator->snapDist  = getSphereDist(pIterator->snapLt, pIterator->snapLg, 
   //                                              mothership.latitude, mothership.longitude);
            INVD_clear(pIterator-MNT_Berthes);   
         }  
         else
         {
            pIterator->cfgState  = MNTState_Init;
         }         
         return ;
      }
      else
      {
         pIterator  = pIterator->pNext;
      }
   }
}









