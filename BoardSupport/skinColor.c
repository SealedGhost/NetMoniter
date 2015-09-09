#include "skinColor.h"





const MapWin_COLOR mapSkins[2] = { 
                                      {
   ///                                 bkColor     ttl_bk    ttl_label  ttl_context  ttl_line      map_grid    map_text   
                                       GUI_BLACK, GUI_BLACK, GUI_WHITE, GUI_GREEN,   GUI_WHITE,    GUI_WHITE,  GUI_GRAY,
   ///                                 boat_org   boat_dsp   boat_bgl   boat_drg     boat_tip_bd   boat_name   boat_cdnt
                                       GUI_BLUE,  GUI_WHITE, GUI_RED,   GUI_GREEN,   GUI_DARKBLUE ,GUI_YELLOW, GUI_DARKYELLOW
                                      },
                                      {
   ///                                 bkColor     ttl_bk    ttl_label  ttl_context  ttl_line      map_grid    map_text   
                                       GUI_WHITE, GUI_WHITE, GUI_BLACK, GUI_GREEN,   GUI_BLACK,    GUI_BLACK,  GUI_GRAY,
   ///                                 boat_org   boat_dsp   boat_bgl   boat_drg     boat_tip_bd   boat_name   boat_cdnt
                                       GUI_BLUE,  GUI_BLACK, GUI_RED,   GUI_GREEN,   GUI_DARKBLUE ,GUI_WHITE,  GUI_DARKMAGENTA
                                      }
                                 };
                                            
const MenuWin_COLOR menuWinSkins[2]  = {
///                                         bkColor     menu_label  bt_Sel         bt_Unsle      bt_text    bt_Focus
                                          { GUI_BLACK, GUI_WHITE,  GUI_DARKGREEN,  GUI_DARKGRAY, GUI_WHITE, GUI_GRAY },
///                                          
                                          { GUI_WHITE, GUI_BLACK,  GUI_GRAY,       GUI_LIGHTGRAY,GUI_BLACK, GUI_DARKGRAY }                                          
                                        }; 

const LVWin_COLOR lvWinSkins[2]  = {
///                              bkColor     winLabel    lv_bkHeader   lv_txHeader  lv_gdHeader      lv_bkUnsel    lv_bkSel   lv_bkFocus      lv_txUnsel      lv_txSel    lv_txFocus      lv_string
                               { GUI_BLACK,  GUI_LIGHTGRAY,  GUI_DARKGREEN, GUI_WHITE,   GUI_LIGHTBLUE,   GUI_DARKGRAY, GUI_GRAY,  GUI_LIGHTBLUE,  GUI_LIGHTGRAY,  GUI_WHITE,  GUI_BLACK, GUI_DARKYELLOW },
                               { GUI_WHITE,  GUI_BLACK,  GUI_DARKGREEN,  GUI_BLACK,   GUI_DARKBLUE,    GUI_LIGHTGRAY,GUI_BLACK, GUI_LIGHTBLUE , GUI_DARKGRAY,   GUI_BLACK,  GUI_DARKYELLOW, GUI_DARKMAGENTA}
                              };  

const MntSetWin_COLOR MntSetWinSkins[2]  = {
///                                            bk            tx         sldBk         sldSlot    sldSlider   sldFocusSlider
                                             { GUI_DARKGRAY, GUI_WHITE, GUI_DARKGRAY, GUI_BLACK, GUI_ORANGE, GUI_YELLOW}, 
                                             { GUI_LIGHTGRAY,GUI_BLACK, GUI_LIGHTGRAY,GUI_WHITE, GUI_MAGENTA, GUI_DARKMAGENTA }
                                       };  

const MntSetting_COLOR MntSettingSkins[2]  = {
///                                            bk            tx         sldBk         sldSlot    sldSlider   sldFocusSlider
                                             { GUI_DARKGRAY, GUI_WHITE, GUI_DARKGRAY, GUI_BLACK, GUI_ORANGE, GUI_YELLOW}, 
                                             { GUI_LIGHTGRAY,GUI_BLACK, GUI_LIGHTGRAY,GUI_WHITE, GUI_MAGENTA, GUI_DARKMAGENTA }
                                       };                              


const SysWin_COLOR SysWinSkins[2]  = {
///                                            bk            tx         sldBk         sldSlot    sldSlider   sldFocusSlider
                                             { GUI_DARKGRAY, GUI_WHITE, GUI_DARKGRAY, GUI_BLACK, GUI_ORANGE, GUI_YELLOW}, 
                                             { GUI_LIGHTGRAY,GUI_BLACK, GUI_LIGHTGRAY,GUI_WHITE, GUI_MAGENTA, GUI_DARKMAGENTA }
                                       };                              