/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "lpc177x_8x_mci.h"
#include "lpc177x_8x_gpdma.h"
#include "sd.h"

/* Definitions of physical drive number for each media */
//#define ATA		0
//#define MMC		1
//#define USB		2
#define SD_CARD 0

extern CARDCONFIG CardConfig;

char status;

// #if MCI_DMA_ENABLED
// void DMA_IRQHandler (void)
// {
//    MCI_DMA_IRQHandler();
// }
// #endif

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD_CARD:
#if MCI_DMA_ENABLED
		GPDMA_Init();
#endif
		if(SD_Init() == FALSE)
			return STA_NOINIT;
		else
			return RES_OK;
//	case ATA :
//		result = ATA_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case MMC :
//		result = MMC_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case USB :
//		result = USB_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD_CARD:
		if(SD_GetStatus() == FALSE)
			return STA_NOINIT;
		else
			return RES_OK;
//	case ATA :
//		result = ATA_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case MMC :
//		result = MMC_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case USB :
//		result = USB_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {	
	case SD_CARD:
		if (MCI_ReadBlock (buff, sector, count) == MCI_FUNC_OK)
		{
			while(MCI_GetDataXferEndState() != 0);

			if(count > 1)
			{
				if(MCI_Cmd_StopTransmission() != MCI_FUNC_OK)
					return RES_ERROR;
			}
        if(MCI_GetXferErrState())
          return RES_ERROR;
        
			return RES_OK;
		} 
		else
			return RES_ERROR;		

//	case ATA :
//		// translate the arguments here
//
//		result = ATA_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case MMC :
//		// translate the arguments here
//
//		result = MMC_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case USB :
//		// translate the arguments here
//
//		result = USB_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD:
		if ( MCI_WriteBlock((uint8_t*)buff, sector, count) == MCI_FUNC_OK)
		{
			while(MCI_GetDataXferEndState() != 0);

			if(count > 1)
			{
				if(MCI_Cmd_StopTransmission()  != MCI_FUNC_OK)
					return RES_ERROR;
			}
   		if(MCI_GetXferErrState())
  			return RES_ERROR;
        
			return RES_OK;
		}
		else
			return RES_ERROR;	

//	case ATA :
//		// translate the arguments here
//
//		result = ATA_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case MMC :
//		// translate the arguments here
//
//		result = MMC_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case USB :
//		// translate the arguments here
//
//		result = USB_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;
	int cardStatus;
	int count = 0;
	BYTE *ptr = buff;

	switch (pdrv) {
	case SD_CARD:
		switch (cmd)
		{
			case CTRL_SYNC:
    		while(count<100)
				{	
					if((MCI_GetCardStatus(&cardStatus) == MCI_FUNC_OK) && (cardStatus & CARD_STATUS_READY_FOR_DATA ))
					{
						return RES_OK;
					}
				 	count++;
				}
				return RES_NOTRDY;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = CardConfig.SectorSize;
				return RES_OK;

			case GET_SECTOR_COUNT:
				*(WORD*)buff = CardConfig.SectorCount;
				return RES_OK;

			case GET_BLOCK_SIZE:
				*(DWORD*)buff = CardConfig.BlockSize;
				return RES_OK;

			case MMC_GET_TYPE :	
				*ptr = CardConfig.CardType;
				res = RES_OK;
				break;

			case MMC_GET_CSD :	
				for (count=0;count<16;count++)
					*(ptr+count) = CardConfig.CSD[count]; 
				res = RES_OK;
				break;

			case MMC_GET_CID :		
        {
        	uint8_t* cid = (uint8_t*) &CardConfig.CardID;
    			for (count=0;count<sizeof(st_Mci_CardId);count++)
    			*(ptr+count) = cid[count];
        }
				res = RES_OK;
				break;
		}
		return res;
//	case ATA :
//		// pre-process here
//
//		result = ATA_disk_ioctl(cmd, buff);
//
//		// post-process here
//
//		return res;
//
//	case MMC :
//		// pre-process here
//
//		result = MMC_disk_ioctl(cmd, buff);
//
//		// post-process here
//
//		return res;
//
//	case USB :
//		// pre-process here
//
//		result = USB_disk_ioctl(cmd, buff);
//
//		// post-process here
//
//		return res;
	}
	return RES_PARERR;
}
#endif

DWORD get_fattime (void)
{
	return	RES_OK; 
}
