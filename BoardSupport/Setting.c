#include "Setting.h"
#include <string.h>
#include "sysConf.h"
#include "invader.h"



#define MNT_BERTH_SIZE  sizeof(MNT_BERTH)

MNT_BERTH MNT_Berthes[MNT_NUM_MAX];


MNT_BERTH * pMntHeader  = NULL;

MNT_SETTING mntSetting; 


static char  strStates[9][10]  =
{
   {"None     "},
   {"Cancel   "},
   {"Init     "},
   {"Pending  "},
   {"Choosen  "},
   {"Default  "},
   {"Monitored"},
   {"Triggered"},
   {"Masked   "}
};


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
   while(pIterator)
   {    
      if(   (pIterator->chsState == MNTState_Choosen)
          ||(pIterator->chsState == MNTState_Default)  )
      {
         pIterator->chsState  = MNTState_Monitored;
         pIterator->trgState  = MNTState_None;        
         
         if(SysConf.Unit == UNIT_nm)
         {        
            pIterator->mntBoat.mntSetting.BGL_Setting.Dist  = 
                      pMNT_Setting->BGL_Setting.Dist*10;
            pIterator->mntBoat.mntSetting.DRG_Setting.Dist  = 
                      pMNT_Setting->DRG_Setting.Dist*10;                       
         }
         else if(SysConf.Unit == UNIT_km)
         {           
            pIterator->mntBoat.mntSetting.BGL_Setting.Dist  =
                      pMNT_Setting->BGL_Setting.Dist*54/10;
            pIterator->mntBoat.mntSetting.DRG_Setting.Dist  = 
                      pMNT_Setting->DRG_Setting.Dist*54/10; 
         }  
         else
         {
INFO("Error!");
         }  
         
         pIterator->mntBoat.mntSetting.DSP_Setting.isEnable  = 
                   pMNT_Setting->DSP_Setting.isEnable;
         
         pIterator->mntBoat.mntSetting.BGL_Setting.isSndEnable  = 
                   pMNT_Setting->BGL_Setting.isSndEnable;
                   
         if(pMNT_Setting->BGL_Setting.Dist == 0)
         {
            pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = DISABLE;         
         }
         else
         {
            pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = 
                     pMNT_Setting->BGL_Setting.isEnable;        
         }

         if(pMNT_Setting->BGL_Setting.Dist > 0  &&  pIterator->mntBoat.mntSetting.BGL_Setting.isEnable)
         {
            pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = ENABLE;
         }
         else 
         {
            pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = DISABLE;
            INVD_clear(pIterator-MNT_Berthes);
         }
          
         pIterator->mntBoat.mntSetting.DRG_Setting.isSndEnable  = 
                   pMNT_Setting->DRG_Setting.isSndEnable;

         if(pMNT_Setting->DRG_Setting.Dist == 0)
         {
            pIterator->mntBoat.mntSetting.DRG_Setting.isEnable  = DISABLE;
         }   
         else
         {
            pIterator->mntBoat.mntSetting.DRG_Setting.isEnable  = 
                      pMNT_Setting->DRG_Setting.isEnable;
         }         

  

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
             
       //      MNT_DumpSetting(pIterator);                       
      }
      
      else if(pIterator->chsState == MNTState_None)
      {
         pIterator->chsState  = MNTState_Default;
         
         pIterator->mntBoat.mntSetting.DSP_Setting.isEnable  = ENABLE;   
                
         Cnt++;
      }
      

      
      pIterator  = pIterator->pNext;
   }
   
//   MNT_printSetting();
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
   
   for(i=0;i<20;i++)
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
   buf->cfgState      = MNTState_Init;
   buf->trgState      = MNTState_Monitored;
   buf->mntBoat.mntSetting.DSP_Setting.isEnable  = ENABLE;
   
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
//      if(pIterator->mntBoat.mmsi == Id)
      if(pIterator->mntBoat.mmsi == Id)
      {
//         pIterator->mntBoat.mntState  = MNTState_Delete;
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



/** @brief MNT_getDefaultNum
 *
 *  @dscrp Get the number of mntBoat with MNTState_Init state.
 *
 */
 uint8_t MNT_getDefaultNum()
 {
    MNT_BERTH * pIterator  = pMntHeader;
    uint8_t cnt  = 0;
    
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
void MNT_initSetting()
{  
   memset((void*)&mntSetting, 0, sizeof(mntSetting));
   mntSetting.DSP_Setting.isEnable  = ENABLE;
   mntSetting.BGL_Setting.Dist      = 5;
   mntSetting.DRG_Setting.Dist      = 10;  
}


/** @brief   printSetting
 *
 *  @dscrp   Print a mntSetting to terminal.
 *  @input   The pointer pointing mntSetting.
 *  @output
 *  @return 
 */
static void printSetting(MNT_SETTING * p_setting)
{
      printf("   DSP     %s\r\n",p_setting->DSP_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("   BGL     %s\r\n",p_setting->BGL_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("       snd %s\r\n",p_setting->BGL_Setting.isSndEnable>DISABLE?"Enable":"Disable");
      printf("      dist %d\r\n",p_setting->BGL_Setting.Dist);
      printf("   DRG     %s\r\n",p_setting->DRG_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("       snd %s\r\n",p_setting->DRG_Setting.isSndEnable>DISABLE?"Enable":"Disable");
      printf("      dist %d\r\n",p_setting->DRG_Setting.Dist);
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
   int cnt  = 0;
   int i  = 0;
   while(pIterator)
   {
         if(pIterator->chsState == MNTState_Default)
            cnt++;
         printf("\r\n");
         printf("%d-mmsi %ld\r\n",i,pIterator->mntBoat.mmsi);
         printSetting(&(pIterator->mntBoat.mntSetting));
         printf("rn State:%d",pIterator->chsState);
         printf("still hava %d is default\r\n",cnt);
         pIterator  = pIterator->pNext;
         i++;
   }
}


void MNT_DumpSetting(MNT_BERTH * pBerth)
{

   if(pBerth)
   {
      printf("%09ld--chs:%s cfg:%s trg:%s\n\r",
                  pBerth->mntBoat.mmsi, 
                  strStates[pBerth->chsState&0x1f],
                  strStates[pBerth->cfgState&0x1f],
                  strStates[pBerth->trgState&0x1f]);
   }
}
