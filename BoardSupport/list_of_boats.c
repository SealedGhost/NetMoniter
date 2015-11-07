 #include "boat_struct.h"
#include "analysis_function.h"
#include "uart.h"
#include "Config.h"
#include "string.h"
#include <math.h>
#include <stdlib.h>
#include "Setting.h"
#include "invader.h"
// find if id exist in aux_boat, yes return 1, otherwise 0, size exclusive

/*----------------------- Macro defination -----------------------*/
#define alloc_boat_mem (_boat**)malloc(sizeof(_boat*)*5000);

#define NOT_ALMOST(x, y)  (x>y?(x-y>2):(y-x>2))

#define PI 3.14


/*----------------------- external variables ---------------------*/
extern int N_boat;
extern int list_endIndex;
extern int myCnt;
extern char * pStrBuf;
extern _boat* boat_list_p[BOAT_NUM_MAX];
extern boat fuckership;
extern BERTH Berthes[BOAT_NUM_MAX];
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
extern MNT_BERTH * pMntHeader;
extern Bool INVD_deleteByMMSI(long MMSI);
/*----------------------- external functions -----------------------*/



/*----------------------- local variables -------------------------*/
const unsigned int  R_KM  = 6371;
const unsigned int  R_NM  = (unsigned int)(0.54*R_KM);
const float         LLTOA = PI/60000/180;

BERTH * pHeader  = NULL;
BERTH * pTail    = NULL;
/*----------------------- local functions --------------------------*/
int insert_18(struct message_18 * p_msg);
int update_18(BERTH * pBerth, struct message_18 * p_msg);
int add_18(struct message_18 * p_msg);

int insert_24A(struct message_24_partA * p_msg);
int update_24A(BERTH * pBerth, struct message_24_partA * p_msg);
int add_24A(struct message_24_partA * p_msg);

int insert_24B(type_of_ship * p_msg);
int update_24B(BERTH * pBerth, type_of_ship * p_msg);
int add_24B(type_of_ship * p_msg);

static int getSphereDist(long lg_1,long lt_1, long lg_2, long lt_2);
void updateTimeStamp(void);
static BERTH * allocOneBerth(void);

/**
* 
*
*/
int insert_18(struct message_18 * p_msg)
{
   int i  = 0; 
   
   /// Give up berthes out of range .
   if( (p_msg->longitude < fuckership.longitude-30000)  ||  (p_msg->longitude > fuckership.longitude+30000) ) 
        return 0;
   if( (p_msg->latitude < fuckership.latitude-30000)  ||  (p_msg->latitude > fuckership.latitude+30000) )
        return 0; 
   /// Update existent berth
   for(i=0;i<BOAT_NUM_MAX;i++)
   {
      if(Berthes[i].Boat.user_id == p_msg->user_id)
      {
         /// We regard little offset as non-move ,just for optimizaton
          if(   NOT_ALMOST(p_msg->longitude,Berthes[i].Boat.longitude)  
             || NOT_ALMOST(p_msg->latitude,Berthes[i].Boat.latitude)    ) 
          {
               
               if(update_18(&(Berthes[i]), p_msg))
               {
                  return 1;               
               }

               else
                  return -1;
          }
          else
          {
             return 0;
          }
      }
   }
   
   /// Add non-existent berth
   if(add_18(p_msg))
   {
      return 1;
   }
   return -1;
}



int insert_24A(struct message_24_partA * p_msg)
{
   int i  = 0;
   
   if(0 == p_msg->user_id)
      return 0;     
   /// This Id exist
   for(i=0;i<BOAT_NUM_MAX;i++)
   {
      /// Update existent berth and add nonexistent one
      if(Berthes[i].Boat.user_id == p_msg->user_id)
      {
         if(update_24A(&(Berthes[i]), p_msg))
         {        
            return  1;
         }
          else
          {
             return -1;
          }
      }
   }
   
   if(add_24A(p_msg))
   { 
      return 1;
   }
   return -1;
}


int insert_24B(type_of_ship * p_msg)
{
   int i  = 0;
   
   if(0 == p_msg->user_id)
      return 0;
   
   for(i=0; i<BOAT_NUM_MAX; i++)
   {
      if(Berthes[i].Boat.user_id == p_msg->user_id)
      {
         if(update_24B(&Berthes[i], p_msg))
         {
            return 1;
         }
         else
         {
            return -1;
         }
      }
   }
   
   if(add_24B(p_msg))
   {
      return 1;
   }
   return  -1;
}





int update_18(BERTH * pBerth, struct message_18 * p_msg)
{
   int lastDist  = 0;
   int Dist  = 0;
   
   BERTH * tmp  = NULL;   
   lastDist  = pBerth->Boat.dist;



   /// Update struct elements  
   pBerth->Boat.SOG    = p_msg->SOG;
   pBerth->Boat.COG    = p_msg->COG;
   pBerth->Boat.true_heading  = p_msg->true_heading;
   pBerth->Boat.longitude     = p_msg->longitude;
   pBerth->Boat.latitude      = p_msg->latitude;
   
   Dist = getSphereDist(p_msg->latitude,p_msg->longitude,
                        fuckership.latitude,fuckership.longitude);  
   pBerth->Boat.dist  = Dist;
   
   pBerth->Boat.time_cnt  = TIMESTAMP;

   tmp  = pBerth;

   /// Distance did not change ,we need do nothing. 
   if(lastDist == Dist)
   {
 
      return 1;
   }   
   /// Distance decrease
   if(Dist < lastDist)
   {
      /// pBerth do not need to change location
      if(pBerth->pLast == NULL)
      {
         return 1; 
      }         
      if(Dist >= pBerth->pLast->Boat.dist)
      {     
         return 1;
      }
      /// pBerth need to change location 
      while(tmp)
      {
         tmp  = tmp->pLast;
         

        /// pBerth->dist is smallest,move it to header
        
 /*        _________________________
  *       /                         \
  *      @here    NODE     NODE    |NODE|    NODE   NODE
  *               tmp                
  */
//         if(tmp->pLast == NULL)
         if(tmp == pHeader)
         {  
            pBerth->pLast->pNext  = pBerth->pNext;
//            if(pBerth->pNext)
            if(pBerth != pTail)
               pBerth->pNext->pLast  = pBerth->pLast;
            else  
               pTail  = pBerth->pLast;
               
            tmp->pLast  = pBerth;
            pBerth->pNext  = tmp;
            pBerth->pLast  = NULL;
            pHeader  = pBerth;
            return 1;
         }
         
         /// Find the correct location and fix pBerth location
         
 /*                _______________________
  *               /                       \
  *      NODE   @here    NODE    NODE   |NODE|    NODE
  *                      tmp
  */
         if(Dist >= tmp->pLast->Boat.dist)
         {        
            pBerth->pLast->pNext  = pBerth->pNext;
//            if(pBerth->pNext)
            if(pBerth != pTail)
               pBerth->pNext->pLast  = pBerth->pLast;
            else
               pTail  = pBerth->pLast;
             
            pBerth->pLast  = tmp->pLast;
            pBerth->pNext  = tmp;
            tmp->pLast->pNext  = pBerth;
            tmp->pLast  = pBerth;
            return 1;
         }
      }
INFO("Err!");      
      return -1;
   }
   
   
   else 
   {
      if(pBerth->pNext == NULL)
      {
         return 1;
      }
      if(Dist <= pBerth->pNext->Boat.dist)
      {     
         return 1;
      }
      
      while(tmp)
      {

      
         tmp  = tmp->pNext;
         
         /// pBerth is biggest, move it to tail
 /*                ____________________________
  *               /                            \
  *       NODE  |NODE|  NODE   NODE   NODE    @here
  *                                    tmp
  */       
         if(tmp == pTail)
         { 
            if(pBerth != pHeader)
               pBerth->pLast->pNext  = pBerth->pNext;
            else
               pHeader  = pBerth->pNext;
            pBerth->pNext->pLast  = pBerth->pLast;
            
            tmp->pNext  = pBerth;
            pBerth->pLast  = tmp;
            pBerth->pNext  = NULL;
            
            pTail  = pBerth;
            return 1;
         }
         
         
         /// pBerth in middle of link list
 /*              ____________________________
  *             /                            \ 
  *   NODE   |NODE|   NODE   NODE   NODE   @here   NODE   NODE
  *                                                 tmp
  */  
         if(Dist <= tmp->pNext->Boat.dist)
         {       
            if(pBerth != pHeader)
               pBerth->pLast->pNext  = pBerth->pNext;
             else
               pHeader  = pBerth->pNext;
             pBerth->pNext->pLast  = pBerth->pLast;
             
             pBerth->pLast  = tmp;
             pBerth->pNext  = tmp->pNext;
             tmp->pNext ->pLast  = pBerth;
             tmp->pNext   = pBerth;
             return 1;
         }
      }
INFO("Err!");      
      return -1;
   }
}


int add_18(struct message_18 * p_msg)   
{
   BERTH * buf  = NULL;
   BERTH * tmp  = NULL;
   
   int Dist  = 0;

   buf  = allocOneBerth();
   if(buf == NULL) 
   {
INFO("alloc berth failed!");   
      return -1;
   }
   
   buf->Boat.user_id  = p_msg->user_id;
   buf->Boat.SOG      = p_msg->SOG;
   buf->Boat.COG      = p_msg->COG;
   buf->Boat.true_heading    = p_msg->true_heading;
   buf->Boat.longitude       = p_msg->longitude;
   buf->Boat.latitude        = p_msg->latitude;
   Dist  = getSphereDist(p_msg->latitude, p_msg->longitude,
                         fuckership.latitude, fuckership.longitude);
   buf->Boat.dist  = Dist;
   buf->Boat.time_cnt  = TIMESTAMP;
   
   if(pHeader == NULL)
   {
      pHeader = buf;
      pTail   = buf;      
      return 1;
   }
   
 /// Insert at header
 /**   |
  *    V
  *  @here    NODE     NODE     NODE     NODE
  *          header   
  */  
   if(Dist <= pHeader->Boat.dist)
   {
      buf->pNext  = pHeader;
      pHeader->pLast  = buf;
      pHeader  = buf;     
      return 1;
   }
   
   tmp  = pHeader;
   while(tmp)
   {
 /// Insert at tail
 /**                                    |
  *                            tail     V
  *   NODE    NODE    NODE    NODE    @here
  *                            tmp     
  */      
      if(tmp == pTail)
      {    
         tmp->pNext  = buf;
         buf->pLast  = tmp;
         buf->pNext  = NULL;
         
         pTail  = buf;
        
         return 3;
      }
      
 /// Insert at middle  
 /**                           |
  *                            V
  *   NODE    NODE    NODE   @here    NODE   NODE
  *                    tmp                   
  */  
      if(Dist <= tmp->pNext->Boat.dist)
      {       
         buf->pNext  = tmp->pNext;
         buf->pLast  = tmp;
         tmp->pNext->pLast  = buf;
         tmp->pNext  = buf;
       
         return 2;
      }
      
      tmp  = tmp->pNext;
   }
   
   return -1;
}



int update_24A(BERTH * pBerth, struct message_24_partA * p_msg)
{
   int i  = 0; 

   pBerth->Boat.time_cnt  = TIMESTAMP; 
   if(pBerth->Boat.name[0] == 0)
   {
      for(i=0;i<20;i++)
      {
         pBerth->Boat.name[i]  = p_msg->name[i];
         if(p_msg->name[i] == '\0')
         {
            break;
         }
      }
      pBerth->Boat.name[19]  = '\0';
   }
   else
   {
      return 0;      
   }
   
   return 1;
}


int add_24A(struct message_24_partA * p_msg)
{
   BERTH * buf  = NULL;

   int i  = 0;
   
 
   buf  = allocOneBerth();
   
   if(buf == NULL)
   {
INFO("alloc berth failed!");   
      return -1;
   }
//   buf->pLast  = NULL;
//   buf->pNext  = NULL;
   
   buf->Boat.user_id  = p_msg->user_id;
   buf->Boat.dist  = 999999;
   buf->Boat.time_cnt  = TIMESTAMP;
   
   for(i=0;i<20;i++)
   {
      buf->Boat.name[i]  = p_msg->name[i];
      if(p_msg->name[i] == '\0')
      {
         break;
      }
   }
   buf->Boat.name[19]  = '\0';
   
   if(pHeader == NULL)
   {
      pHeader  = buf;
      pTail    = buf;
      return 1;
   }
  
  /***     Add at tail  
   *                                   |
   *                                   V
   *  NODE    NODE    NODE    NODE   @here
   *                           tail
   */                       
   pTail->pNext  = buf;
   buf->pLast    = pTail;
   
   buf->pNext  = NULL;
   pTail  = buf;

   return 1;
}


int update_24B(BERTH * pBerth, type_of_ship * p_msg)
{
   pBerth->Boat.time_cnt  = TIMESTAMP;

   if(    p_msg->vender_id[0] == 8
       && p_msg->vender_id[1] == 19
       && p_msg->vender_id[2] == 4)
   {
   
      pBerth->Boat.isHSD  = TRUE;
      return 0;
   }
   
   return 1;
}



int add_24B(type_of_ship * p_msg)
{
   BERTH * buf  = NULL;
   
 
   buf  = allocOneBerth();
   
   if(buf == NULL)
   {
INFO("alloc berth failed!");      
       return -1;
   }
   
   buf->Boat.user_id  = p_msg->user_id;
   buf->Boat.dist  = 999999;
   buf->Boat.time_cnt   = TIMESTAMP;

   
   if(    p_msg->vender_id[0] == 8
       && p_msg->vender_id[1] == 19
       && p_msg->vender_id[2] == 4)
   {
      buf->Boat.isHSD  = TRUE;
   }
   buf->Boat.isHSD  = TRUE;
   
   if(pHeader == NULL)
   {
      pHeader  = buf;
      pTail    = buf;
      return 1;
   }
   
   pTail->pNext  = buf;
   buf->pLast    = pTail;
   
   buf->pNext  = NULL;
   pTail       = buf;
   
   return 1;
}



void updateTimeStamp()
{
//   MNT_BERTH * pIterator  = NULL;
   BERTH * tmp  = NULL;
     
   BERTH * pCur  = NULL;
   int i  = 0;  
   pCur  = pHeader; 
   
   while(pCur)
   {
      if(pCur->Boat.time_cnt > 0)
      {
         SimpBerthes[i].latitude   = pCur->Boat.latitude;
         SimpBerthes[i].longitude  = pCur->Boat.longitude;
         SimpBerthes[i].Dist       = pCur->Boat.dist;
         SimpBerthes[i].pBerth     = pCur;
         
         pCur->Boat.time_cnt--;                   
         pCur  = pCur->pNext;
         i++;
      }
      else
      { 
//printf("Delete %09ld\n\r", pCur->Boat.user_id); 
//         pIterator  = pMntHeader;
//         while(pIterator)
//         {
//            if(pIterator->mntBoat.mmsi == pCur->Boat.user_id)
//            {
//               pIterator->pBoat  = NULL;
//               break;
//            }
//            else
//            {
//               pIterator  = pIterator->pNext;
//            }
//         }
         if(pCur->isInvader)
         {
printf("deleteAddr\n");         
            INVD_deleteByAddr(pCur);    
         }
         /// Delete at header
         if(pCur == pHeader)
         {
            pHeader  = pCur->pNext;
            if(pCur->pNext)
               pCur->pNext->pLast  = NULL;          
         }
         ///  Delete at middle
         else if(pCur->pNext)
         {
            pCur->pLast->pNext  = pCur->pNext;
            pCur->pNext->pLast  = pCur->pLast;
         }
         /// Delete at tail
         else
         {

            pCur->pLast->pNext  = NULL;
            pTail  = pCur->pLast;
         }
         tmp  = pCur->pNext;
         
         memset((void*)pCur, 0, sizeof(BERTH));
         
         pCur  = tmp;
        
      }

   }
   N_boat  = i;    


//   for(i=0;i<BOAT_NUM_MAX;i++)
//   {
//      if(Berthes[i].Boat.user_id != 0)
//      {
//         k++;
//      }
//   } 
//  if(N_boat != k)
//  {
//     mymyCnt++;
//     INFO("N_boat:%d < true N_Boat:%d %d err happend",N_boat,k,mymyCnt);
//  }
}


static int getSphereDist(long lt_1,long lg_1, long lt_2, long lg_2)
{
	float dist = 0.0;
	float f_1 = 1.0*lt_1 / 60000;
	float f_2 = 1.0*lt_2 / 60000;

	float diff = 1.0*(lg_1 - lg_2) / 60000;
 float cosTheta  = 0.0;
// f_1  = lt_1/60000 + ( (lt_1%60000)/10000 )*0.01667;

// printf("\r\nf_1:%lf\r\n",f_1);

// f_2  = 2.12345;
// printf("\r\nf_2:%lf\r\n",f_2);
// 
// tmp  = cos((f_1-f_2)*PI/180);
// printf("\r\ntmp:%f",tmp);
// 
// 
 if( (lt_1 == 0) || (lg_1 == 0) )
 {
    return 999999;
 }
 
 if( (lt_1>lt_2?lt_1-lt_2:lt_2-lt_1) <500  &&  (lg_1>lg_2?lg_1-lg_2:lg_2-lg_1) < 500 )
 {
    dist  = sqrt( (lt_1-lt_2)*(lt_1-lt_1) + (lg_1-lg_2)*(lg_1-lg_2));
    return (int)(dist);
 }
 
// cosTheta  =   cos((f_1 - f_2)*PI / 180) / 2
//             - cos((f_1 + f_2)*PI / 180) / 2
//             + cos((diff + f_1 + f_2)*PI / 180) / 4
//             + cos((diff + f_1 - f_2)*PI / 180) / 4
//             + cos((diff - f_1 + f_2)*PI / 180) / 4
//             + cos((diff - f_1 - f_2)*PI / 180) / 4  ;
//             
// if(cosTheta > 1) 
//    cosTheta = 1;
// else if(cosTheta < -1) 
//    cosTheta = -1;
// 
// dist  = 6371 *0.54 * acos(cosTheta);
 
	dist = 6371 *0.54 * acos(cos((f_1 - f_2)*PI / 180) / 2
		- cos((f_1 + f_2)*PI / 180) / 2
		+ cos((diff + f_1 + f_2)*PI / 180) / 4
		+ cos((diff + f_1 - f_2)*PI / 180) / 4
		+ cos((diff - f_1 + f_2)*PI / 180) / 4
		+ cos((diff - f_1 - f_2)*PI / 180) / 4
		);
 return (dist*1000);
}


static BERTH * allocOneBerth()
{
   int i  = 0;
   for(i=0;i<BOAT_NUM_MAX;i++)
   {
      if(Berthes[i].Boat.user_id == 0)
      {         
         return &(Berthes[i]);
      }
   }  
   return NULL;
}



void myPrint()
{
   int i  = 0;
   BERTH * tmp  = NULL;
   
   tmp  = pHeader;
   
   printf("\r\n");
   while(tmp)
   {
      i++;
      printf("[%09ld,%d]->",tmp->Boat.user_id,tmp->Boat.dist);
      if(i>20)
      {
         printf("\r\n");  
         return ;         
      }
      tmp  = tmp->pNext;
   }
   
   printf("\r\n");
}

