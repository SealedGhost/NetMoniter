#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H




/**
 *   CONF_SYS Sys_conf;
 *      
 *   Sys_conf.Skin.MapWinColor.Map_Grid  = GUI_RED;
 *
 */
#include "GUI.h"
#include "uart.h"

typedef enum _SKINS SKINS;
enum _SKINS {SKIN_Night=0, SKIN_Day};

typedef enum _UNITS   UNITS;
enum _UNITS{UNIT_km=1, UNIT_nm};

typedef enum _BOAT_SHAPES   BOAT_SHAPES;
enum _BOAT_SHAPES{SHAPE_Fish=0, SHAPE_Boat};



/// Sound config struct.
typedef struct _CONF_SND CONF_SND;
struct _CONF_SND
{
   short Vol;       ///  Volumn.
   short ArmSnd;
   short KeySnd;  
};


/// Light config struct.
typedef struct _CONF_LIGHT CONF_LIGHT;
struct _CONF_LIGHT
{
   short Brightness;
};





/// Unit config struct.
typedef struct _CONF_UNIT CONF_UNIT;
struct _CONF_UNIT
{
  UNITS Unit;
};





/// Boat shape struct.
typedef struct _CONF_BOAT_SHAPE   CONF_BOAT_SHAPE;
struct _CONF_BOAT_SHAPE
{
  BOAT_SHAPES  Shape;  
};



typedef struct _MapWin_COLOR MapWin_COLOR;
struct _MapWin_COLOR
{
  GUI_COLOR BackGround;
    
  GUI_COLOR Title_Background;    /// 
	 GUI_COLOR Title_Label;
 	GUI_COLOR Title_Context;
 	GUI_COLOR Title_HorLine;
	
	 GUI_COLOR Map_Grid;            /// Color of grid.
	 GUI_COLOR Map_Text;            /// Color of text.
     
  GUI_COLOR Boat_ORG;            /// Color of boat with nono setting.
  GUI_COLOR Boat_Dsp;            /// Color of boat with DSP setting.
  GUI_COLOR Boat_Bgl;            /// Color of boat with BGL setting.
  GUI_COLOR Boat_Drg;            /// Color of boat with DRG setting.
  GUI_COLOR Boat_Tip_Border;     /// Color of tip's border.
  GUI_COLOR Boat_Name;           /// Color of boat's name in tip.
  GUI_COLOR Boat_Cdnt;           /// Color of boat's coordiate in tip.
};


typedef struct _MenuWin_COLOR MenuWin_COLOR;
struct _MenuWin_COLOR 
{
  GUI_COLOR BackGround;
  
  GUI_COLOR Menu_Label;           /// Color of string "Main Menu".
  
  GUI_COLOR Bt_Sel;
  GUI_COLOR Bt_Unsel;
  GUI_COLOR Bt_Text;
  GUI_COLOR Bt_Focus;
};


typedef struct _LVWin_COLOR LVWin_COLOR;
struct _LVWin_COLOR
{
  GUI_COLOR BackGround;
    
  GUI_COLOR Win_Label;
  
  GUI_COLOR LV_Header_Bk;
  GUI_COLOR LV_Header_Text;
  GUI_COLOR LV_Header_Grid;
  
  GUI_COLOR LV_bkUnsel;
  GUI_COLOR LV_bkSel; 
  GUI_COLOR LV_bkFocus;
  GUI_COLOR LV_tx_Unsel;
  GUI_COLOR LV_tx_Sel;
  GUI_COLOR LV_tx_Focus;
  
  GUI_COLOR String;   
};

typedef struct _EditWin_COLOR EditWin_COLOR;
struct _EditWin_COLOR
{
  GUI_COLOR EditWin_BackGround;
  
  GUI_COLOR EditWin_Label;
  
  GUI_COLOR EditWin_bkEnable;
  GUI_COLOR EditWin_Text; 
  GUI_COLOR EditWin_txBk;
};


typedef struct _CnfmWin_COLOR  CnfmWin_COLOR;
struct _CnfmWin_COLOR
{
  GUI_COLOR BackGround;
    
  GUI_COLOR Title;
  
  GUI_COLOR Text;
  
  GUI_COLOR Bt_PRESSED;
  GUI_COLOR Bt_UNPRESSED;
  GUI_COLOR Bt_Text;
  GUI_COLOR Bt_Focus;
};


typedef struct _SysWin_COLOR  SysWin_COLOR;
struct _SysWin_COLOR
{
  GUI_COLOR BackGround;
  
  GUI_COLOR Label;
  
  GUI_COLOR Edit_Enable;
  GUI_COLOR Edit_Disable;
  GUI_COLOR Edit_Text;  
};



/// UI color config struct.
typedef struct _CONF_UI_COLOR CONF_UI_COLOR;
struct _CONF_UI_COLOR
{
   MapWin_COLOR   MapWinColor;      /// Map window color config.
   MenuWin_COLOR  MenuWinColor;     /// Menu window color config.
   LVWin_COLOR    Sub0WinColor;     /// Sub0 window color config.
   LVWin_COLOR    Sub1WinColor;     /// Sub1 window color config.
   LVWin_COLOR    Sub2WinColor;     /// Sub2 window color config.
   LVWin_COLOR    Sub3WinColor;     /// Sub3 window color config.
   EditWin_COLOR  EditWinCOLOR;     /// Edit window color config.
   CnfmWin_COLOR  CnfmWinColor;     /// Confirm window color config.
};


/// System setting config struct
typedef struct _CONF_SYS  CONF_SYS;
struct _CONF_SYS
{
  SKINS                Skin;        /// Skin  config.
 	CONF_SND             Snd;         /// Sound config.
	 short                Brt;  /// Light config.
  UNITS                Unit;        /// Unit  config.
  BOAT_SHAPES          Shape;       /// Shape config.
};

extern LVWin_COLOR lvWinSkins[2];
extern LVWin_COLOR * pLVSkin;

extern EditWin_COLOR EditWinSkins[2];
extern EditWin_COLOR * pEtWinSkin;


void sysInit(void);
void sysLoad(void);
void sysStore(void);

#endif

