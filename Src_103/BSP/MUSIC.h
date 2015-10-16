#ifndef  MUSIC_H
#define  MUSIC_H
#include <stm32f10x.h>


void Send_Music_Data(u8 data);


void Music_Init(void);
void Sound_U(void);
void Sound_D(void);
void OpenMusic(void);
void ShutDown(void);


void Choose_Alarm1(void);
void Choose_Alarm2(void);
void Choose_Alarm3(void);

void Close_Alarm1(void);
void Close_Alarm2(void);
void Close_Alarm3(void);





#endif












