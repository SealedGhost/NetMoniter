#ifndef  LIST_OF_BOATS_H
#define  LIST_OF_BOATS_H

#include "boat_struct.h"
#include "analysis_function.h"
#include "uart.h"
boat* find(boat** aux_boat, int size, int id);
_boat_m24A* find_24A(_boat_m24A ** aux_boat_24A,int size,int id);
_boat_m24B* find_24B(_boat_m24B ** aux_boat_24B,int size,int id);

void resolve(boat** aux_boat, boat*  boats, boat** startPos, boat** endPos, int max_size, int* N);
void resolve_24A(_boat_m24A **aux_boat, _boat_m24A * boats_24A, _boat_m24A **startPos,_boat_m24A **endPos,int max_size,int *N);
void resolve_24B(_boat_m24B **aux_boat, _boat_m24B * boats_24B, _boat_m24B **startPos,_boat_m24B **endPos,int max_size,int *N);

void insert (boat*  boats, boat** startPos, boat** endPos, int max_size, struct message_18 * message);
void insert_24A(_boat_m24A* boats_24A, _boat_m24A** startPos,_boat_m24A** endPos,int max_size,struct message_24_partA *message);
void insert_24B(_boat_m24B* boats_24B, _boat_m24B** startPos,_boat_m24B** endPos,int max_size,struct type_of_ship *message);

#endif
