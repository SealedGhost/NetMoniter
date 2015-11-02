#include "HSD_BUTTON.h"
#include "Config.h"
#include "str.h"
#include "stdio.h"

typedef struct {
  WIDGET Widget;
  GUI_COLOR bkColor[3];
  GUI_COLOR txColor[2];
  GUI_COLOR vColor[2];
  I16 Align;
  const GUI_FONT GUI_UNI_PTR * txFont;
  const GUI_FONT GUI_UNI_PTR * vFont;
  WM_HMEM hpText;
  int v;
  WM_HMEM ahDrawObj[3];

} BUTTON_Obj;


static void _Paint(BUTTON_Obj * pObj, BUTTON_Handle hObj)
{
   const char * s  = NULL;
   int i  ;
   GUI_RECT r,focusRect;
   GUI_COLOR txColor;
   GUI_COLOR vColor;
   
   GUI_GetClientRect(&r);
   
   if(pObj->Widget.State & WIDGET_STATE_FOCUS)
   {
      txColor  =   pObj->txColor[1];
      vColor   =   pObj->vColor[1];
   }
   else
   {
      txColor  =   pObj->txColor[0];
      vColor   =   pObj->vColor[0];
   }
   
   LCD_SetColor(pObj->bkColor[0]);
   GUI_Clear();
   GUI_FillRectEx(&r);
   GUI_SetColor(GUI_GRAY);
//   GUI_DrawRectEx(&r);
   LCD_SetColor(pObj->bkColor[2]);
   GUI_DrawHLine(r.y0, r.x0, r.x1);
   GUI_DrawHLine(r.y1, r.x0, r.x1);
   
   GUI_SetTextMode(GUI_TM_TRANS);   

   if(pObj->hpText)
   {
      s  = (const char*) GUI_ALLOC_h2p(pObj->hpText);
      GUI_SetFont(pObj->txFont);
      LCD_SetColor(txColor);

//      GUI_DispStringAt(s, r.x0, r.y0);
      GUI_DispStringInRect(s, &r, pObj->Align);
   }
  
   if(pObj->v)
   {
      int strX  = 0;
      int strY  = 0;
      
      sprintf(pStrBuf, "%2d", pObj->v);
      
      GUI_SetColor(GUI_RED);
      GUI_FillCircle(r.x1-16,   r.y0+16,  8);
   
      GUI_SetFont(pObj->vFont);
      
      LCD_SetColor(vColor);

      strX  = GUI_GetStringDistX(pStrBuf);
      strY  = GUI_GetYDistOfFont(pObj->vFont);
      GUI_DispStringAt(pStrBuf, r.x1-16-strX/2, r.y0+16-strY/2    );   
   }
}



 /**********************************************************
  *                 _OnKey
  *
  */
static void _OnKey(BUTTON_Handle hObj, WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pKeyInfo;
   WM_MESSAGE myMsg;
   int Key;
   pKeyInfo  = (const WM_KEY_INFO*)(pMsg->Data.p);
   Key  = pKeyInfo->Key;
   if(pKeyInfo->PressedCnt > 0)
   {
      switch(Key)
      {
         case GUI_KEY_UP:
              WM_SetFocusOnPrevChild(WM_GetParent(pMsg->hWin));
              break;
         case GUI_KEY_DOWN:
              WM_SetFocusOnNextChild(WM_GetParent(pMsg->hWin));
              break;

         default:
              BUTTON_Callback(pMsg);
              break ;
      }
   }
}


void HSD_BUTTON_Callback(WM_MESSAGE * pMsg)
{
   BUTTON_Handle hObj  = pMsg->hWin;
   BUTTON_Obj * pObj   = BUTTON_H2P(hObj);
   
   if(WIDGET_HandleActive(hObj, pMsg) == 0)
   {
      return ;
   }
   
   switch(pMsg->MsgId)
   {
      case WM_PAINT:
          _Paint(pObj, hObj);
           return ;
      case WM_KEY:
           _OnKey(hObj, pMsg);     
           return ;
      
   }
   WM_DefaultProc(pMsg);
}


BUTTON_Handle HSD_BUTTON_CreateEx(int x0, int y0, int xSize, int ySize, WM_HWIN hParent,
                                  int WinFlags,  int ExFlags, int Id)
{
   BUTTON_Handle hObj;
   
   WM_LOCK();
   hObj  = WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, WinFlags, HSD_BUTTON_Callback, sizeof(BUTTON_Obj) - sizeof(WM_Obj));
   
   if(hObj)
   {
      BUTTON_Obj * pObj  = BUTTON_H2P(hObj);
      WIDGET__Init(&pObj->Widget, Id, WIDGET_STATE_FOCUSSABLE);
      pObj->bkColor[0]  = GUI_WHITE;
      pObj->bkColor[1]  = GUI_GRAY;
      pObj->txColor[0]  = GUI_BLACK;
      pObj->txColor[1]  = GUI_DARKGRAY;
      pObj->vColor[0]   = GUI_BLACK;
      pObj->vColor[1]   = GUI_DARKGRAY;
      pObj->txFont  = &GUI_Font24_1;
      pObj->vFont   = GUI_FONT_13B_1;
      pObj->Align  = GUI_TA_HCENTER | GUI_TA_VCENTER;
   }
   else
   {
INFO("BUTTON_Cretae failed");      
   }
   
   WM_UNLOCK();
   return hObj;
} 


BUTTON_Handle HSD_BUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb )
{
   BUTTON_Handle hThis;
   
   GUI_USE_PARA(cb);
   
   hThis  = HSD_BUTTON_CreateEx(pCreateInfo->x0+x0, pCreateInfo->y0+y0,
                                pCreateInfo->xSize, pCreateInfo->ySize,
                                hWinParent, 0, pCreateInfo->Flags, pCreateInfo->Id);

   return hThis;                                
}
  


void HSD_BUTTON_SetText(BUTTON_Handle hObj, const char * s)  
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      if( GUI__SetText(&pObj->hpText, s) )
      {
         BUTTON_Invalidate(hObj);
      }
      WM_UNLOCK();
   }
}


void HSD_BUTTON_SetValue(BUTTON_Handle hObj, const int val)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      if(pObj->v != val)
      {
         pObj->v  = val;
         BUTTON_Invalidate(hObj);
      }
      WM_UNLOCK();
   }
}


void HSD_BUTTON_SetTxFont(BUTTON_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->txFont  = pFont;
   }
   WM_UNLOCK();
   
}


void HSD_BUTTON_SetVFont(BUTTON_Handle hObj, const GUI_FONT GUI_UNI_PTR * pFont)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->vFont  = pFont;
   }
   WM_UNLOCK();
   
}



void HSD_BUTTON_SetBkColor(BUTTON_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->bkColor[0]  = Color;
      WM_InvalidateWindow(hObj); 
      WM_UNLOCK();
   }

}


void HSD_BUTTON_SetTBLineColor(BUTTON_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->bkColor[2]  = Color;
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();
   }
}


void HSD_BUTTON_SetFocusBkColor(BUTTON_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->bkColor[1]  = Color;
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();
   }
  
}


void HSD_BUTTON_SetTextColor(BUTTON_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->txColor[0]  = Color;
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();
   }
}


void HSD_BUTTON_SetTextFocusColor(BUTTON_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->txColor[1]  = Color;
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();
   }
}



void HSD_BUTTON_SetVColor(BUTTON_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->vColor[0]  = Color;
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();
   }
}



void HSD_BUTTON_SetVFocusColor(BUTTON_Handle hObj, GUI_COLOR Color)
{
   if(hObj)
   {
      BUTTON_Obj * pObj;
      WM_LOCK();
      pObj  = BUTTON_H2P(hObj);
      pObj->vColor[1]  = Color;
      WM_InvalidateWindow(hObj);
      WM_UNLOCK();
   }
}



