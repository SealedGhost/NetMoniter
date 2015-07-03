#ifndef __EXFUNS_H
#define __EXFUNS_H 					   
#include <LPC177x_8x.H>
#include "ff.h"
#include "sdram.h"
extern FATFS *fs[_VOLUMES];  
extern FIL *file;	 	
extern FIL *FILE_PIC;     //图片文件
extern UINT br,bw;
extern FILINFO fileinfo;
extern DIR dir;
extern uint32_t SIZE_UNIGBK;
extern uint8_t *UNIGBK;
extern uint8_t *UNICODE16;
extern uint8_t *UNICODE24;
extern uint8_t *UNICODE32;
extern uint8_t *fatbuf;//SD卡数据缓存区
extern uint8_t *PICTURE;//SD卡数据缓存区0xA0700000~0xA1000000

extern uint32_t SIZE_UNICODE16;
extern uint32_t SIZE_UNICODE24;


#define UNIGBK_BASEADDR       ((uint32_t)SDRAM_BASE_ADDR ) //GBK与UNICODE转码表
#define UNICODE16_BASEADDR	  ((uint32_t)SDRAM_BASE_ADDR + 0x130000 )//UNICODE16字库存放始地址
#define UNICODE24_BASEADDR	  ((uint32_t)SDRAM_BASE_ADDR + 0x200000)//GBK24字库存放始地址
#define UNICODE32_BASEADDR	  ((uint32_t)SDRAM_BASE_ADDR + 0x400000)//GBK32字库存放始地址
#define FATFS_BASEADDR	      ((uint32_t)SDRAM_BASE_ADDR + 0x700000)
#define file_BASEADDR	        ((uint32_t)SDRAM_BASE_ADDR + 0x700000+0x10000)
#define file_PIC_BASEADDR     ((uint32_t)SDRAM_BASE_ADDR + 0x700000+0x20000)
#define fatbuf_BASEADDR	      ((uint32_t)SDRAM_BASE_ADDR + 0x700000+0x30000)
#define PICTURE_BASEADDR      ((uint32_t)SDRAM_BASE_ADDR + 0x800000)

//f_typetell返回的类型定义
//根据表FILE_TYPE_TBL获得.在exfuns.c里面定义
#define T_BIN		0X00	//bin文件
#define T_LRC		0X10	//lrc文件
#define T_NES		0X20	//nes文件

#define T_TEXT		0X30	//.txt文件
#define T_C			0X31	//.c文件
#define T_H			0X32    //.h文件

#define T_WAV		0X40	//WAV文件
#define T_MP3		0X41	//MP3文件 
#define T_APE		0X42	//APE文件
#define T_FLAC		0X43	//FLAC文件

#define T_BMP		0X50	//bmp文件
#define T_JPG		0X51	//jpg文件
#define T_JPEG		0X52	//jpeg文件		 
#define T_GIF		0X53	//gif文件   

#define T_AVI		0X60	//avi文件  

 
uint8_t exfuns_init(void);							//申请内存
uint8_t f_typetell(uint8_t *fname);						//识别文件类型
uint8_t exf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free);	//得到磁盘总容量和剩余容量
uint32_t exf_fdsize(uint8_t *fdname);						//得到文件夹大小			
void fs_test(void);
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
);
void RES_FALT(FRESULT RES);
#endif


