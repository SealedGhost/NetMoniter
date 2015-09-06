#include "Setting.h"
#include <string.h>
#include "sysConf.h"



#define MNT_BERTH_SIZE  sizeof(MNT_BERTH)

int N_monitedBoat  = 0;
MNT_BERTH MNT_Berthes[MNT_NUM_MAX];


MNT_BERTH * pMntHeader  = NULL;

MNT_SETTING mntSetting; 




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
 * @dscrp  Copy setting in etWin to  selected  boats monited.
 * @input  Monited boat array.
 *         Number of monited boat.
 *         Setting in etWin.
 * @Output 
 * @return Times of copy. 
 */
void MNT_makeSettingUp (MNT_SETTING * pMNT_Setting)
{
   int i  = 0;
   MNT_BERTH * pIterator  = pMntHeader;
   while(pIterator)
   {    
      if(   (pIterator->chsState == MNTState_Choosen)
          ||(pIterator->chsState == MNTState_Default)  )
      {
         pIterator->chsState  = MNTState_Monited;
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
         
         if(pMNT_Setting->BGL_Setting.Dist == 0)
         {
            pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = DISABLE;         
         }
         else
         {
            pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = 
                     pMNT_Setting->BGL_Setting.isEnable;        
         }

         pIterator->mntBoat.mntSetting.BGL_Setting.isSndEnable  = 
                   pMNT_Setting->BGL_Setting.isSndEnable;
         
         if(pMNT_Setting->DRG_Setting.Dist == 0)
         {
            pIterator->mntBoat.mntSetting.DRG_Setting.isEnable  = DISABLE;
         }   
         else
         {
            pIterator->mntBoat.mntSetting.DRG_Setting.isEnable  = 
                      pMNT_Setting->DRG_Setting.isEnable;
         }         

         pIterator->mntBoat.mntSetting.DRG_Setting.isSndEnable  = 
                   pMNT_Setting->DRG_Setting.isSndEnable;  
                 
                   
 

                   
      }
      
      else if(pIterator->chsState == MNTState_None)
      {
         pIterator->chsState  = MNTState_Default;
         
         pIterator->mntBoat.mntSetting.DSP_Setting.isEnable  = ENABLE;     

      }
      
      pIterator  = pIterator->pNext;
   }
   
   MNT_printSetting();
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
Bool MNT_add(boat * pBoat)
{
   int i  = 0;
   MNT_BERTH * buf  = NULL;
   MNT_BERTH * pIterator  = NULL; 
   
   pIterator  = pMntHeader;
   
   /// If exist ,none
   while(pIterator)
   {
      if(pIterator->mntBoat.mmsi == pBoat->user_id)
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
      buf->mntBoat.name[i]  = pBoat->name[i];
      if(pBoat->name[i] == '\0')
      {
         break;
      }
   }
   
   buf->mntBoat.name[19]  = '\0';  
   buf->pBoat  = pBoat;
   buf->mntBoat.mmsi  = pBoat->user_id;
   buf->mntBoat.lt    = pBoat->latitude;
   buf->mntBoat.lg    = pBoat->longitude;
   buf->chsState      = MNTState_Default;
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
         pIterator->chsState  = MNTState_Delete;
         return TRUE;
      }
      else
      {
         pIterator  = pIterator->pNext;
      }
   } 
   return  FALSE;
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
         printf("/r/n State:%d",pIterator->chsState);
         printf("still hava %d is default\r\n",cnt);
         pIterator  = pIterator->pNext;
         i++;
   }
}
