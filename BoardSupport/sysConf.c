#include "SystemConfig.h"
#include "Config.h"
#include "Setting.h"
#include "string.h"
#include "lpc177x_8x_eeprom.h"

#define FLASH_SIZE (8*1024*1024)

extern MNT_BERTH MNT_Berthes[MNT_NUM_MAX];



char text_Buf[10] = {'-'};

///                                        
static  CONF_UI_COLOR UI_Skins[2] = {GUI_BLACK};

uint8_t TEXT_Buffer[]={"1234567890"}; 

uint8_t datatemp[sizeof(TEXT_Buffer)];

CONF_SYS SysConf;
CONF_SYS sysConf;

const int SYSCONF_SIZE = sizeof(SysConf);


void printSysConf(CONF_SYS * p)
{
   printf("\n\r");
   printf("Skin:           %s-%d\n\r",p->Skin?"Day":"Night",p->Skin);
   printf("Snd -- Vol      %d\n\r",p->Snd.Vol);
   printf("Brightness      %d\n\r",p->Brt);
   printf("Snd -- ArmSnd   %d\n\r",p->Snd.ArmSnd);
   printf("Snd -- KeySnd   %d\n\r",p->Snd.KeySnd);
   printf("Unit            %s-%d\n\r",p->Unit?"nm":"km",p->Unit);
   printf("Shape           %s-%d\n\r",p->Shape?"Fish":"Boat",p->Shape);
}


static Bool checkSysConf()
{
   Bool flag  = TRUE;
   if(SysConf.Skin != SKIN_Day  &&  SysConf.Skin != SKIN_Night) 
   {
      flag  = FALSE;
      printf("Skin load error!\n\r");
      SysConf.Skin  = SKIN_Night;
   }
   if(SysConf.Brt < 0  ||  SysConf.Brt > 4)                     
   {
      flag  = FALSE;   
      printf("Brt  load error!\n\r");
      SysConf.Brt  = 1;
   }
   if(SysConf.Snd.Vol < 0  ||  SysConf.Snd.Vol > 6)            
   {
      flag  = FALSE;   
      printf("Vol  load error!\n\r");
      SysConf.Snd.Vol  = 1;
   }
   if(SysConf.Snd.ArmSnd < 1  ||  SysConf.Snd.ArmSnd > 2)      
   {
      flag  = FALSE;   
      printf("ArmSnd  load error!\n\r");
      SysConf.Snd.ArmSnd  = 1;
   }
   if(SysConf.Snd.KeySnd < 0  || SysConf.Snd.KeySnd > 2)       
   {
      flag  = FALSE;   
      printf("KeySnd  load error!\n\r");
      SysConf.Snd.KeySnd  = 1;
   }
   if(SysConf.Unit != UNIT_km  &&  SysConf.Unit != UNIT_nm)     
   {
      flag  = FALSE;   
      printf("Unit load error!\n\r");
      SysConf.Unit  = UNIT_nm;
   }
   if(SysConf.Shape != SHAPE_Boat  &&  SysConf.Shape != SHAPE_Fish) 
   {
      flag  = FALSE;   
      printf("Shape load error!\n\r");
      SysConf.Shape  = SHAPE_Fish;
   }
   return flag;
}



Bool sysLoad()
{  
   uint16_t i  = 0; 
   uint16_t j  = 0;
   uint16_t cnt  = 0;   
   Bool flag  = TRUE;
   
   
   printf("\n\r Loading...\n\r"); 
   
   EEPROM_Read(SYSCONF_ADDR%EEPROM_PAGE_SIZE, SYSCONF_ADDR/EEPROM_PAGE_SIZE,
               &SysConf, MODE_8_BIT, sizeof(CONF_SYS));
   /// Check if system config is right or not.
   printSysConf(&SysConf);
   if(!checkSysConf())
   {
printf("after fix:\n\r");   
      printSysConf(&SysConf);
      flag  = FALSE;
   }
         

   for(i=0; i<MNT_NUM_MAX; i++)
   {
      EEPROM_Read(0, MNT_PAGE_ID+i, &MNT_Berthes[i], MODE_8_BIT, sizeof(MNT_BOAT));
      if(MNT_Berthes[i].mntBoat.mmsi)
      {
         MNT_Berthes[i].chsState  = MNTState_Init;
         MNT_Berthes[i].trgState  = MNTState_Init;
printf("%d--MMSI:%09ld\n\r",i,MNT_Berthes[i].mntBoat.mmsi);      
         cnt++;
      }
   }
   
   if(cnt)
   {
      i  = 0;
      while(MNT_Berthes[i].mntBoat.mmsi == 0) i++;
      
      pMntHeader  = &MNT_Berthes[i];
      
      for(j=i+1; j<MNT_NUM_MAX;)
      {
         if(MNT_Berthes[j].mntBoat.mmsi)
         {
            MNT_Berthes[i].pNext  = &MNT_Berthes[j];
            i  = j;
            j  = i+1;
         }
         else
         {
            j++;
         }
      }
   }
   
   return flag;
}


void sysStore()
{
   uint16_t i  = 0;
   for(i=0; i<MNT_NUM_MAX; i++)
   {
      EEPROM_Erase(MNT_PAGE_ID+i);
   }
   EEPROM_Write(SYSCONF_ADDR%EEPROM_PAGE_SIZE, SYSCONF_ADDR/EEPROM_PAGE_SIZE,
               &SysConf, MODE_8_BIT, sizeof(CONF_SYS));        
}


void sysInit()
{    
   MNT_BERTH * pIterator  = NULL;
   uint16_t  i  = 0; 
    
//   EEPROM_Erase(MNT_PAGE_ID);
   if(sysLoad())
   {
INFO("System load successfully!");   
   }
   else
   {
INFO("Error happened when system load.System will be configed with default value");   

   }
   
   pIterator  = pMntHeader;
   while(pIterator)
   {
      if(i == 5)
         printf("\n\r");
      printf("%09ld --> ",pIterator->mntBoat.mmsi);
      
      i++;
      i  = i%5;
      pIterator  = pIterator->pNext;
   }
   printf("\n\r");
   
}

