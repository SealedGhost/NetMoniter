#ifndef _HSD_BUTTON_H
#define _HSD_BUTTON_H

#include "BUTTON.h"
#include "WM.h"
#include "WIDGET.h"

#define BUTTON_H2P(h) (BUTTON_Obj*) GUI_ALLOC_h2p(h)

//void HSD_BUTTON_SetText(BUTTON_Handle hObj, const char* s) ;
//void HSD_BUTTON_SetVal(BUTTON_Handle hObj, I16 val);
//void HSD_BUTTON_SetFont(BUTTON_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont);
//void HSD_BUTTON_SetValFont(BUTTON_Handle hObj, const GUI_FONT GUI_UNI_PTR * pfont);
//void HSD_BUTTON_SetBkColor(BUTTON_Handle hObj, GUI_COLOR Color) ;
//void HSD_BUTTON_SetFocusBkColor(BUTTON_Handle hObj, GUI_COLOR Color) ;
//void HSD_BUTTON_SetTextColor(BUTTON_Handle hObj,GUI_COLOR Color);
//void HSD_BUTTON_SetFocusTextColor(BUTTON_Handle hObj,GUI_COLOR Color) ;
//void HSD_BUTTON_SetValColor(BUTTON_Handle hObj,GUI_COLOR Color);
//void HSD_BUTTON_SetFocusValColor(BUTTON_Handle hObj,GUI_COLOR Color);

BUTTON_Handle HSD_BUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
void HSD_BUTTON_Callback(WM_MESSAGE * pMsg);
void HSD_BUTTON_SetText(BUTTON_Handle hObj, const char * s) ;
void HSD_BUTTON_SetValue(BUTTON_Handle hObj, const int val);
void HSD_BUTTON_SetBkColor(BUTTON_Handle hObj, GUI_COLOR Color);
void HSD_BUTTON_SetTBLineColor(BUTTON_Handle hObj, GUI_COLOR Color);
void HSD_BUTTON_SetFocusBkColor(BUTTON_Handle hObj, GUI_COLOR Color);
void HSD_BUTTON_SetTextColor(BUTTON_Handle hObj, GUI_COLOR Color);
void HSD_BUTTON_SetTextFocusColor(BUTTON_Handle hObj, GUI_COLOR Color);
void HSD_BUTTON_SetVColor(BUTTON_Handle hObj, GUI_COLOR Color);
void HSD_BUTTON_SetVFocusColor(BUTTON_Handle hObj, GUI_COLOR Color);
void HSD_BUTTON_SetTxFont(BUTTON_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont);
void HSD_BUTTON_SetVFont(BUTTON_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont);
#endif



