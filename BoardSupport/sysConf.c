#include "SystemConfig.h"


///                                        
static  CONF_UI_COLOR UI_Skins[2] = {GUI_BLACK};



CONF_SYS SysConf;


void sysLoad()
{
   printf("\n\r    Loading...  \n\r");
}

void sysStore()
{
   printf("\n\r    Storing... \n\r");
}


void sysInit()
{   
   sysLoad();
   
   SysConf.Skin  = SKIN_Night;
   SysConf.Brt  = 1;
   SysConf.Snd.Vol  = 2;
   SysConf.Snd.ArmSnd  = 3;
   SysConf.Snd.KeySnd  = 4;
   SysConf.Unit  = UNIT_nm;
   SysConf.Shape  = SHAPE_Fish;
}