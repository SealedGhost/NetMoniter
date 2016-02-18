#include "snap.h"


extern boat mothership;
extern int N_boat;
extern SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];

void*  pSnapLink  = &mothership;
char   snapType   = 0;


static long checkMMSI  = 0;

long SNAP_getSnapObjMMSI(void)
{
   return checkMMSI;
}


void SNAP_reset(void)
{
   pSnapLink  = &mothership;
   snapType   = 0;
   checkMMSI  = 0;
}

void SNAP_getPara(long* pLg, long* pLt)
{
   switch(snapType)
   {
      case 1:
           if( ((BERTH*)pSnapLink)->Boat.user_id == checkMMSI)
           {
              *pLg  = ((BERTH*)pSnapLink)->Boat.longitude;
              *pLt  = ((BERTH*)pSnapLink)->Boat.latitude;
           }
           else
           {
              *pLg  = mothership.longitude;
              *pLt  = mothership.latitude;
           }
           break;
      case 2:
           if( ((MNT_BERTH*)pSnapLink)->pBerth  &&  ((MNT_BERTH*)pSnapLink)->pBerth->Boat.user_id == checkMMSI)
           {
              *pLg  = ((MNT_BERTH*)pSnapLink)->pBerth->Boat.longitude;
              *pLt  = ((MNT_BERTH*)pSnapLink)->pBerth->Boat.latitude;
           }
           else
           {
              *pLg  = ((MNT_BERTH*)pSnapLink)->snapLg;
              *pLt  = ((MNT_BERTH*)pSnapLink)->snapLt;
           }  
           break;
     default:
           *pLg  = mothership.longitude;
           *pLt  = mothership.latitude;
           break;
   }
}

void SNAP_searchNearestObj(short Dir_x, short Dir_y)
{
     long  minDiff  = 99999;
     long  refLg    = 0;
     long  refLt    = 0;
     long  refMMSI  = 0;
     
     void* potentialSnapLink = NULL;
     char  potentialSnapType = 0;
     
     MNT_BERTH* pIterator  = NULL;
     
     int   i;
     
     switch(snapType)
     {
        /// Previous link is berth
        case 1:
             refMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
             if(refMMSI == checkMMSI)
             {
                refLg  = ((BERTH*)pSnapLink)->Boat.longitude;
                refLt  = ((BERTH*)pSnapLink)->Boat.latitude;
             }
             else
             {
                refLg  = mothership.longitude;
                refLt  = mothership.latitude;
                refMMSI = 0;
             }
             break;
             
        /// Previous link is mnt berth
        case 2:
             if( ((MNT_BERTH*)pSnapLink)->pBerth  &&  ((MNT_BERTH*)pSnapLink)->pBerth->Boat.user_id == checkMMSI )
             {
                refLg  = ((MNT_BERTH*)pSnapLink)->pBerth->Boat.longitude;
                refLt  = ((MNT_BERTH*)pSnapLink)->pBerth->Boat.latitude;
             }
             else
             {
                refLg  = ((MNT_BERTH*)pSnapLink)->snapLg;
                refLt  = ((MNT_BERTH*)pSnapLink)->snapLt;
             }
             refMMSI  = checkMMSI;
             break;
       default:
             refLg  = mothership.longitude;
             refLt  = mothership.latitude;
             refMMSI  = 0;
             checkMMSI = 0;
printf("ref snap:mothership\n\r");             
             break;       
     }
     
     
     if(Dir_x == -1)
     {
        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].longitude < refLg  &&  refLg - SimpBerthes[i].longitude < minDiff)
                 {
                    minDiff  = refLg - SimpBerthes[i].longitude;
                    potentialSnapLink  = SimpBerthes[i].pBerth;
                    potentialSnapType  = 1;                   
                 }
              }
           }
        }
        
        pIterator  = pMntHeader;
        while(pIterator)
        {
           if(pIterator->mntBoat.mmsi != refMMSI)
           {
              if(pIterator->cfgState == MNTState_Monitored)
              {
                 if(pIterator->pBerth)
                 {
                    if(pIterator->pBerth->Boat.longitude < refLg  &&  refLg - pIterator->pBerth->Boat.longitude < minDiff)
                    {
                       minDiff  = refLg - pIterator->pBerth->Boat.longitude;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;                      
                    }
                 }
                 else
                 {
                    if(pIterator->snapLg < refLg  &&  refLg - pIterator->snapLg < minDiff)
                    {
                       minDiff  = refLg - pIterator->snapLg;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;                       
                    }
                 }
              }
           }
           
           pIterator  = pIterator->pNext;
        }
        
        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           snapType  = potentialSnapType;
           
           switch(snapType)
           {
              case 1:
                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
                   break;
              case 2:
                   checkMMSI  = ((MNT_BERTH*)pSnapLink)->mntBoat.mmsi;
                   break;
             default:
                   checkMMSI  = 0;
                   break;
           }
        }    
        return ;
     }
     else if(Dir_x == 1)  
     {
        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].longitude > refLg  &&  SimpBerthes[i].longitude - refLg < minDiff)
                 {
                    minDiff  = SimpBerthes[i].longitude - refLg;
                    potentialSnapLink  = SimpBerthes[i].pBerth;
                    potentialSnapType  = 1;               
                 }
              }
           }
        }
        
        pIterator  = pMntHeader;
        while(pIterator)
        {
           if(pIterator->mntBoat.mmsi != refMMSI)
           {
              if(pIterator->cfgState == MNTState_Monitored)
              {
                 if(pIterator->pBerth)
                 {
                    if(pIterator->pBerth->Boat.longitude > refLg  &&  pIterator->pBerth->Boat.longitude - refLg < minDiff)
                    {
                       minDiff  = pIterator->pBerth->Boat.longitude - refLg;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;                      
                    }
                 }
                 else
                 {
                    if(pIterator->snapLg > refLg  &&  pIterator->snapLg - refLg < minDiff)
                    {
                       minDiff  = pIterator->snapLg - refLg;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;                      
                    }
                 }
              }
           }
           
           pIterator  = pIterator->pNext;
        }
        
        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           snapType  = potentialSnapType;
           switch(snapType)
           {
              case 1:
                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
                   break;
              case 2:
                   checkMMSI  = ((MNT_BERTH*)pSnapLink)->mntBoat.mmsi;
                   break;
             default:
                   checkMMSI  = 0;
                   break;
           }           
        }      
        return ;
     }
     else if(Dir_y == 1)
     {

        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].latitude < refLt  &&  refLt - SimpBerthes[i].latitude < minDiff)
                 {
                    minDiff  = refLt - SimpBerthes[i].latitude;
                    potentialSnapLink  = SimpBerthes[i].pBerth;
                    potentialSnapType  = 1;
                 }
              }
           }
        }
        
        pIterator  = pMntHeader;
        while(pIterator)
        {
           if(pIterator->mntBoat.mmsi != refMMSI)
           {
              if(pIterator->cfgState == MNTState_Monitored)
              {
                 if(pIterator->pBerth)
                 {
                    if(pIterator->pBerth->Boat.latitude < refLt  &&  refLt - pIterator->pBerth->Boat.latitude < minDiff)
                    {
                       minDiff  = refLt - pIterator->pBerth->Boat.latitude;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;
                    }
                 }
                 else
                 {
                    if(pIterator->snapLt < refLt  &&  refLg - pIterator->snapLg < minDiff)
                    {
                       minDiff  = refLt - pIterator->snapLg;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;
                    }
                 }
              }
           }
           
           pIterator  = pIterator->pNext;
        }
        
        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           snapType  = potentialSnapType;
           switch(snapType)
           {
              case 1:
                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
                   break;
              case 2:
                   checkMMSI  = ((MNT_BERTH*)pSnapLink)->mntBoat.mmsi;
                   break;
             default:
                   checkMMSI  = 0;
                   break;
           }           
        }
        
        return ;
     } 
     else if(Dir_y == -1)   
     {

        for(i=0; i<N_boat; i++)
        {
           if(SimpBerthes[i].pBerth->isInvader)
           {
              if(SimpBerthes[i].pBerth->Boat.user_id != refMMSI)
              {
                 if(SimpBerthes[i].latitude > refLt  &&  SimpBerthes[i].latitude - refLt < minDiff)
                 {
                    minDiff  = SimpBerthes[i].latitude - refLt < minDiff;
                    potentialSnapLink  = SimpBerthes[i].pBerth;
                    potentialSnapType  = 1;
                 }
              }
           }
        }
        
        pIterator  = pMntHeader;
        while(pIterator)
        {
           if(pIterator->mntBoat.mmsi != refMMSI)
           {
              if(pIterator->cfgState == MNTState_Monitored)
              {
                 if(pIterator->pBerth)
                 {
                    if(pIterator->pBerth->Boat.latitude > refLt  &&  pIterator->pBerth->Boat.latitude - refLt < minDiff)
                    {
                       minDiff  = pIterator->pBerth->Boat.latitude - refLt;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;
                    }
                 }
                 else
                 {
                    if(pIterator->snapLt > refLt  &&  pIterator->snapLt - refLt< minDiff)
                    {
                       minDiff  = pIterator->snapLt - refLt;
                       potentialSnapLink  = pIterator;
                       potentialSnapType  = 2;
                    }
                 }
              }
           }
           
           pIterator  = pIterator->pNext;
        }
        
        if(potentialSnapLink)
        {
           pSnapLink  = potentialSnapLink;
           snapType  = potentialSnapType;
           switch(snapType)
           {
              case 1:
                   checkMMSI  = ((BERTH*)pSnapLink)->Boat.user_id;
                   break;
              case 2:
                   checkMMSI  = ((MNT_BERTH*)pSnapLink)->mntBoat.mmsi;
                   break;
             default:
                   checkMMSI  = 0;
                   break;
           }           
        }
        
        return ;
     }     
}
