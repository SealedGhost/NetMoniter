#ifndef _SOUND_H
#define _SOUND_H
#include "stdint.h"
#include "lpc177x_8x_uart.h"

#define SND_ID_MAX 14
#define SND_ID_MIN  1

#define SND_VOL_MAX   5
#define SND_VOL_MIN   1

#define SND_ID_WLCM  0x01
#define SND_ID_DSP   0x02
#define SND_ID_DRG   0x03
#define SND_ID_BGL   0x04

#define SND_ID_NUM_BASE 0x05

void SND_SelectID(uint8_t SndID);
void SND_SetVol(uint8_t SndVol);
void SND_Stop(void);
void SND_Play(void);
#endif

