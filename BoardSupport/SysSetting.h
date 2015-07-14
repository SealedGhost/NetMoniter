#ifndef _SYSSETTING_H
#define _SYSSETTING_H

typedef struct
{
	short LCD_Mode;          ///夜间模式？
	short LCD_BkLightLevel;  ///屏幕亮度
}LCD_SETTING;

typedef struct
{
	short SND_Mode;      /// 声音类型
	short SND_Volume;     /// 音量
}SND_SETTING;


typedef  struct 
{
	short TIME_Mode;
	short TIME_Zone;	
	long TIME_TickCnt;
} TIME_SETTING;



typedef enum UNITS{UNITS_Km=1,UNITS_Nm} UNITS;

typedef struct
{
	LCD_SETTING   LCD_Setting;
	SND_SETTING   SND_AlarmSetting;
	SND_SETTING   SND_KeySetting;
	UNITS         SysUnit ;
	unsigned int  SoftVersion;
}SYS_SETTING;



///
typedef struct{
  uint8_t isEnable;
}DSP_SETTING;

typedef struct{
   uint8_t isEnable;
   uint8_t isSndEnable;
   uint32_t dist;         /// Unit (m)
}BGL_SETTING;

typedef struct{
   uint8_t isEnable;
   uint8_t isSndEnable;
   uint32_t dist;   
}DRG_SETTING;  /// Dragging anchor setting

typedef struct
{
   DSP_SETTING DSP_Setting;
   BGL_SETTING BGL_Setting;
   DRG_SETTING DRG_Setting;
}MNT_SETTING;

typedef struct
{
   long mmsi;
   MNT_SETTING MNTSetting;
   uint8_t MNTState;
}MNT_BOAT;

#endif
