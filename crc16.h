/*
	crc.h
*/

#ifndef __crc_h
#define __crc_h

#include <stdint.h>
// #include <types.h>

typedef unsigned short CRCType;

extern void calcCRC16(unsigned char p, CRCType *crc);
extern void calcCRCArray16(unsigned char *p, unsigned len, CRCType *crc);

#endif
