// bme280 interface code

#ifndef __bme280_if_h
#define __bme280_if_h

#include "fila_config.h"

class BME280_IF
{
public:
    BME280_IF(FilaConfig *fc);

    void loop(void);

    float temp(void) const { return m_temp; }
    float humid(void) const { return m_humid; }
    float pres(void) const { return m_pres; }
    float alt(void) const { return m_alt; }

private:
    FilaConfig  *m_fc;
    bool         m_initialized;
    float        m_temp;
    float        m_humid;
    float        m_pres;
    float        m_alt;
};

#endif
