#ifndef _SOUND_H
#define _SOUND_H
#include "stdint.h"
#include "lpc177x_8x_uart.h"

#define SND_ID_MAX  5
#define SND_ID_MIN  0

#define SND_VOL_MAX   4
#define SND_VOL_MIN   0

void SND_SelectID(uint8_t SndID);
void SND_SetVol(uint8_t SndVol);
void SND_Stop(void);
void SND_Play(void);
#endif