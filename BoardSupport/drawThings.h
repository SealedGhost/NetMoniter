#ifndef _DRAW_THINGS_H
#define _DRAW_THINGS_H

/*---------------------------- includes ---------------------------*/
#include "boat_struct.h"
#include "Config.h"

void setBoatSkin(SKINS skin);
void setShape(BOAT_SHAPES shape);
void setView(const long lg, const long lt, const map_scale * pScale);
void setAutoView(void);
#endif
