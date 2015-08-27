#ifndef DRAWINFORMATION_H
#define DRAWINFORMATION_H

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "DROPDOWN.h"
#include "map_draw.h"

void Createbutton(WM_MESSAGE* pMsg);
void PaintFrame(void);
void CleanText(WM_MESSAGE* pMsg);
void InitText (WM_MESSAGE* pMsg,long user_id,short SOG,short COG,short true_heading,long longitude,long latitude,char *name,int type_of_electronic_position_fixing_device);
void showShipName(WM_MESSAGE* pMsg,char*);
long strtoi(const char*str);
void lltostr(long l_o_l,char *str);
//void GUI_DispStringAt(const unsigned char *str,const int x, const int y);
void TEXT_SetTextEx(TEXT_Handle hObj, const unsigned char* s);

void Draw_ScaleRuler(scale_map scale);

#define SIZE_X   590
#define SIZE_X1  630
#define SIZE_X2  680
#define SIZE_X3  720
#define SIZE_X4  740

#define SPACE    20

#define SPACE1   25
#define SIZE_Y0  3
#define SIZE_Y1  SPACE*1
#define SIZE_Y2  SPACE*2
#define SIZE_Y3  SPACE*3
#define SIZE_Y4  SPACE*4
#define SIZE_Y5  SPACE*5
#define SIZE_Y6  SPACE*6
#define SIZE_Y7  SPACE*7
#define SIZE_Y8  SPACE*8
#define SIZE_Y9  SPACE*9
#define SIZE_Y10  SPACE*10
#define SIZE_Y11  SPACE*11
#define SIZE_Y12  SPACE*12
#define SIZE_Y13  SPACE*13
#define SIZE_Y14  SPACE*14
#define SIZE_Y15  SPACE*15
#define SIZE_Y16  SPACE*16
#define SIZE_Y17  SPACE*17
#define SIZE_Y18  SPACE*18
#define SIZE_Y19  SPACE*19
#define SIZE_Y20  SPACE*20
#define SIZE_Y21  SPACE*21
#define SIZE_Y22  SPACE*22
#define SIZE_Y23  SPACE*23

#define LINE_X   580             //·Ö¸îÏß×ø±ê
#define LINE_Y1  20+5             
#define LINE_Y2  200
#define LINE_Y3  300
#define LINE_Y4  395


/* define TEXT_ID */
#define TEXT_ID_MMSI      GUI_ID_TEXT1
#define TEXT_ID_NAME_PY   GUI_ID_TEXT2//
#define TEXT_ID_NAME_CHS  GUI_ID_TEXT3
#define TEXT_ID_LATITUDE  GUI_ID_TEXT4
#define TEXT_ID_LONGITUDE GUI_ID_TEXT5
#define TEXT_ID_SOG       GUI_ID_TEXT6
#define TEXT_ID_HEADING   GUI_ID_TEXT7
#define TEXT_ID_DSTN      GUI_ID_TEXT8
#define TEXT_ID_LOC       GUI_ID_TEXT9
#define TEXT_ID_OFFSET_WARNING    GUI_ID_TEXT10
#define TEXT_ID_CLOSING_WARNING   GUI_ID_TEXT11
#define TEXT_ID_SOG_WARNING       GUI_ID_TEXT12
#define TEXT_ID_SELF_LATITUDE     GUI_ID_TEXT13
#define TEXT_ID_SELF_LONGITUDE    GUI_ID_TEXT14


#endif
