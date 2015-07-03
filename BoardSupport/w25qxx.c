#include "w25qxx.h" 
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_pinsel.h"  
#include "uart.h"	
#include "sdram.h"
#include "HZ.h"
#include "lcd43.h"
#include "exfuns.h"

//extern uint32_t SIZE_UNICODE16;
//extern uint32_t SIZE_UNICODE24;

void SPI_FLASH_Init(void)     //SPI_FLASH初始化函数
{
	SSP_CFG_Type SSP_ConfigStruct;
  PINSEL_ConfigPin(0, 7, 2);//p0.7：SSP0_SCK
  PINSEL_ConfigPin(0, 6, 0);//p0.6: CS //SSP0_SSEL
  PINSEL_ConfigPin(0, 8, 2);//p0.8: SSP0_MISO
  PINSEL_ConfigPin(0, 9, 2);//p0.9: SSP0_MOSI
  GPIO_SetDir(0, (1<<6), 1); 	/* 设置触摸片选管脚p0.6为输出 */
  GPIO_SetValue(0, (1<<6));

  SSP_ConfigStruct.ClockRate =2000000; 	/* 配置SPI参数最高20M */
  SSP_ConfigStruct.CPHA = SSP_CPHA_FIRST;
  SSP_ConfigStruct.CPOL = SSP_CPOL_HI; 
	SSP_ConfigStruct.Databit = SSP_DATABIT_8;
	SSP_ConfigStruct.Mode = SSP_MASTER_MODE;
	SSP_ConfigStruct.FrameFormat = SSP_FRAME_SPI;
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);//初始化SSP1

	LPC_SSP1->CR1 |= SSP_CR1_SSP_EN;//SSP_Cmd(LPC_SSP1, ENABLE);	/* 使能SPI 外设 1 */
}
/* 函数名(Function):	SPI_FLASH_SendByte
 * 参数(Param):				byte 要发送的1字节数据 							
 * 返回值(Return):		tmp  接收到的1字节数据
 * 描述(Description):	SPI字节发送接收函数*/
uint8_t SPI_FLASH_SendByte(uint8_t byte)//
{
	while ( (LPC_SSP1->SR & (SSP_SR_TNF|SSP_SR_BSY)) != SSP_SR_TNF );  /* 等待忙标志位为零和发送缓冲为未满 */
	LPC_SSP1->DR = byte;  /* 发送1字节数据到SPI1 */
 	while ( (LPC_SSP1->SR & (SSP_SR_BSY|SSP_SR_RNE)) != SSP_SR_RNE );  /* 忙标志位为零和接收缓冲器非空 */
	return (LPC_SSP1->DR);
}

uint16_t W25QXX_TYPE=W25Q64;	//默认是W25Q64

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q64
//容量为8M字节,共有128个Block,4096个Sector 
void W25QXX_Init(void)
{ 
	SPI_FLASH_Init();		   			//初始化SPI
 	SPI_FLASH_CS_HIGH();	
	W25QXX_TYPE=W25QXX_ReadID();	//读取FLASH ID.
}  
//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	
  SPI_FLASH_CS_LOW();	
	SPI_FLASH_SendByte(0x90);//发送读取ID命令	  
	SPI_FLASH_SendByte(0x00); 	    
	SPI_FLASH_SendByte(0x00); 	    
	SPI_FLASH_SendByte(0x00); 	 			   
	Temp|=SPI_FLASH_SendByte(0xFF)<<8;
	Temp|=SPI_FLASH_SendByte(0xFF);	 
  printf("Flash ID :%x\r\n",Temp);	
	SPI_FLASH_CS_HIGH();
	return Temp;
} 
//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t W25QXX_ReadSR(void)   
{  
	uint8_t byte=0;   
	SPI_FLASH_CS_HIGH();    
	SPI_FLASH_CS_LOW();                            //使能器件   
	SPI_FLASH_SendByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI_FLASH_SendByte(0Xff);             //读取一个字节  
// 	SPI_FLASH_CS_HIGH();                            //取消片选     
	return byte;   
} 
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25QXX_Write_SR(uint8_t sr)   
{   
  SPI_FLASH_CS_HIGH(); 	
	SPI_FLASH_CS_LOW();                            //使能器件   
	SPI_FLASH_SendByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI_FLASH_SendByte(sr);               //写入一个字节  
// 	SPI_FLASH_CS_HIGH();                            //取消片选     	      
}   
//W25QXX写使能	
//将WEL置位   
void W25QXX_Write_Enable(void)   
{
	SPI_FLASH_CS_HIGH(); 
	SPI_FLASH_CS_LOW();                            //使能器件   
  SPI_FLASH_SendByte(W25X_WriteEnable);      //发送写使能  
 	SPI_FLASH_CS_HIGH();                            //取消片选     	      
} 
//W25QXX写禁止	
//将WEL清零  
void W25QXX_Write_Disable(void)   
{  
	SPI_FLASH_CS_HIGH(); 
	SPI_FLASH_CS_LOW();                            //使能器件   
  SPI_FLASH_SendByte(W25X_WriteDisable);     //发送写禁止指令    
// 	SPI_FLASH_CS_HIGH();                            //取消片选     	      
} 		
  		    
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   	
    SPI_FLASH_CS_HIGH(); 	
	  SPI_FLASH_CS_LOW();                            //使能器件   
    SPI_FLASH_SendByte(W25X_ReadData);         //发送读取命令   
    SPI_FLASH_SendByte((uint8_t)((ReadAddr)>>16));  //发送24bit地址    
    SPI_FLASH_SendByte((uint8_t)((ReadAddr)>>8));   
    SPI_FLASH_SendByte((uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI_FLASH_SendByte(0XFF);   //循环读数  
    }
 	SPI_FLASH_CS_HIGH();  				    	      
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    W25QXX_Write_Enable();                  //SET WEL 
	SPI_FLASH_CS_LOW();                            //使能器件   
    SPI_FLASH_SendByte(W25X_PageProgram);      //发送写页命令   
    SPI_FLASH_SendByte((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
    SPI_FLASH_SendByte((uint8_t)((WriteAddr)>>8));   
    SPI_FLASH_SendByte((uint8_t)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI_FLASH_SendByte(pBuffer[i]);//循环写数  
// 	SPI_FLASH_CS_HIGH();                            //取消片选 
	W25QXX_Wait_Busy();					   //等待写入结束
	SPI_FLASH_CS_HIGH(); 
} 
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		
		W25QXX_Write_Enable();
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}
		else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
		SPI_FLASH_CS_HIGH(); 
	};	 
}
	  
//等待时间超长...
void W25QXX_Erase_Chip(void) //擦除整个芯片	  
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_Wait_Busy();   
  	SPI_FLASH_CS_LOW();                            //使能器件   
    SPI_FLASH_SendByte(W25X_ChipErase);        //发送片擦除命令  
	SPI_FLASH_CS_HIGH();                            //取消片选     	      
	W25QXX_Wait_Busy();   				   //等待芯片擦除结束
}   

//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr) //擦除一个扇区  
{  
	//监视falsh擦除情况,测试用  
	SPI_FLASH_CS_HIGH(); 	
 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	SPI_FLASH_CS_LOW();                            //使能器件   
    SPI_FLASH_SendByte(W25X_SectorErase);      //发送扇区擦除指令 
    SPI_FLASH_SendByte((uint8_t)((Dst_Addr)>>16));  //发送24bit地址    
    SPI_FLASH_SendByte((uint8_t)((Dst_Addr)>>8));   
    SPI_FLASH_SendByte((uint8_t)Dst_Addr);  
// 	 SPI_FLASH_CS_HIGH();                            //取消片选     	      
    W25QXX_Wait_Busy();   				   //等待擦除完成
	SPI_FLASH_CS_HIGH(); 
}  
//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	SPI_FLASH_CS_HIGH(); 
	while((W25QXX_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//进入掉电模式
void W25QXX_PowerDown(void)   
{ 
	  SPI_FLASH_CS_HIGH(); 
  	SPI_FLASH_CS_LOW();                            //使能器件   
    SPI_FLASH_SendByte(W25X_PowerDown);        //发送掉电命令  
// 	SPI_FLASH_CS_HIGH();                            //取消片选     	      
    //delay_us(3);                               //等待TPD  
}   
//唤醒
void W25QXX_WAKEUP(void)   
{  	SPI_FLASH_CS_HIGH();
  	SPI_FLASH_CS_LOW();                            //使能器件   
    SPI_FLASH_SendByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
// 	SPI_FLASH_CS_HIGH();                            //取消片选     	      
   // delay_us(3);                               //等待TRES1
}   

/*void updata_font(void)
{
	   uint16_t l=0;	uint16_t m=0;		 

		 l=308;//l=SIZE_UNICODE16/4096+1;
	   printf("\n\rupdata_font l:%d\n\r",l);
	   LCD_ShowString(30,150,200,16,16,"LOAD FONT TO Flash:");		
 		 LCD_FillRectangle(LCD_PANEL_UPPER,166,380,150,166, skyblue);
 	   for(m=0;m<l;m++)
	   {
		  W25QXX_Write(UNICODE16+m*4096,4096*m,4096);		//从0开始写入4096个数据
      LCD_FillRectangle(LCD_PANEL_UPPER,166,166+(310/l*m),150,166, NavyBlue);			 
		 }  
}*/

void load_font(void)
{
	   uint16_t l=0;	uint16_t m=0; 
	   l=308;//l=SIZE_UNICODE16/4096+1;
	   LCD_FillRectangle(LCD_PANEL_UPPER,166,380,150,166, skyblue);
     printf("\n\rload_font l:%d\n\r",l);
	   //LCD_ShowString(30,150,200,16,16,"LOAD FONT FORM FLASH:");
		 for(m=0;m<l;m++)
	   {
 			 W25QXX_Read(UNICODE16+m*4096,6*1024*1024+4096*m,4096);
       LCD_FillRectangle(LCD_PANEL_UPPER,166,166+(310/l*m),150,166, NavyBlue);			 
		 }	 
	   //LCD_ShowString(30,166,200,16,16,"16*16FONT LOAD SUCCEED");	 
}
























