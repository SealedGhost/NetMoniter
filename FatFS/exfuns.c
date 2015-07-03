#include "string.h"
#include "exfuns.h"
#include "uart.h"
#include "ff.h"
#include "sd.h"
#include "lpc_types.h"
#include "lcd43.h"
// #include "HZ.h"
#define FILE_MAX_TYPE_NUM		7	//最多FILE_MAX_TYPE_NUM个大类
#define FILE_MAX_SUBT_NUM		4	//最多FILE_MAX_SUBT_NUM个小类

uint8_t*const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]= //文件类型列表
{
{"BIN"},			//BIN文件
{"LRC"},			//LRC文件
{"NES"},			//NES文件
{"TXT","C","H"},	//文本文件
{"WAV","MP3","APE","FLAC"},//支持的音乐文件
{"BMP","JPG","JPEG","GIF"},//图片文件
{"AVI"},			//视频文件
};

FATFS *fs[_VOLUMES];//逻辑磁盘工作区.	 
FIL *file;	  		//文件
FIL *FILE_PIC;    //图片文件
UINT br,bw;			//读写变量
FILINFO fileinfo;	//文件信息
DIR dir;  			//目录
uint32_t SIZE_UNIGBK;
uint8_t *UNIGBK;
uint8_t *UNICODE16;
uint8_t *UNICODE24;
uint8_t *UNICODE32;
uint8_t *fatbuf;		//SD卡数据缓存区,1MB之内
uint8_t *PICTURE;    //SD卡数据缓存区

uint32_t SIZE_UNICODE16;
uint32_t SIZE_UNICODE24;

uint8_t exfuns_init(void)//为exfuns申请内存
 {
	fs[0]=(FATFS*)FATFS_BASEADDR;	//为磁盘i工作区申请内存	
  file=(FIL*)file_BASEADDR;		//单竏鯯DRAM，两个字节地址，为file申请内存
	FILE_PIC=(FIL*)file_PIC_BASEADDR;
	UNIGBK=(uint8_t*)UNIGBK_BASEADDR;
	UNICODE16=(uint8_t*)UNICODE16_BASEADDR;
	UNICODE24=(uint8_t*)UNICODE24_BASEADDR;
	UNICODE32=(uint8_t*)UNICODE32_BASEADDR;
	PICTURE=(uint8_t*)PICTURE_BASEADDR;			//为PICTURE申请内存
 	fatbuf=(uint8_t*)fatbuf_BASEADDR;				//为fatbuf申请内存	
 	return 0;
 }

void fs_test(void)
{
	FRESULT res;
	FRESULT ceshi;
	int i;
	char folder[255] = ""; 
	res = (FRESULT)SD_Init();
	if(res == FALSE){printf("sd init error.\n\r");}
	res = f_mount(fs[0],"0:", 1);
	if(res != FR_OK){printf("f_mount error = %d\n\r",res);}

//  LCD_ShowString(200,100,400,32,32,"Loading UNIGBK.BIN .....");
	ceshi=f_open (file, "UNIGBK.BIN", FA_READ);  RES_FALT(ceshi);
	SIZE_UNIGBK=(*file).fsize;
	printf("\n UNIGBK.BIN 文件所占大小：%d字节 \n",(*file).fsize);
	ceshi=f_read(file, UNIGBK, SIZE_UNIGBK, &br);	RES_FALT(ceshi);
	printf("\nbr 的值：%d\n",br);printf("\n");
	//for (i = 0; i < br; i++){printf("%c",UNIGBK[i]);}	
	f_close(file);br=0;//////////////////////////////////////////////关闭文件

  scan_files(folder);	//遍历SD卡文件

//  LCD_ShowString(200,132,400,32,32,"Loading text.txt .....");	
	ceshi=f_open (file, "text.txt", FA_READ);  RES_FALT(ceshi);
	printf("\n text.txt 文件所占大小：%d字节 \n",(*file).fsize);
	ceshi=f_read(file, fatbuf, (*file).fsize, &br);	RES_FALT(ceshi);
	printf("\nbr 的值：%d\n",br);printf("\n");
	for (i = 0; i < br; i++){printf("%c",fatbuf[i]);}	
	f_close(file);br=0;//////////////////////////////////////////////关闭文件
	
//	LCD_ShowString(200,164,400,32,32,"Loading HZ16.sif .....");
 	ceshi=f_open(file,"HZ16.sif", FA_READ);
	SIZE_UNICODE16=(*file).fsize;
	printf("\n UNICODE16所占大小：%d字节 \n",SIZE_UNICODE16);
	ceshi=f_read(file, UNICODE16,SIZE_UNICODE16, &br);//将HZ16.sif文件读到0xA0030000起始的SDRAM
	printf("\n br 的值：%d \n",br);
 // for (i = 0; i < br; i++){printf("%x",GBK16[i]);}
	f_close(file);br=0;//////////////////////////////////////////////关闭文件
	
// 	ceshi=f_open(file,"GBK24.FON", FA_READ);
// 	SIZE_GBK24=(*file).fsize;
// 	printf("\n SIZE_GBK24所占大小：%d字节 \n",SIZE_GBK24);
// 	ceshi=f_read(file, GBK24,SIZE_GBK24, &br);//将GBK16文件读到0xA0100000起始的SDRAM
// 	printf("\n br 的值：%d \n",br);
//  // for (i = 0; i < br; i++){printf("%x",GBK16[i]);}
// 	f_close(file);br=0;//////////////////////////////////////////////关闭文件
 	
// 	ceshi=f_open(file,"GBK32.FON", FA_READ);
// 	SIZE_GBK32=(*file).fsize;
// 	printf("\n SIZE_GBK32所占大小：%d字节 \n",SIZE_GBK32);
// 	ceshi=f_read(file, GBK32,SIZE_GBK32, &br);//将GBK16文件读到0xA0300000起始的SDRAM
// 	printf("\n br 的值：%d \n",br);
//  // for (i = 0; i < br; i++){printf("%x",GBK16[i]);}
// 	f_close(file);br=0;//////////////////////////////////////////////关闭文件
	

//	LCD_ShowString(200,196,400,32,32,"ALL FILE LOADING SUCCED");
}
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fileinfo;
    DIR dir;
    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fileinfo.lfname = lfn;
    fileinfo.lfsize = sizeof lfn;
#endif


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fileinfo);                   /* Read a directory item */
            if (res != FR_OK || fileinfo.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fileinfo.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
            fn = fileinfo.fname;
#endif
            if (fileinfo.fattrib & AM_DIR) {                    /* It is a directory */
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s \n\r", path, fn);
            }

        }
    }

    return res;
}
//将小写字母转为大写字母,如果是数字,则保持不变.
uint8_t char_upper(uint8_t c)
{
	if(c<'A')return c;//数字,保持不变.
	if(c>='a')return c-0x20;//变为大写.
	else return c;//大写,保持不变
}	      
//报告文件的类型
//fname:文件名
//返回值:0XFF,表示无法识别的文件类型编号.
//		 其他,高四位表示所属大类,低四位表示所属小类.
uint8_t f_typetell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr='\0';//后缀名
	uint8_t i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//偏移到了最后了.
		fname++;
	}
	if(i==250)return 0XFF;//错误的字符串.
 	for(i=0;i<5;i++)//得到后缀名
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//全部变为大写 
	for(i=0;i<FILE_MAX_TYPE_NUM;i++)	//大类对比
	{
		for(j=0;j<FILE_MAX_SUBT_NUM;j++)//子类对比
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//此组已经没有可对比的成员了.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//找到了
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//没找到		 			   
}	 

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
uint8_t exf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free)
{
	FATFS *fs1;
	uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(uint32_t)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}	
void RES_FALT(FRESULT RES)
{
	if(RES) 
	{
		switch (RES)
		{
			case FR_NO_FILE :        printf("\n can not found this file \n");break;
			case FR_NO_PATH :        printf("\n can not found this PATH \n");break;
			case FR_INVALID_NAME :printf("\n 文件名无效\n");break;
			case FR_INVALID_DRIVE   :printf("\n 驱动器号无效 \n");break;
			case FR_EXIST :          printf("\n 该文件已存在 \n");break;
			case FR_DENIED :         printf("\n 访问被拒绝 \n");break;
			case FR_NOT_READY :      printf("\n 由于。。磁盘驱动器无法工作 \n");break;
			case FR_WRITE_PROTECTED: printf("\n SD卡被写保护 \n");break;
			case FR_DISK_ERR :       printf("\n 底层IO借口错误 \n");break;
			case FR_INT_ERR:         printf("\n FAT结构错误 \n");break;
			case FR_NOT_ENABLED :    printf("\n 逻辑驱动器没有工作区 \n");break;
			case FR_NO_FILESYSTEM :  printf("\n 磁盘没有有效的FAT卷 \n");break;
		}
  }	
}









