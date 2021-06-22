// val_mapper.h

#ifndef __val_mapper_h
#define __val_mapper_h

#include <stdint.h>
#include <Arduino.h>

struct ValMapper
{
    ValMapper(const char *n, uint16_t inl, uint16_t inh, uint16_t outl, uint16_t outh)
        : name(n), inlo(inl), inhi(inh), outlo(outl), outhi(outh)
    {
#ifdef DEBUG_VALMAPPER
        char msg[56];
        sprintf(msg, "%s mapper: inlo %u inhi %u outlo %u outhi %u",
            name, inlo, inhi, outlo, outhi);
        Serial.println(msg);
#endif
    }

    uint16_t map(uint16_t val) const
    {
        // return ::map(val, inlo, inhi, outlo, outhi);
        uint16_t mapped = ::map(val, inlo, inhi, outlo, outhi);
#ifdef DEBUG_VALMAPPER
        char msg[56];
        sprintf(msg, "%s mapped %u to %u", name, val, mapped);
        Serial.println(msg);
#endif
        return mapped;
    }

    const char *name;
    uint16_t inlo;
    uint16_t inhi;
    uint16_t outlo;
    uint16_t outhi;
};

#endif
