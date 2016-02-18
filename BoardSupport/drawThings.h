#ifndef _DRAW_THINGS_H
#define _DRAW_THINGS_H

/*---------------------------- includes ---------------------------*/
#include "boat_struct.h"
#include "Config.h"

#define SHOW_OPTION_NAME   (0x01)
#define SHOW_OPTION_LL     (0x02)
#define SHOW_OPTION_SCOG   (0x04)
#define SHOW_OPTION_BAT    (0x08)
#define SHOW_OPTION_DST    (0x10)



extern const GUI_POINT  Points_boat[3];
extern const GUI_POINT  Points_fish[11];
extern const GUI_POINT bPoints[5];
extern const GUI_POINT Points_Mute[6];

void  drawNum(unsigned char num, short x, short y, short penSize);
void setBoatSkin(SKINS skin);
void setShape(BOAT_SHAPES shape);
void getMntWrapPara(long *halfDiff_lg, long* halfDiff_lt, map_scale* pScale);
void setManualView(long lg, long lt, map_scale * pScale);
void setAutoView(long lg,   long lt,  map_scale * pScale);
//void setAutoView(void);
#endif
