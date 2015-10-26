
#include "Check.h"
#include <math.h>
#include "sound.h"

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
INVADER Invaders[INVD_NUM_MAX];
INVADER * pInvdHeader  = NULL;
INVADER * pInvdTail    = NULL;
INVADER InvdMaskCursor = {0};


/*--------------------- Local functions -----------------------*/
static Bool removeById(long Id);
static void MNT_filter(void);


MNT_BERTH NULL_Berth  = {0};


void check()
{
   static int Cnt  = 0;
   static int Num  = 0;   
   
   int i  = 0;
   unsigned char trgState  = 0;
   
   float r   = 0.0;
   long diff_lt  = 0;
   long diff_lg  = 0;
   MNT_BERTH * pMntBerth  = NULL;
   MNT_BERTH * pIterator  = NULL;

   
   int isDSP  = 0;
   int isBGL  = 0;
   int isDRG  = 0;
   
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
    
     /// 更新掩码游标
      if(pInvdTail)
      {
INFO("mask cursor move"); 
         InvdMaskCursor.pNext  = pInvdTail;     
         InvdMaskCursor.MMSI  = pInvdTail->MMSI;
      }
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

//INFO("check %09ld",pMntBerth->mntBoat.mmsi);   

   
  /*****************************************************************************
   
                                      DSP check
   
   ******************************************************************************/  
   trgState  = (0x01<<7);   
   for(i=N_boat-1; i>=0; i--)   
   {  
      /// Not disappear.
      if(SimpBerthes[i].pBerth->Boat.user_id == pMntBerth->mntBoat.mmsi)
      {
         trgState  = 0;
         /// This boat had been gone but come back now.      
         if(SimpBerthes[i].pBerth->mntState == MNTState_None)
         {
//INFO("come back");            
            pMntBerth->pBoat  = &(SimpBerthes[i].pBerth->Boat);
            SimpBerthes[i].pBerth->mntState  = MNTState_Monited;
            if(pMntBerth->chsState == MNTState_Init)            
            {
               pMntBerth->chsState  = MNTState_Monited;
               pMntBerth->trgState  = MNTState_None;
            }
         }         
         break;
      }
   }

   if(trgState)
   {
      pMntBerth->pBoat  = NULL;
//INFO("This boy is gone %09ld",pMntBerth->mntBoat.mmsi); 
INFO("This boy is gone %x", trgState);     
   }
   
   
 
   /*****************************************************************************
   
                                      DRG check 
                                      
   ******************************************************************************/   
   if(   pMntBerth->mntBoat.mntSetting.DRG_Setting.isEnable   ///  开启走锚报警
      && (!(trgState&0x80))                                   ///  并且船没有消失
      && pMntBerth->pBoat->latitude  )
   {
      diff_lt  = pMntBerth->pBoat->latitude - pMntBerth->mntBoat.lt;
      diff_lg  = pMntBerth->pBoat->longitude- pMntBerth->mntBoat.lg;
      
      diff_lt  = MYABS(diff_lt);
      diff_lg  = MYABS(diff_lg);
      
 /*          

              
 */       
      
      
      if(   diff_lt+2*diff_lt/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist
          ||diff_lg+2*diff_lg/5 > pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
      {
         r  = sqrt(diff_lt*diff_lt + diff_lg*diff_lg);
      }
      
      if(r >= pMntBerth->mntBoat.mntSetting.DRG_Setting.Dist)
      { 
         trgState  |= (0x01<<6); 
         
INFO("Offset happened :%x", trgState);        
      }
   }
   
  
   /*****************************************************************************
   
                                      BGL check
   
   ******************************************************************************/      
   if(   pMntBerth->mntBoat.mntSetting.BGL_Setting.isEnable  ///开启防盗报警
      && (!(trgState&0xc0))                                         ///船没有消失,亦没有走锚
      && pMntBerth->pBoat->latitude  )
   {
      for(i=N_boat-1; i>=0; i--)
      {
         if(  SimpBerthes[i].pBerth->mntState == MNTState_None )
         {
            /// Closing.
            if(   (MYABS(SimpBerthes[i].latitude - pMntBerth->pBoat->latitude) <= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist)
               && (MYABS(SimpBerthes[i].longitude- pMntBerth->pBoat->longitude)<= pMntBerth->mntBoat.mntSetting.BGL_Setting.Dist)  )
            {
//INFO("\a%09ld invader %09ld", SimpBerthes[i].pBoat->user_id, pMntBerth->mntBoat.mmsi);            
               isBGL  = TRUE;
               SimpBerthes[i].pBerth->Boat.target  = pMntBerth->mntBoat.mmsi;
               INVD_add(SimpBerthes[i].pBerth->Boat.user_id, pMntBerth->mntBoat.mmsi);
            }
            else
            {          
               /// Black --> Whiit.
               if(SimpBerthes[i].pBerth->Boat.target  == pMntBerth->mntBoat.mmsi)
               {
//INFO("%09ld --> White",SimpBerthes[i].pBerth->Boat.user_id);                
                  /// INVD_delete.
                  SimpBerthes[i].pBerth->Boat.target  = 0;
                  INVD_deleteByMMSI(SimpBerthes[i].pBerth->Boat.user_id);
               }
            }
         }
         else
         {
        
            /// INVD_delete.
            if(SimpBerthes[i].pBerth->Boat.target)
            {
//INFO("invader %09ld become mnt",SimpBerthes[i].pBerth->Boat.user_id);  
              INVD_deleteByMMSI(SimpBerthes[i].pBerth->Boat.user_id);           
              SimpBerthes[i].pBerth->Boat.target  = 0;
            }
         }
      }
   }
   else
   {
INVD_deleteByTargetMMSI(pMntBerth->mntBoat.mmsi);   
      for(i=N_boat-1; i>=0; i--)
      {
         if(SimpBerthes[i].pBerth->Boat.target == pMntBerth->mntBoat.mmsi)
         {
//INFO("clear follower:%09ld",SimpBerthes[i].pBerth->Boat.user_id);         
            SimpBerthes[i].pBerth->Boat.target  = 0;
         }
      }
   }
   
   if(InvdMaskCursor.pNext != pInvdTail)
   {  
      trgState  |= (0x01<<5);
      pMntBerth->trgState  = (pMntBerth->trgState & 0xe0) | MNTState_Triggered;
INFO("Some one is closing :%x", trgState);      
   }
   

   
   if(pMntBerth->mntBoat.mntSetting.DSP_Setting.isEnable == DISABLE)
   {
      trgState  &= (~(0x01<<7));
   }
   
   if(pMntBerth->trgState != MNTState_Init)
   {
      uint8_t alarmFlags  = 0;
   
      /// 全 0 恢复监控状态
      if(trgState == 0)
      {    
         pMntBerth->trgState  = MNTState_None;
      }
      ///多出来 1 触发报警
      else if( ((trgState ^ pMntBerth->trgState) & trgState) >> 5)
      {  
         pMntBerth->trgState  = (trgState & 0xe0) | MNTState_Triggered;    
         
      }

      /// 其他情况，很淡定的更新监控状态就中了
      else
      { 
         pMntBerth->trgState  = trgState | (pMntBerth->trgState & 0x1f);
      }

      if( (pMntBerth->trgState & 0x1f) == MNTState_Triggered)
      {
         alarmFlags  = (trgState & 0xe0) >> 5;
INFO("alarmFlags:%x",alarmFlags);         
         switch(alarmFlags)         
         {
            case 0x05:
            case (0x01 << 2):
                SND_SelectID(SND_ID_DSP);
                break;
            case (0x01 << 1 ):
                SND_SelectID(SND_ID_DRG);
                break;
            case (0x01):
                SND_SelectID(SND_ID_BGL);
                break;
            default:
   INFO("Err!");            
                break;
         }          
      }
     
   }
   
   
   

//   
//   pIterator  = pMntHeader;
//   while(pIterator)
//   {
//      if( (pIterator->trgState & 0x1f)  ==  MNTState_Triggered )
//      {
//  
////         	UART_Send((UART_ID_Type)UART_2, (uint8_t*) &trgState, 1, BLOCKING);  /* ·¢ËÍÒ»¸ö×Ö·ûµ½UART */
////          UART_SendByte(UART_2, 0xaa);
////          UART_SendByte(UART_2, (SysConf.Snd.ArmSnd <<4)  |  (SysConf.Snd.Vol));          
//INFO("\aFBI warning!");   
//          SND_SelectID(SysConf.Snd.ArmSnd-1+4);
//          break;      
//      }  
//      else
//      {
//         pIterator  = pIterator->pNext;
//      }      
//   }
   
   
//   INVD_print();
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
      
      while(pIterator)
      {
         if(pIterator->MMSI == MMSI)
         {
            pIterator->targetMMSI  = targetMMSI;
            return 0;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
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
            
            memset((void*)pBC, 0, sizeof(INVADER));
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
void INVD_deleteByTargetMMSI(long targetMMSI)
{
   INVADER * pIterator  = NULL;
   INVADER * pBC        = NULL;
   
   
   while(pInvdHeader && pInvdHeader->targetMMSI==targetMMSI)
   {
INFO("delete invd at header");   
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
         if(pIterator->pNext->targetMMSI == targetMMSI)
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
             memset((void*)pBC, 0, sizeof(INVADER));
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



static void MNT_filter()
{
   MNT_BERTH * pIterator  = NULL;
   MNT_BERTH * pBC        = NULL;
   int i  = 0;
   
   while(pMntHeader && pMntHeader->chsState == MNTState_Delete)
   {
INFO("delete at header");   
      pBC  = pMntHeader;
      pMntHeader  = pMntHeader->pNext;
      
      for(i=N_boat-1;i>=0;i--)
      {
         if(SimpBerthes[i].pBerth->Boat.target == pBC->mntBoat.mmsi)
            SimpBerthes[i].pBerth->Boat.target  = 0;
      } 
      INVD_deleteByTargetMMSI(pBC->mntBoat.mmsi);    

//EEPROM_Write(MNT_PAGE_ID, MNT_getAddrOffset((uint8_t*)pBC), NULL, sizeof(MNT_BERTH));      
      EEPROM_Write(0 , MNT_PAGE_ID+pBC-MNT_Berthes,
                  &(NULL_Berth.mntBoat), MODE_8_BIT, sizeof(MNT_BOAT));
      memset((void*)pBC, 0, sizeof(MNT_BERTH));    
   }
//EEPROM_Write(MNT_PAGE_ID_HEADER, 0, (uint8_t*)&pMntHeader, sizeof(pMntHeader));   
   
   if(pMntHeader == NULL)
   {
      return;
   }
   
   pIterator  = pMntHeader;
   pBC        = pMntHeader->pNext; 
   while(pBC)
   {
      if(pBC->chsState == MNTState_Delete)
      {
INFO("delete at body");      
         pIterator->pNext  = pBC->pNext;
         for(i=N_boat-1;i>=0;i--)
         {
            if(SimpBerthes[i].pBerth->Boat.target == pBC->mntBoat.mmsi)
               SimpBerthes[i].pBerth->Boat.target  = 0;
         } 
         INVD_deleteByTargetMMSI(pBC->mntBoat.mmsi);  
         
//EEPROM_Write(MNT_PAGE_ID, MNT_getAddrOffset((uint8_t*)pBC), NULL, sizeof(MNT_BERTH));            
         EEPROM_Write(0 , MNT_PAGE_ID+pBC-MNT_Berthes,
                      &(NULL_Berth.mntBoat), MODE_8_BIT, sizeof(MNT_BOAT));         
         memset((void*)pBC, 0, sizeof(MNT_BERTH));
      }
      else
      {
       ///   没有消失报警  && 船丢了 && 非初始化状态  -->船丢了但还没来得及check
        if( (pIterator->trgState<0x80) && (!pIterator->pBoat) && pIterator->trgState != MNTState_Init)
        {
           INVD_deleteByTargetMMSI(pIterator->mntBoat.mmsi);
        }
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