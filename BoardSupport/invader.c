#include "invader.h"
#include "string.h"
#include "Setting.h"

static INVADER INVD_Heap[MNT_NUM_MAX][INVD_NUM_MAX] = {0};
INVADER * pInvdHeaders[MNT_NUM_MAX] = {NULL};
INVADER * pInvdTails[MNT_NUM_MAX] = {NULL};
static INVADER * pInvdMasks[MNT_NUM_MAX]  = {NULL};

/**@brief  INVD_allocOne
 *
 * @dscr   Find empty place for add new invader.
 *
 */
static INVADER * INVD_allocOne(unsigned int  row)
{
  int i = 0;

   for(; i<INVD_NUM_MAX; i++)   
   {
      if(INVD_Heap[row][i].pBerth == NULL)
      {
         return &(INVD_Heap[row][i]);
      }
   }
   
   return NULL;
}



void INVD_deleteByAddr(BERTH * pBerth)
{
   int i  = 0;
   pBerth->isInvader  = 0;
   for(; i<MNT_NUM_MAX; i++)
   {
      if(pInvdHeaders[i])
      {
         INVD_delete(i, pBerth);
      }
   }
}



/**@brief  INVD_add
 *
 * @dscr   Add invader into Invd linked-list.
 *
 */
Bool INVD_add(int row, BERTH * pBerth)
{
   INVADER * pIterator  = NULL;
   INVADER * buf  = NULL;
 
   if(row<0  ||  row>=MNT_NUM_MAX)
   {
INFO("pass error row:%d",row);  
      return FALSE;
   }
 
//   INVD_deleteByAddr(pBerth);
 
   if(pInvdHeaders[row] != NULL)  
   {
      pIterator  = pInvdHeaders[row];
      
      while(pIterator)
      {
         /// This invader has existed and  return.
         if(pIterator->pBerth == pBerth)
         {

            return TRUE;
         }
         else
         {
            pIterator  = pIterator->pNext;
         }
      }
      
      /// Add at tail.
    
      buf  = INVD_allocOne(row);
   
      if(buf != NULL)
      {   
         buf->pBerth  = pBerth;      
         pInvdTails[row]->pNext  = buf;     
         pInvdTails[row]  = buf;       
         return TRUE;
      }
      else
      {
INFO("alloc invd failed!");
          return FALSE;
      }
   }
   /// Add at first time.
   else
   {
//printf("add as header\n");   
      buf  = INVD_allocOne(row);
      
      if(buf)
      {
         buf->pBerth  = pBerth;
         buf->pNext  = NULL;
         pInvdHeaders[row]  = buf;
         pInvdTails[row]  = buf;
         return TRUE;
      }
      else
      {
INFO("alloc invd Err!");
         return FALSE;
      }
   }
}


/**@brief  INVD_delete
 *
 * @dscr   
 *
 */
void INVD_delete(int row, BERTH * pBerth)
{
   if(row<0  ||  row>=MNT_NUM_MAX)
   {
INFO("pass error row:%d",row); 
      return;
   }
  
   if(pInvdHeaders[row])
   {
      INVADER * pIterator  = pInvdHeaders[row];
      INVADER * pBC  = NULL;
      
      /// 头节点要删除
      if(pInvdHeaders[row]->pBerth == pBerth)
      {
         pInvdHeaders[row]  = pInvdHeaders[row]->pNext;
         
         /// 头节点和尾节点相同，即链表中只有一个节点.
         if(pInvdTails[row] == pIterator)
         {
            pInvdTails[row]  = NULL;
         }
         
         /// 头节点为掩码节点
         if(pInvdMasks[row] == pIterator)
         {
            pInvdMasks[row]  = pIterator->pNext;
         }
        
         if(pIterator->pBerth->isInvader == MNT_Berthes[row].mntBoat.mmsi)
         {
            pIterator->pBerth->isInvader  = 0;
         }
         
         memset((void*)pIterator, 0, sizeof(INVADER));
         return ;
      }
      else
      { 
         pBC  = pIterator->pNext;
         while(pBC)
         {
            if(pBC->pBerth == pBerth)
            {
               pIterator->pNext  = pBC->pNext;
               
               if(pInvdTails[row] == pBC)
               {
                  pInvdTails[row]  = pIterator;
               }
               
               if(pBC == pInvdMasks[row])
               {
                  pInvdMasks[row]  = pBC->pNext;
               }
               
               if(pBC->pBerth->isInvader == MNT_Berthes[row].mntBoat.mmsi)
               {
                  pBC->pBerth->isInvader  = 0;
               }
               memset((void*)pBC, 0, sizeof(INVADER));
               return ;
            }
            else
            {
               pIterator  = pBC;
            }
            
            pBC  = pIterator->pNext;
         }
         
//INFO("Can not find this invader!");         
         return;
      }
   }
   
   /// 头节点为空，直接返回FALSE.
   else
   {
       return ;
   }
}



/**@brief  INVD_clear
 *
 * @dscr   
 *
 */
void INVD_clear(int row)
{
   INVADER * pIterator;
   
   if(row>=0 && row < MNT_NUM_MAX)
   {   
      pIterator  = pInvdHeaders[row];
      
      while(pIterator)
      {         
         if(pIterator->pBerth && pIterator->pBerth->isInvader  == MNT_Berthes[row].mntBoat.mmsi)
         {
            pIterator->pBerth->isInvader  = 0;
         }
         pIterator  = pIterator->pNext;
      }

      pInvdHeaders[row]  = NULL;
      pInvdTails[row]  = NULL;
      pInvdMasks[row] = NULL;
      
      memset((void*)INVD_Heap[row], 0, sizeof(INVADER) * INVD_NUM_MAX);
      return;   
   }
   else
   {
INFO("pass error row:%d",row);
      return ;
   }
}



/**@brief  INVD_updataMask
 *
 * @dscr   
 *
 */
void INVD_updataMask()
{
   int i  = MNT_NUM_MAX;
   
   for(; i; i--)
   {
      pInvdMasks[i-1]  = pInvdTails[i-1];
   }
}



/**@brief  INVD_isAllMasked
 *
 * @dscr   
 *
 */
Bool INVD_isAllMasked(int row)
{
   return (pInvdMasks[row] == pInvdTails[row]);
}

Bool INVD_isAlone(int row)
{
   return (pInvdHeaders[row] == NULL);
}

void INVD_printf()
{
   int i  = 0;
   INVADER * pIterator;
   
   for(; i<MNT_NUM_MAX; i++ )   
   {
      pIterator  = pInvdHeaders[i];
      
      while(pIterator)
      {

            if(pIterator != pInvdMasks[i])
               printf("-|%09ld|-", pIterator->pBerth->Boat.user_id);
            else
               printf("*|%09ld|*", pIterator->pBerth->Boat.user_id);
  
         pIterator  = pIterator->pNext;
      }
      if(pInvdHeaders[i])      
         printf("\n\r");
   }
}



