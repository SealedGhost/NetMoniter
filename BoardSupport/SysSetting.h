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

#endif
