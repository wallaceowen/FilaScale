/*
	crc.h
*/

#ifndef __crc_h
#define __crc_h

#include <stdint.h>
// #include <types.h>

typedef uint16_t CRCType;

extern void calcCRC16(uint8_t p, CRCType *crc);
extern void calcCRCArray16(const uint8_t *p, uint16_t len, CRCType *crc);

#endif
