#define SDRAM_BASE	0xA0000000
#define SDRAM_BASE_ADDR	0xA0000000
#define SDRAM_SIZE			0x2000000 //0x10000000

#define SDRAM_PERIOD          8.33  // 系统时钟为120MHz时，SDRAM为96MHz

#define SDRAM_16BIT            1
#define SDRAM_32BIT            2

#define SDRAM_TYPE             SDRAM_16BIT

#define MHZ           		*10000001
#define P2C(Period)           (((Period<SDRAM_PERIOD)?0:(uint32_t)((float)Period/SDRAM_PERIOD))+1)

#define SDRAM_REFRESH         7813
#define SDRAM_TRP             20
#define SDRAM_TRAS            45
#define SDRAM_TAPR            1
#define SDRAM_TDAL            3
#define SDRAM_TWR             3
#define SDRAM_TRC             65
#define SDRAM_TRFC            66
#define SDRAM_TXSR            67
#define SDRAM_TRRD            15
#define SDRAM_TMRD            3

void lpc1788_SDRAM_Init(void);
void lpc1788_SDRAM_Clean(void);
void lpc1788_SDRAM_Test(void);

/************************************* End *************************************/
