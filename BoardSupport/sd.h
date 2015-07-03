#include <LPC177x_8x.H>
#include "lpc_types.h"
#include "lpc177x_8x_mci.h"
typedef struct tagCARDCONFIG
{
    uint32_t 		SectorSize;    /* size (in byte) of each sector, fixed to 512bytes */
    uint32_t 		SectorCount;     /* total sector number */  
    uint32_t 		BlockSize;     /* erase block size in unit of sector */
	uint32_t 		CardAddress;	/* Card Address */
	uint32_t 		OCR;			/* OCR */
	en_Mci_CardType CardType;		/* Card Type */
	st_Mci_CardId 	CardID;			/* CID */
	uint8_t  		CSD[16];		/* CSD */
} CARDCONFIG;

	Bool SD_Init (void);
	Bool SD_GetStatus(void);
	void swap_buff(uint8_t* buff, uint32_t count);
	uint32_t unstuff_bits(uint8_t* resp,uint32_t start, uint32_t size);

/************************************* End *************************************/
