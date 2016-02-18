#include "skinColor.h"



const CnfmWin_COLOR cnfmWinSkins[2]  = {
     ///bkTop       bkBottom    text        bt_bkColor  bt_bkFocus  bt_txColor  bt_txFocus
      { 0x00c8c8c8, 0x00c8c8c8, 0x001d1d1d, 0x00595959, 0x00005dff, 0xeaeaea, 0x00eaeaea },                               
      { 0x00313131, 0x00313131, 0x00FFFFFF, 0x00595959, 0x009c6f00, 0x00eaeaea, 0x00eaeaea}
      //{ 0x00313131, 0x00313131, 0x00eaeaea, 0x00595959, 0x009c6f00, 0x00eaeaea, 0x00eaeaea}
} ;



const MapWin_COLOR mapSkins[2]  = {
///  bkColor       ttl_bkTop    ttl_bkBottom  ttl_Label    ttl_Context  map_Grid    map_tip_Tx  map_tip_Bat    boat_Mom    boat_Org     boat_Dsp    boat_Bgl     boat_DRG    sclColor   net_lab
   { 0x00c3e0e9,   0x00e5f4f7,  0x00819298,   0x002f2f2f,  0x00005dff,  0x00979797, 0x002f2f2f, GUI_DARKGREEN, 0x002f2f2f, 0x00000000,  0x00000000, 0x000000ff,  0x0000ff00, 0x00005dff, 0x007a7a7a },
   { 0x001a1a1a,   0x00313131,  0x00202020,   0x00cccccc,  0x00cf9b18,  0x00323232, 0x00d3d3d3, GUI_GREEN ,    0x00cccccc, 0x00ffffff,  0x00ffffff, 0x000000ff,  0x0000ff00, 0x00cf9b18, 0x00565656 }
// { 0x001a1a1a,   0x00313131,  0x00202020,   0x00cccccc,  0x00cf9b18,  0x00323232, 0x00d3d3d3, 0x0080ffff,  0x00cccccc, 0x00ffffff,  0x00ffffff, 0x000000ff,  0x0000ff00, 0x00cf9b18, 0x00565656 }   
};
                                            

const MenuWin_COLOR menuWinSkins[2]  = {
///  bkColor     tbLineColor ttl_bkTop   ttl_bkBottom  ttl_text   btBkColor   btBkSel     btBkFocus   btTxColor   btTxFocus
   { 0x009baeb7, 0x00ffffff, 0x00404448, 0x0025292c , 0x00eaeaea, 0x009baeb7, 0x007c8b92, 0x00005dff, 0x002f2f2f, 0x00eaeaea},
   { 0x00171717, 0x00000000, 0x002f3233, 0x00252524 , 0x00cccccc, 0x00171717, 0x00303030, 0x009c6f00, 0x00cccccc, 0x00eaeaea}
};


const LVWin_COLOR lvWinSkins[2]  = {
/// bkColor     ttl_text    head_bk     head_text   itm_bkUnsel itm_bkSel   itm_bkFocus itm_txUnsel itm_txSel   itm_txFocus inf_bk      inf_label   inf_txColor
  { 0x00c3e0e9, 0x002f2f2f, 0x00404647, 0x00eaeaea, 0x009baeb7, 0x009baeb7, 0x00005dff, 0x002f2f2f, 0x002f2f2f, 0x00eaeaea, 0x00404647, 0x00eaeaea, 0x00518ff3},
  { 0x00000000, 0x00cccccc, 0x00171717, 0x00cccccc, 0x00202021, 0x00202021, 0x009c6f00, 0x00cccccc, 0x00cccccc, 0x00cccccc, 0x00171717, 0x00cccccc, 0x00ddb857},
};



const MntSetWin_COLOR MntSetWinSkins[2]  = {
/// bk          ttl_label    bt_bk      bt_bkFocus  bt_txColor bt_txFocus   txColr     
  { 0x00404647, 0x00000000, 0x00404647, 0x00005dff, 0x003780f6, 0x00eaeaea, 0x00eaeaea}, 
  { 0x00171717, 0x00cccccc, 0x00171717, 0x00cf9b18, 0x00ddb857, 0x00cccccc, 0x00cccccc} 
};  

const MntSetting_COLOR MntSettingSkins[2]  = {
 /// bk            tx         sldBk         sldSlot    sldSlider   sldFocusSlider
  { 0x00404647, GUI_WHITE, GUI_DARKGRAY, GUI_BLACK, GUI_ORANGE, GUI_YELLOW}, 
  { GUI_LIGHTGRAY,GUI_BLACK, GUI_LIGHTGRAY,GUI_WHITE, GUI_MAGENTA, GUI_DARKMAGENTA }
};                              


const SysWin_COLOR SysWinSkins[2]  = {
///   bk         client        tx         sldBk         sldSlot    sldSlider   sldFocusSlider
  { 0x00c3e0e9, 0x00404647, 0x00eaeaea,  0x00404647, 0x002f2f2f, 0x0006b940, 0x00005dff}, 
  { 0x00000000, 0x00171717, 0x00cccccc,  0x00171717, 0x00404647, 0x0006b940, 0x00cf9b18}
};    


