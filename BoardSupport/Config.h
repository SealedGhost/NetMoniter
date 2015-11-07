#ifndef _CONFIG_H
#define _CONFIG_H

#define MOTHERSHIP_LA    1927265
#define MOTHERSHIP_LG    7128663



/********************************************************************
 *
 *               UI config
 */
#define Win_Main_WIDTH 800
#define Win_Main_HEIGHT 480

#define MenuLabel_X 0
#define MenuLabel_WIDTH  120
#define MenuLabel_Y 40
#define MenuLabel_HEIGHT 200

#define MenuButton_WIDTH  120
#define MenuButton_HEIGHT 50

/// UI --- Sub window
#define SubWin_X        (MenuLabel_WIDTH)
#define SubWin_WIDTH    (Win_Main_WIDTH-MenuLabel_WIDTH)
#define SubWin_Y        0
#define SubWin_HEIGHT   Win_Main_HEIGHT

/// UI --- ET window
#define ETWin_X   (MenuLabel_WIDTH+LV_MoniteSet_WIDTH+10)
#define ETWin_Y   SubWin_Y
#define ETWin_WIDHT  (SubWin_WIDTH-LV_MoniteSet_WIDTH-5)
#define ETWin_HEIGHT  SubWin_HEIGHT

/// UI --- Monite list lV
#define LV_MoniteList_X         5
#define LV_MoniteList_WIDTH     330
#define LV_MoniteList_Y         40
#define LV_MoniteList_HEIGHT    400

#define LV_MoniteList_Col_0_WIDTH    90
#define LV_MoniteList_Col_1_WIDTH    170
#define LV_MoniteList_Col_2_WIDTH    70
#define LV_MoniteList_Row_HEIGHT     40
#define LV_MoniteList_Header_HEIGHT  40



/// UI --- Monite set
#define LV_MoniteSet_X         5
#define LV_MoniteSet_WIDTH     440
#define LV_MoniteSet_Y         40
#define LV_MoniteSet_HEIGHT    400

#define LV_MoniteSet_Col_0_WIDTH    250
#define LV_MoniteSet_Col_1_WIDTH    120
#define LV_MoniteSet_Col_2_WIDTH    70
#define LV_MoniteSet_Row_HEIGHT     40
#define LV_MoniteSet_Header_HEIGHT  40


/// UI --- All list
#define LV_AllList_X            5
#define LV_AllList_WIDTH        320
#define LV_AllList_Y            40
#define LV_AllList_HEIGHT       400  

#define LV_AllList_Col_0_WIDTH    80
//#define LV_AllList_Col_1_WIDTH    80
#define LV_AllList_Col_2_WIDTH    160
#define LV_AllList_Col_3_WIDTH    80
#define LV_AllList_Row_HEIGHT     40
#define LV_AllList_Header_HEIGHT  40
#define LV_AllList_Col_DIST        0
#define LV_AllList_Col_MMSI        1
#define LV_AllList_Col_STT         2


/// UI  --- Sys set
#define Win_SysSet_X             5
#define Win_SysSet_WIDHT         (Win_Main_WIDTH-MenuLabel_RIGHT)
#define Win_SysSet_Y             0
#define Win_SysSet_BOTTOM        (Win_Main_HEIGT)
#define Win_SysSet_Text_HEIGHT   30
#define Win_SysSet_Text_WIDTH    120
#define Win_SysSet_txOrg         55
#define Win_SysSet_txGrap        15

/********************************************************************
 *
 *               boat drawing config
 */
#define DSP_BOAT_COLOR   GUI_BLACK

#define BGL_BOAT_COLOR   GUI_RED
#define BGL_CIRCLE_R     15

#define DRG_BOAT_COLOR   GUI_GREEN
#define DRG_CIRCLE_R     20
#define DRG_PENSIZE      4

#define MM_BOAT_COLOR    GUI_BLUE
#define MM_BOAT_PENSIZE   2





/********************************************************************
 *
 *               timer counter config
 */
#define TIMESTAMP           60

#define MAP_REFRESH_SLOT    500
#define AUTO_ADAPTER_CNT    20  



/********************************************************************
 *
 *              array size config
 */
#define MSG_QUEUE_TABNUM    20
#define BOAT_NUM_MAX        500
#define MNT_NUM_MAX         20
#define INVD_NUM_MAX        10


/********************************************************************
 *
 *              EEPROM address config
 */
#define SYSCONF_ADDR     0                        

#define MNT_PAGE_ID_HEADER  (uint8_t)1
#define MNT_PAGE_ID         (uint8_t)2       


/********************************************************************
 *
 *              code switch config
 */
#define CODE_CHECK 

/********************************************************************
 *
 *               printf config.
 */
#define __INFO_ENABLE 

#ifndef __INFO_ENABLE

#define INFO(format,...) 

#else

#define INFO(format,...)  printf("FILE:%s,LINE:%d,"format"\r\n",__FILE__,__LINE__,##__VA_ARGS__)

#endif


/********************************************************************
 *
 *               cursor config.
 */
#define CURSOR_SPEED   5


#endif
