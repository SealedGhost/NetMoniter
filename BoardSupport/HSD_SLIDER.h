#ifndef HSD_SLIDER_H
#define HSD_SLIDER_H

#include "SLIDER.h"
#include "WM.h"
#include "WIDGET.h"


#define GUI_KEY_MOVE   40



/**********************************************************************
 *
 *               Create functions
 *
 **********************************************************************/
SLIDER_Handle HSD_SLIDER_Create        (int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int WinFlags, int SpecialFlags);
SLIDER_Handle HSD_SLIDER_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);
SLIDER_Handle HSD_SLIDER_CreateEx      (int x0, int y0, int xsize, int ysize, WM_HWIN hParent,
                                    int WinFlags, int ExFlags, int Id);


/**********************************************************************
 *
 *               Member functions
 *
 **********************************************************************/
void HSD_SLIDER_Callback(WM_MESSAGE * pMsg);
void HSD_SLIDER_Inc         (SLIDER_Handle hObj);
void HSD_SLIDER_Dec         (SLIDER_Handle hObj);
void HSD_SLIDER_Loop        (SLIDER_Handle hObj);
void HSD_SLIDER_SetBkColor  (SLIDER_Handle hObj, GUI_COLOR Color);
void HSD_SLIDER_SetFocusBkColor(SLIDER_Handle hObj, GUI_COLOR Color);
void HSD_SLIDER_SetSlotColor(SLIDER_Handle hObj, GUI_COLOR Color);
void HSD_SLIDER_SetFocusSlotColor(SLIDER_Handle hObj, GUI_COLOR Color);
void HSD_SLIDER_SetSliderColor(SLIDER_Handle hObj, GUI_COLOR Color);
void HSD_SLIDER_SetFocusSliderColor(SLIDER_Handle hObj, GUI_COLOR Color);
void HSD_SLIDER_SetWidth    (SLIDER_Handle hObj, int Width);
void HSD_SLIDER_SetValue    (SLIDER_Handle hObj, int v);
void HSD_SLIDER_SetRange    (SLIDER_Handle hObj, int Min, int Max);
void HSD_SLIDER_SetNumTicks (SLIDER_Handle hObj, int NumTicks);





/**********************************************************************
 *
 *               Query state
 *
 **********************************************************************/
int HSD_SLIDER_GetValue(SLIDER_Handle hObj);

#endif 







