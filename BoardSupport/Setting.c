#include "Setting.h"
#include "Config.h"
#include <string.h>
#include "SystemConfig.h"

int N_monitedBoat  = 0;
MNT_BERTH MNT_Berthes[MNT_NUM_MAX];

 MNT_BERTH * pMntHeader  = NULL;
 MNT_BERTH * Next  = NULL;

extern CONF_SYS  SysConf;

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
         pMNT_Boat[i].pBoat->mntStates  = MNTState_Monited;
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

int MNT_deleteByIndex(MNT_BOAT * pMNT_Boat,int index, long id)
{
   if(pMNT_Boat[index].mmsi != id)
   {
      return -1;
   }
   memset(pMNT_Boat+index,0, sizeof(MNT_BOAT));
   N_monitedBoat--;
   return 0;
}


int MNT_deleteById(MNT_BOAT * pMNT_Boat, long id)
{
   int i  = 0;
   for(i=0;i<N_monitedBoat;i++)
   {
      if(pMNT_Boat[i].mmsi == id)
      {
         pMNT_Boat[i].pBoat->mntStates  = MNTState_None;
         memset(pMNT_Boat+i, 0, sizeof(MNT_BOAT));
         N_monitedBoat--;
         return i;
      }
   }
   
   return -1;
}

/**@brief  MNT_makeSettingUp
 * 
 * @dscrp  Copy setting in etWin to  selected  boats monited.
 * @input  Monited boat array.
 *         Number of monited boat.
 *         Setting in etWin.
 * @Outpu 
 * @return Times of copy. 
 */

void MNT_makeSettingUp (MNT_SETTING * pMNT_Setting)
{
   int i  = 0;

//   for(i=0;i<N;i++)
//   {
//      if(  (pMNT_Boat[i].mntState == MNTState_Choosen)
//         ||(pMNT_Boat[i].mntState == MNTState_Default) )
//      {
//         cnt++;
//         pMNT_Boat[i].mntState  = MNTState_Monited;
//         
//         pMNT_Boat[i].mntSetting.DSP_Setting.isEnable  = 
//                   pMNT_Setting->DSP_Setting.isEnable;
//         
//         pMNT_Boat[i].mntSetting.BGL_Setting.isEnable  = 
//                   pMNT_Setting->BGL_Setting.isEnable;
//         pMNT_Boat[i].mntSetting.BGL_Setting.isSndEnable  = 
//                   pMNT_Setting->BGL_Setting.isSndEnable;
//         pMNT_Boat[i].mntSetting.BGL_Setting.Dist  = 
//                   pMNT_Setting->BGL_Setting.Dist;
//                   
//         pMNT_Boat[i].mntSetting.DRG_Setting.isEnable  = 
//                   pMNT_Setting->DRG_Setting.isEnable;
//         pMNT_Boat[i].mntSetting.DRG_Setting.isSndEnable  = 
//                   pMNT_Setting->DRG_Setting.isSndEnable;
//         pMNT_Boat[i].mntSetting.DRG_Setting.Dist  = 
//                   pMNT_Setting->DRG_Setting.Dist;
//      }
//      else if(pMNT_Boat[i].mntState == MNTState_None)
//      {
//         pMNT_Boat[i].mntState  = MNTState_Default;
//         
//         pMNT_Boat[i].mntSetting.DSP_Setting.isEnable  = ENABLE;
//      }
//   }

   MNT_BERTH * pIterator  = pMntHeader;
   while(pIterator)
   {
      if(   (pIterator->mntBoat.mntState == MNTState_Choosen)
          ||(pIterator->mntBoat.mntState == MNTState_Default)  )
      {
         pIterator->mntBoat.mntState  = MNTState_Monited;
         
         pIterator->mntBoat.mntState  = MNTState_Monited;
         
         pIterator->mntBoat.mntSetting.DSP_Setting.isEnable  = 
                   pMNT_Setting->DSP_Setting.isEnable;
         
         pIterator->mntBoat.mntSetting.BGL_Setting.isEnable  = 
                   pMNT_Setting->BGL_Setting.isEnable;
         pIterator->mntBoat.mntSetting.BGL_Setting.isSndEnable  = 
                   pMNT_Setting->BGL_Setting.isSndEnable;
                   
         pIterator->mntBoat.mntSetting.DRG_Setting.isEnable  = 
                   pMNT_Setting->DRG_Setting.isEnable;
         pIterator->mntBoat.mntSetting.DRG_Setting.isSndEnable  = 
                   pMNT_Setting->DRG_Setting.isSndEnable;                   
                   
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
                   

 

         pIterator->mntBoat.lt  = pIterator->mntBoat.pBoat->latitude;
         pIterator->mntBoat.lg  = pIterator->mntBoat.pBoat->longitude;         
      }
      
      else if(pIterator->mntBoat.mntState == MNTState_None)
      {
         pIterator->mntBoat.mntState  = MNTState_Default;
         
         pIterator->mntBoat.mntSetting.DSP_Setting.isEnable  = ENABLE;
      }
      
      pIterator  = pIterator->pNext;
   }
}


MNT_BERTH * MNT_allocOneBerth(MNT_BERTH * mntBerthes)
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


//void MNT_freeOneBerth(MNT_BERTH * pMntBerth)
//{
//   memset((void*)pMntBerth, 0, sizeof(MNT_BERTH));
//}


Bool MNT_add(boat * pBoat)
{
   int i  = 0;
   MNT_BERTH * buf  = NULL;
   MNT_BERTH * pIterator  = NULL; 
   
   buf  = MNT_allocOneBerth(MNT_Berthes);
   if(buf == NULL)
   {
INFO("allco mnt berth failed!");   
      return FALSE;
   }
   
   buf->mntBoat.pBoat  = pBoat;
   buf->mntBoat.mmsi  = pBoat->user_id;
   buf->mntBoat.lt    = pBoat->latitude;
   buf->mntBoat.lg    = pBoat->longitude;
   buf->mntBoat.mntState  = MNTState_Default;
   buf->mntBoat.pBoat->mntStates  = MNTState_Monited;
   for(i=0;i<20;i++)
   {
      buf->mntBoat.name[i]  = pBoat->name[i];
      if(pBoat->name[i] == '\0')
      {
         break;
      }
   }
   buf->mntBoat.name[19]  = '\0';
   
   i  = 0;
   
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
   return TRUE;
}


Bool MNT_removeById(long Id)
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
      pBC = pMntHeader;
      pIterator  = pMntHeader->pNext;
      while(pIterator)
      {
         if(pIterator->mntBoat.mmsi == Id)
         {
            pBC->pNext  = pIterator->pNext;
            memset((void*)pIterator, 0, sizeof(MNT_BERTH));
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
}

//void MNT_resetIterator()
//{
//   Next  = pMntHeader;
//}

void MNT_testReset()
{
   Next  = pMntHeader;
}


MNT_BERTH * MNT_getNext()
{
   if(Next  == pMntHeader)
   {
   }
   if(Next != NULL)
   {
      Next  = Next->pNext;
   }
//   else
//   {
//      Next  = pMntHeader;
//   }
   
   return Next;
}

void MNT_init(MNT_SETTING * pMntSetting)
{
   memset((void*)pMntSetting, 0, sizeof(MNT_SETTING));
   pMntSetting->DSP_Setting.isEnable  = ENABLE; 
   
}


void printSetting(MNT_SETTING * p_setting)
{
      printf("   DSP     %s\r\n",p_setting->DSP_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("   BGL     %s\r\n",p_setting->BGL_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("       snd %s\r\n",p_setting->BGL_Setting.isSndEnable>DISABLE?"Enable":"Disable");
      printf("      dist %d\r\n",p_setting->BGL_Setting.Dist);
      printf("   DRG     %s\r\n",p_setting->DRG_Setting.isEnable>DISABLE?"Enable":"Disable");
      printf("       snd %s\r\n",p_setting->DRG_Setting.isSndEnable>DISABLE?"Enable":"Disable");
      printf("      dist %d\r\n",p_setting->DRG_Setting.Dist);
}



void printMoniteSetting(MNT_BOAT * pMNT_Boat)
{
   int i  = 0;
   int cnt  = 0;
   for(i=0;i<N_monitedBoat;i++)
   {
      if(pMNT_Boat[i].mmsi > 0)
      {
         if(pMNT_Boat[i].mntState == MNTState_Default)
            cnt++;
         printf("\r\n");
         printf("%d-mmsi %ld\r\n",i,pMNT_Boat[i].mmsi);
         printf("%d-name %s\r\n", i,pMNT_Boat[i].name);
   //      printf("   DSP     %s\r\n",mntBoats[i].MNTSetting.DSP_Setting.isEnable>DISABLE?"Enable":"Disable");
   //      printf("   BGL     %s\r\n",mntBoats[i].MNTSetting.BGL_Setting.isEnable>DISABLE?"Enable":"Disable");
   //      printf("       snd %s\r\n",mntBoats[i].MNTSetting.BGL_Setting.isSndEnable>DISABLE?"Enable":"Disable");
   //      printf("      dist %d\r\n",mntBoats[i].MNTSetting.BGL_Setting.dist);
   //      printf("   DRG     %s\r\n",mntBoats[i].MNTSetting.DRG_Setting.isEnable>DISABLE?"Enable":"Disable");
   //      printf("       snd %s\r\n",mntBoats[i].MNTSetting.DRG_Setting.isSndEnable>DISABLE?"Enable":"Disable");
   //      printf("      dist %d\r\n",mntBoats[i].MNTSetting.DRG_Setting.dist);
         printSetting(&(pMNT_Boat[i].mntSetting));
         printf("/r/nState:%d",pMNT_Boat[i].mntState);
         printf("still hava %d is default\r\n",cnt);
      }
   } 
}


void MNT_printSetting()
{
   MNT_BERTH * pIterator  = pMntHeader;
   int cnt  = 0;
   int i  = 0;
   while(pIterator)
   {
         if(pIterator->mntBoat.mntState == MNTState_Default)
            cnt++;
         printf("\r\n");
         printf("%d-mmsi %ld\r\n",i,pIterator->mntBoat.mmsi);
         printf("%d-name %s\r\n", i,pIterator->mntBoat.name);
   //      printf("   DSP     %s\r\n",mntBoats[i].MNTSetting.DSP_Setting.isEnable>DISABLE?"Enable":"Disable");
   //      printf("   BGL     %s\r\n",mntBoats[i].MNTSetting.BGL_Setting.isEnable>DISABLE?"Enable":"Disable");
   //      printf("       snd %s\r\n",mntBoats[i].MNTSetting.BGL_Setting.isSndEnable>DISABLE?"Enable":"Disable");
   //      printf("      dist %d\r\n",mntBoats[i].MNTSetting.BGL_Setting.dist);
   //      printf("   DRG     %s\r\n",mntBoats[i].MNTSetting.DRG_Setting.isEnable>DISABLE?"Enable":"Disable");
   //      printf("       snd %s\r\n",mntBoats[i].MNTSetting.DRG_Setting.isSndEnable>DISABLE?"Enable":"Disable");
   //      printf("      dist %d\r\n",mntBoats[i].MNTSetting.DRG_Setting.dist);
         printSetting(&(pIterator->mntBoat.mntSetting));
         printf("/r/nState:%d",pIterator->mntBoat.mntState);
         printf("still hava %d is default\r\n",cnt);
         pIterator  = pIterator->pNext;
         i++;
   }
}
