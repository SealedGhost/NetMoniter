#include "HSD_SLIDER.h"




typedef struct
{
   WIDGET  Widget;
   GUI_COLOR aBkColor[2];
   GUI_COLOR aColor[4];
   int Min,Max,v;
   int Flags;
   int NumTicks;
   I16 Width;
}SLIDER_Obj;


/**********************************************************************
 *
 *               Macros for internal use
 *
 **********************************************************************/
 #define SLIDER_H2P(h) (SLIDER_Obj*) GUI_ALLOC_h2p(h)
 
 
 /**********************************************************************
 *
 *               Static routines
 *
 **********************************************************************/
 /**********************************************************
  *                 _Paint
  *
  */
static void _Paint(SLIDER_Obj* pObj, WM_HWIN hObj)
{
   GUI_RECT r, rFocus, rSlider, rSlot;
   GUI_COLOR  SlotColor, SliderColor;
   int x0, xSize,  Range, NumTicks,hasFocus;
   WIDGET__GetClientRect(&pObj->Widget, &rFocus);
   GUI__ReduceRect(&r, &rFocus, 1);
   NumTicks  = pObj->NumTicks;
   xSize  = r.x1 - r.x0 + 1 - pObj->Width;
   x0     = r.x0 + pObj->Width / 2;
   Range  = pObj->Max - pObj->Min;
   
   if(Range == 0)
   {
      Range  = 1;
   }
   
   
   if(pObj->Widget.State & WIDGET_STATE_FOCUS)
   {
      LCD_SetColor(pObj->aBkColor[1]);
      SlotColor  = pObj->aColor[1];
      SliderColor  = pObj->aColor[3];
      
      hasFocus  = 1;
   }
   else
   {
      LCD_SetColor(pObj->aBkColor[0]);
      SlotColor  = pObj->aColor[0];
      SliderColor  = pObj->aColor[2];
      hasFocus  = 0;
   }
   
   GUI_Clear();
   GUI_FillRectEx(&rFocus);
   
   rSlider     = r;
   rSlider.y0  = 5;
   rSlider.x0  = x0 + (U32)xSize * (U32)(pObj->v - pObj->Min) / Range - pObj->Width / 2;
   rSlider.x1  = rSlider.x0 + pObj->Width;    
   
   LCD_SetColor(SlotColor);
   rSlot.x0  = x0;
   rSlot.x1  = x0 + xSize;
   rSlot.y0  = (rSlider.y0 + rSlider.y1) / 2 - 1;
   rSlot.y1  = rSlot.y0 + 3; 
   GUI_FillRectEx(&rSlot);
   
   if(hasFocus)
   {
      LCD_SetColor(GUI_WHITE);
      GUI_DrawRect(rSlot.x0-1, rSlot.y0-1, rSlot.x1+1,rSlot.y1+1);   
   }
    
   LCD_SetColor(SliderColor);   
   GUI_FillCircle(rSlider.x0+pObj->Width/2,(rSlider.y0+rSlider.y1)/2 , pObj->Width/2);  
   
   
   if(NumTicks > 0)
   {
      LCD_SetColor(GUI_BLACK);
      GUI_SetFont(GUI_FONT_16B_1);
      GUI_SetTextMode(GUI_TM_TRANS);
      GUI_DispDecAt(pObj->v, rSlider.x0+pObj->Width/2-3,(rSlider.y0+rSlider.y1)/2-8, 1);
   }
}


 /**********************************************************
  *                 _OnKey
  *
  */
static void _OnKey(SLIDER_Handle hObj, WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pKeyInfo;

   int Key;
   pKeyInfo  = (const WM_KEY_INFO*)(pMsg->Data.p);
   Key  = pKeyInfo->Key;
   if(pKeyInfo->PressedCnt > 0)
   {
      switch(Key)
      {
         case GUI_KEY_RIGHT:
              HSD_SLIDER_Inc(hObj); 
              break;
         case GUI_KEY_LEFT:
              HSD_SLIDER_Dec(hObj);
              break;
         case GUI_KEY_UP:
              WM_SetFocusOnPrevChild(WM_GetParent(pMsg->hWin));
              break;
         case GUI_KEY_DOWN:
              WM_SetFocusOnNextChild(WM_GetParent(pMsg->hWin));
              break;
         case GUI_KEY_MOVE:
         
         default:
              SLIDER_Callback(pMsg);
              break ;
      }
   }
}



/**********************************************************
 *                 _SLIDER_Callback
 *
 */
void HSD_SLIDER_Callback(WM_MESSAGE * pMsg)
{
   SLIDER_Handle hObj;
   SLIDER_Obj * pObj;
   
   hObj  = pMsg->hWin;
   pObj  = SLIDER_H2P(hObj);
   if(WIDGET_HandleActive(hObj, pMsg) == 0)
   {
      return ;
   }
   
   switch(pMsg->MsgId)
   {
      case WM_PAINT:
           _Paint(pObj, hObj);
           return;
      case WM_KEY:
           _OnKey(hObj, pMsg);        
           break;
   }
   WM_DefaultProc(pMsg);
}



/*********************************************************************
*
*       SLIDER_Create
*/
SLIDER_Handle HSD_SLIDER_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int Id, int WinFlags, int SpecialFlags) {
  return SLIDER_CreateEx(x0, y0, xsize, ysize, hParent, WinFlags, SpecialFlags, Id);
}




/**********************************************************
 *                 HSD_SLIDER_CreateEx
 *
 */
SLIDER_Handle HSD_SLIDER_CreateEx(int x0, int y0, int xSize, int ySize, WM_HWIN hParent,
                                  int WinFlags, int ExFlags, int Id)
{
   SLIDER_Handle hObj;
   
   WM_LOCK();
   hObj  = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, WinFlags, HSD_SLIDER_Callback, sizeof(SLIDER_Obj)-sizeof(WM_Obj));
   if(hObj)
   {
      SLIDER_Obj* pObj  = SLIDER_H2P(hObj);
      U16 InitState;
      InitState  = WIDGET_STATE_FOCUSSABLE;
      if(ExFlags & SLIDER_CF_VERTICAL)
      {
         InitState  |= WIDGET_CF_VERTICAL;
      }
      
      WIDGET__Init(&pObj->Widget, Id, InitState);
      
      pObj->aBkColor[0]  = GUI_GRAY;
      pObj->aBkColor[1]  = GUI_DARKCYAN;
      pObj->aColor[0]  = GUI_BLACK;
      pObj->aColor[1]  = GUI_ORANGE;
      pObj->Width  = 16;
      pObj->Max  = 6;
      pObj->Min  = 0;
      pObj->NumTicks  = -1;
   }
   else
   {
//INFO("Slider create failed!");   
   }
   WM_UNLOCK();
   return hObj;
}     



/*********************************************************************
*
*       SLIDER_CreateIndirect
*/
SLIDER_Handle HSD_SLIDER_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb) {
  SLIDER_Handle  hThis;
  GUI_USE_PARA(cb);;  
  hThis = HSD_SLIDER_CreateEx(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,
                          hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);
  return hThis;
}




/*********************************************************************
*
*       HSD_SLIDER_Dec
*/ 
void HSD_SLIDER_Dec(SLIDER_Handle hObj)                          
{
   SLIDER_Obj * pObj;  
   if(hObj)
   {
      WM_LOCK();
      pObj  = SLIDER_H2P(hObj);
      if(pObj->v > pObj->Min)
      {
         pObj->v--;
         WM_InvalidateWindow(hObj);
         WM_NotifyParent(hObj, WM_NOTIFICATION_VALUE_CHANGED);      
      }
      WM_UNLOCK();
   }
}



/*********************************************************************
*
*       HSD_SLIDER_Inc
*/
void HSD_SLIDER_Inc(SLIDER_Handle hObj) {
  SLIDER_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = SLIDER_H2P(hObj);
    if (pObj->v < pObj->Max) {
      pObj->v++;
      WM_InvalidateWindow(hObj);
      WM_NotifyParent(hObj, WM_NOTIFICATION_VALUE_CHANGED);
    }
    WM_UNLOCK();
  }
}


/*********************************************************************
*
*       HSD_SLIDER_Loop
*/
void HSD_SLIDER_Loop(SLIDER_Handle hObj)
{
   SLIDER_Obj * pObj;
   if(hObj)
   {
      WM_LOCK();
      pObj  = SLIDER_H2P(hObj);  
      pObj->v  = (pObj->v + 1 - pObj->Min) % (pObj->Max - pObj->Min + 1) + pObj->Min;    
      WM_InvalidateWindow(hObj);
      WM_NotifyParent(hObj, WM_NOTIFICATION_VALUE_CHANGED);       
      WM_UNLOCK();       
   }
}




/*********************************************************************
*
*       HSD_SLIDER_SetWidth
*/
void HSD_SLIDER_SetWidth(SLIDER_Handle hObj, int Width) {
  SLIDER_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = SLIDER_H2P(hObj);
    if (pObj->Width != Width) {
      pObj->Width = Width;
      WM_InvalidateWindow(hObj);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       HSD_SLIDER_SetValue
*/
void HSD_SLIDER_SetValue(SLIDER_Handle hObj, int v) {
  SLIDER_Obj* pObj;
  if (hObj) {
    WM_LOCK();
    pObj = SLIDER_H2P(hObj);
    /* Put in min/max range */
    if (v < pObj->Min) {
      v = pObj->Min;
    }
    else if (v > pObj->Max) {
      v = pObj->Max;
    }
    if (pObj->v != v) {
      pObj->v = v;
      WM_InvalidateWindow(hObj);
      WM_NotifyParent(hObj, WM_NOTIFICATION_VALUE_CHANGED);
    }
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       HSD_SLIDER_SetRange
*/
void HSD_SLIDER_SetRange(SLIDER_Handle hObj, int Min, int Max) {
  if (hObj) {
    SLIDER_Obj* pObj;
    WM_LOCK();
    pObj = SLIDER_H2P(hObj);
    if (Max < Min) {
      Max = Min;
    }
    pObj->Min = Min;
    pObj->Max = Max;
    if (pObj->v < Min) {
      pObj->v = Min;
    }
    if (pObj->v > Max) {
      pObj->v = Max;
    }
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       HSD_SLIDER_SetNumTicks
*/
void HSD_SLIDER_SetNumTicks(SLIDER_Handle hObj, int NumTicks) {
  if (hObj && (NumTicks >= 0)) {
    SLIDER_Obj* pObj;
    WM_LOCK();
    pObj = SLIDER_H2P(hObj);
    pObj->NumTicks = NumTicks;
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}

/*********************************************************************
*
*       HSD_SLIDER_SetBkColor
*/
void HSD_SLIDER_SetBkColor(SLIDER_Handle hObj, GUI_COLOR Color) {
  if (hObj) {
    SLIDER_Obj * pObj;
    WM_LOCK();
    pObj = SLIDER_H2P(hObj);
    pObj->aBkColor[0] = Color;
    #if SLIDER_SUPPORT_TRANSPARENCY
      if (Color <= 0xFFFFFF) {
        WM_ClrHasTrans(hObj);
      } else {
        WM_SetHasTrans(hObj);
      }
    #endif
    WM_InvalidateWindow(hObj);
    WM_UNLOCK();
  }
}


/*********************************************************************
*
*       HSD_SLIDER_SetFocusBkColor
*/
void HSD_SLIDER_SetFocusBkColor(SLIDER_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      SLIDER_Obj * pObj;
      WM_LOCK();
      pObj  = SLIDER_H2P(hObj);
      pObj->aBkColor[1]  = Color;
      #if SLIDER_SUPPORT_TRANSPARENCY
        if (Color <= 0xFFFFFF) {
          WM_ClrHasTrans(hObj);
        } else {
          WM_SetHasTrans(hObj);
        }
      #endif
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();      
   }
}



/*********************************************************************
*
*       HSD_SLIDER_SetSlotColor
*/
void HSD_SLIDER_SetSlotColor(SLIDER_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      SLIDER_Obj * pObj;
      WM_LOCK();
      pObj  = SLIDER_H2P(hObj);
      pObj->aColor[0]  = Color;
      #if SLIDER_SUPPORT_TRANSPARENCY
        if (Color <= 0xFFFFFF) {
          WM_ClrHasTrans(hObj);
        } else {
          WM_SetHasTrans(hObj);
        }
      #endif
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();      
   }
}



/*********************************************************************
*
*       HSD_SLIDER_SetFocusSlotColor
*/
void HSD_SLIDER_SetFocusSlotColor(SLIDER_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      SLIDER_Obj * pObj;
      WM_LOCK();
      pObj  = SLIDER_H2P(hObj);
      pObj->aColor[1]  = Color;
      #if SLIDER_SUPPORT_TRANSPARENCY
        if (Color <= 0xFFFFFF) {
          WM_ClrHasTrans(hObj);
        } else {
          WM_SetHasTrans(hObj);
        }
      #endif
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();      
   }
}



/*********************************************************************
*
*       HSD_SLIDER_SetSliderColor
*/
void HSD_SLIDER_SetSliderColor(SLIDER_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      SLIDER_Obj * pObj;
      WM_LOCK();
      pObj  = SLIDER_H2P(hObj);
      pObj->aColor[2]  = Color;
      #if SLIDER_SUPPORT_TRANSPARENCY
        if (Color <= 0xFFFFFF) {
          WM_ClrHasTrans(hObj);
        } else {
          WM_SetHasTrans(hObj);
        }
      #endif
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();      
   }
}



/*********************************************************************
*
*       HSD_SLIDER_SetFocusSliderColor
*/
void HSD_SLIDER_SetFocusSliderColor(SLIDER_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      SLIDER_Obj * pObj;
      WM_LOCK();
      pObj  = SLIDER_H2P(hObj);
      pObj->aColor[3]  = Color;
      #if SLIDER_SUPPORT_TRANSPARENCY
        if (Color <= 0xFFFFFF) {
          WM_ClrHasTrans(hObj);
        } else {
          WM_SetHasTrans(hObj);
        }
      #endif
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();      
   }
}




/*********************************************************************
*
*       Query state
*
**********************************************************************
*/
/*********************************************************************
*
*       HSD_SLIDER_GetValue
*/
int HSD_SLIDER_GetValue(SLIDER_Handle hObj) {
  int r = 0;
  SLIDER_Obj* pObj;
  
  if (hObj) 
  { 
     WM_LOCK();
     pObj = SLIDER_H2P(hObj);    
     r = pObj->v;    
     WM_UNLOCK();    
  }
  return r;
}
