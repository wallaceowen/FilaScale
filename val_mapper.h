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
    }

    uint16_t map(uint16_t val) const
    {
        return ::map(val, inlo, inhi, outlo, outhi);
        // uint16_t mapped = ::map(val, inlo, inhi, outlo, outhi);
        // return mapped;
    }

    const char *name;
    uint16_t inlo;
    uint16_t inhi;
    uint16_t outlo;
    uint16_t outhi;
};

#endif
