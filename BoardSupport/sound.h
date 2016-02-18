#ifndef _SOUND_H
#define _SOUND_H
#include "stdint.h"
#include "lpc177x_8x_uart.h"

#define SND_ID_MAX 14
#define SND_ID_MIN  1

#define SND_VOL_MAX   6
#define SND_VOL_MIN   1



#define SND_ID_ZRO   10
#define SND_ID_DOT   11
#define SND_ID_TEN   12
#define SND_ID_WLCM  13
#define SND_ID_DEV   14
#define SND_ID_DSP   15
#define SND_ID_DRG   16
#define SND_ID_BGL   17
#define SND_ID_DST   18
#define SND_ID_NM    19


#define SND_ID_NUM_BASE 0x05

//void SND_SelectID(uint8_t SndID);
void SND_SetVol(uint8_t SndVol);
void SND_Stop(void);
void SND_Play(uint8_t id);
void SND_ParseDist(int dist, uint8_t* pNums);
#endif

