#include "lpc177x_8x_mci.h"
#include "lpc177x_8x_gpdma.h"
#include "lpc177x_8x_pinsel.h"
#include "sd.h"

CARDCONFIG CardConfig;

Bool SD_Init (void)//SD接口（mci）初始化函
{
	uint32_t c_size, c_size_mult, read_bl_len;
    uint8_t csd_struct = 0;
	
	if(MCI_Init(LOW_LVL) != MCI_FUNC_OK)
	{
		return FALSE;
	}
	do
	{
  	CardConfig.CardType= MCI_GetCardType();		/* Get Card Type */
		if(CardConfig.CardType == MCI_CARD_UNKNOWN)
		{
			break;
		}
		/*if(MCI_ReadOCR(&CardConfig.OCR) != MCI_FUNC_OK)	//Read OCR
		{
			break;
		} */
		if (MCI_GetCID(&CardConfig.CardID) != MCI_FUNC_OK)	    /* Read CID */
		{
			break;
		}
		if(MCI_SetCardAddress() != MCI_FUNC_OK)		/* Set Address */
		{
			break;
		}
		CardConfig.CardAddress = MCI_GetCardAddress();
		if(MCI_GetCSD((uint32_t*)CardConfig.CSD) != MCI_FUNC_OK)		/* Read CSD */
		{
			break;
		}
        swap_buff(&CardConfig.CSD[0], sizeof(uint32_t));
        swap_buff(&CardConfig.CSD[4], sizeof(uint32_t));
        swap_buff(&CardConfig.CSD[8], sizeof(uint32_t));
        swap_buff(&CardConfig.CSD[12], sizeof(uint32_t));
		    swap_buff(CardConfig.CSD, 16);	
    	CardConfig.SectorSize = 512;		/* sector size */
       csd_struct = CardConfig.CSD[15] >> 6;
	    if (csd_struct == 1)    /* CSD V2.0 */		 /* Block count */
	    {

	       c_size =  unstuff_bits(CardConfig.CSD, 48,22);	        /* Read C_SIZE */
	       CardConfig.SectorCount  = (c_size + 1) * 1024;	        /* Calculate block count */

	    } else   /* CSD V1.0 (for Standard Capacity) */
	    {
	        c_size = unstuff_bits(CardConfig.CSD, 62,12);	        /* C_SIZE */
	        c_size_mult = unstuff_bits(CardConfig.CSD, 47,3);	        /* C_SIZE_MUTE */
	        read_bl_len = unstuff_bits(CardConfig.CSD, 80,4);	        /* READ_BL_LEN */	       
	        CardConfig.SectorCount = (((c_size+1)* (0x01 << (c_size_mult+2))) * (0x01<<read_bl_len))/512; /* sector count = BLOCKNR*BLOCK_LEN/512, we manually set SECTOR_SIZE to 512*/
	    }
        switch (CardConfig.CardType)        /* Get erase block size in unit of sector */
        {
            case MCI_MMC_CARD:
                 CardConfig.BlockSize = unstuff_bits(CardConfig.CSD, 42,5) + 1;
				         CardConfig.BlockSize <<=  unstuff_bits(CardConfig.CSD, 22,4);
                 CardConfig.BlockSize /= 512;
                break;
            case MCI_SDHC_SDXC_CARD:
            case MCI_SDSC_V2_CARD:
                CardConfig.BlockSize = 1;
                break;
            case MCI_SDSC_V1_CARD:
                if(unstuff_bits(CardConfig.CSD, 46,1))
                {
                     CardConfig.BlockSize = 1;
                }
                else
                {
                    CardConfig.BlockSize = unstuff_bits(CardConfig.CSD, 39,7) + 1;
                    CardConfig.BlockSize <<=  unstuff_bits(CardConfig.CSD, 22,4);
                    CardConfig.BlockSize /= 512;
                }
                break;
            default:
                break;                
        }
		if(MCI_Cmd_SelectCard() != MCI_FUNC_OK)		/* Select Card */
		{
			break;
		}

        MCI_Set_MCIClock( MCI_NORMAL_RATE );
		if ((CardConfig.CardType== MCI_SDSC_V1_CARD) ||
			(CardConfig.CardType== MCI_SDSC_V2_CARD) ||
			(CardConfig.CardType== MCI_SDHC_SDXC_CARD)) 
		{		
				if (MCI_SetBusWidth( SD_4_BIT ) != MCI_FUNC_OK )
				{
					break;
				}
		}
        else
        {
                if (MCI_SetBusWidth( SD_1_BIT ) != MCI_FUNC_OK )
				{
					break;
				}
        }    
        if (CardConfig.CardType == MCI_MMC_CARD ||     /* For SDHC or SDXC, block length is fixed to 512 bytes, for others,the block length is set to 512 manually. */
            CardConfig.CardType == MCI_SDSC_V1_CARD ||
            CardConfig.CardType == MCI_SDSC_V2_CARD )
        {if(MCI_SetBlockLen(BLOCK_LENGTH) != MCI_FUNC_OK){break;}}            
        return TRUE;
	}
	while (FALSE);
    return FALSE;
}

Bool SD_GetStatus(void)
{
	int	respValue;
	 
	if (MCI_GetCardStatus(&respValue) != MCI_FUNC_OK)
	{
		return FALSE;
	}

	return TRUE;
}

void swap_buff(uint8_t* buff, uint32_t count)
{
    uint8_t tmp;
    uint32_t i;

    for(i = 0; i < count/2; i++)
    {
        tmp = buff[i];
        buff[i] = buff[count-i-1];
        buff[count-i-1] = tmp;
    }
}

uint32_t unstuff_bits(uint8_t* resp,uint32_t start, uint32_t size)
{                                   
     uint32_t byte_idx_stx;                                        
     uint8_t bit_idx_stx, bit_idx;
     uint32_t ret, byte_idx;

     byte_idx_stx = start/8;
     bit_idx_stx = start - byte_idx_stx*8;

     if(size < (8 - bit_idx_stx))       // in 1 byte
     {
        return ((resp[byte_idx_stx] >> bit_idx_stx) & ((1<<size) - 1));        
     }

     ret = 0;
     
     ret =  (resp[byte_idx_stx] >> bit_idx_stx) & ((1<<(8 - bit_idx_stx)) - 1);
     bit_idx = 8 - bit_idx_stx;
     size -= bit_idx;

     byte_idx = 1;
     while(size > 8)
     {
        ret |= resp[byte_idx_stx + byte_idx] << (bit_idx);
        size -= 8;
        bit_idx += 8;
        byte_idx ++;
     }
     
    
     if(size > 0)
     {
        ret |= (resp[byte_idx_stx + byte_idx] & ((1<<size) - 1)) << bit_idx;
     }

     return ret;
}

/************************************* End *************************************/
