#ifndef _STR_H
#define _STR_H

#include <lpc_types.h>


extern char * pStrBuf;


void disttostr( char * str, int num);
long strtoi(const char*str);
void lltostr(long l_o_l,char *str);
char * ttoi(long num , char *str);
void str_trim(char * pszSrc, int nMaxLen);

Bool Mem_isEqual(void* _pSrc, void* _pDst, unsigned int nSize);

#endif


