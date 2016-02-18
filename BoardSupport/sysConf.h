#ifndef  _SYS_CONF_H
#define _SYS_CONF_H




/**
 *   CONF_SYS Sys_conf;
 *      
 *   Sys_conf.Skin.MapWinColor.Map_Grid  = GUI_RED;
 *
 */
#include "GUI.h"
#include "uart.h"
#include "lpc177x_8x_eeprom.h"

#define OFFSETOF(x) ((uint8_t*)x-(uint8_t*)MNT_Berthes)


#define DEFAULT_SKIN  SKIN_Day
#define DEFAULT_BRT   4
#define DEFAULT_VOL   6
#define DEFAULT_UNIT  UNIT_nm
#define DEFAULT_SHAPE SHAPE_Fish


typedef enum _SKINS SKINS;
enum _SKINS {SKIN_Day=0, SKIN_Night};

typedef enum _UNITS   UNITS;
enum _UNITS{UNIT_nm=0, UNIT_km};

typedef enum _BOAT_SHAPES   BOAT_SHAPES;
enum _BOAT_SHAPES{SHAPE_Boat=0, SHAPE_Fish};




/// Light config struct.
typedef struct _CONF_LIGHT CONF_LIGHT;
struct _CONF_LIGHT
{
   unsigned char Brightness;
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
   GUI_COLOR bkColor;
   
   GUI_COLOR ttl_bkTop;
   GUI_COLOR ttl_bkBottom;
   GUI_COLOR ttl_Label;
   GUI_COLOR ttl_Context;
   
   GUI_COLOR map_Grid;
   GUI_COLOR map_tip_Text;
   GUI_COLOR map_tip_Bat;
   
   GUI_COLOR boat_Mom;
   GUI_COLOR boat_Org;
   GUI_COLOR boat_Dsp;
   GUI_COLOR boat_Bgl;
   GUI_COLOR boat_Drg;
   
   GUI_COLOR sclColor;
	
	  GUI_COLOR net_lab;
};




typedef struct _MenuWin_COLOR MenuWin_COLOR;


struct _MenuWin_COLOR
{
   GUI_COLOR bkColor;
   GUI_COLOR tbColor;
   
   GUI_COLOR ttl_bkTop;
   GUI_COLOR ttl_bkBottom;
   GUI_COLOR ttl_Text;
   
   GUI_COLOR btBkColor; 
   GUI_COLOR btBkSel;   
   GUI_COLOR btBkFocus;
   GUI_COLOR btTxColor;
   GUI_COLOR btTxFocus;
};


typedef struct _LVWin_COLOR LVWin_COLOR;

struct _LVWin_COLOR 
{
   GUI_COLOR bkColor; 
   
   GUI_COLOR ttl_Text;
   
   GUI_COLOR Header_Bk;
   GUI_COLOR Header_Text;
   
   GUI_COLOR itm_bkUnsel;
   GUI_COLOR itm_bkSel;
   GUI_COLOR itm_bkFocus;
   
   GUI_COLOR itm_txUnsel;
   GUI_COLOR itm_txSel;
   GUI_COLOR itm_txFocus;
   
   GUI_COLOR inf_bkColor;
   GUI_COLOR inf_Label;
   GUI_COLOR inf_txColor;
};


typedef struct _MntSetWin_COLOR MntSetWin_COLOR;
struct _MntSetWin_COLOR
{
  GUI_COLOR bkColor;
  
  GUI_COLOR ttl_Label;
	
	 GUI_COLOR bt_bkColor;
  GUI_COLOR bt_bkFocus;
  GUI_COLOR bt_txColor;
  GUI_COLOR bt_txFocus;
  
	 GUI_COLOR txColor;
};


typedef struct _MntSetting_COLOR MntSetting_COLOR;
struct _MntSetting_COLOR
{
   GUI_COLOR bkColor;
   GUI_COLOR txColor;
   
   GUI_COLOR sldBk;
   GUI_COLOR sldSlot;
   GUI_COLOR sldSlider;
   GUI_COLOR sldFocusSlider;
};

typedef struct _CnfmWin_COLOR  CnfmWin_COLOR;
//struct _CnfmWin_COLOR
//{
//  GUI_COLOR BackGround;
//    
//  GUI_COLOR Title;
//  
//  GUI_COLOR Text;
//  
//  GUI_COLOR Bt_PRESSED;
//  GUI_COLOR Bt_UNPRESSED;
//  GUI_COLOR Bt_Text;
//  GUI_COLOR Bt_Focus;
//};

struct _CnfmWin_COLOR
{
   GUI_COLOR bkTop;
   GUI_COLOR bkBottom;
   
   GUI_COLOR text;
   
   GUI_COLOR bt_bkColor;
   GUI_COLOR bt_bkFocus;
   GUI_COLOR bt_txColor;
   GUI_COLOR bt_txFocus;
};


typedef struct _SysWin_COLOR  SysWin_COLOR;
struct _SysWin_COLOR
{
   GUI_COLOR bkColor;
	 GUI_COLOR ClientbkColor;
   GUI_COLOR txColor;
   
   
   GUI_COLOR sldBk;
   GUI_COLOR sldSlot;
   GUI_COLOR sldSlider;
   GUI_COLOR sldFocusSlider;
};


///// UI color config struct.
//typedef struct _CONF_UI_COLOR CONF_UI_COLOR;
//struct _CONF_UI_COLOR
//{
//   MapWin_COLOR     MapWinColor;      /// Map window color config.
//   MenuWin_COLOR    MenuWinColor;     /// Menu window color config.
//   LVWin_COLOR      Sub0WinColor;     /// Sub0 window color config.
//   LVWin_COLOR      Sub1WinColor;     /// Sub1 window color config.
//   LVWin_COLOR      Sub2WinColor;     /// Sub2 window color config.
//   LVWin_COLOR      Sub3WinColor;     /// Sub3 window color config.
//   MntSetWin_COLOR  EditWinCOLOR;     /// Edit window color config.
//   CnfmWin_COLOR    CnfmWinColor;     /// Confirm window color config.
//};


/// System setting config struct
typedef struct _CONF_SYS  CONF_SYS;
struct _CONF_SYS
{
  SKINS                Skin;        /// Skin  config.
 	char                 Vol;         /// Sound config.
	 char                 Brt;          /// Light config.
  UNITS                Unit;        /// Unit  config.
  BOAT_SHAPES          Shape;       /// Shape config.
};


extern CONF_SYS SysConf;


extern unsigned long SYS_Date;
extern unsigned long SYS_Time;

void sysInit(void);
Bool sysLoad(void);
void sysStore(void);
void sysRevive(void);

#endif

