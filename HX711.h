/* HX711.h */

#ifndef __HX711_h
#define __HX711_h

#include <inttypes.h>

class HX711
{
public:
    enum Gain {G_A128=1, G_B32=2, G_A64=3};
    HX711(Gain gain);

    void wait_for_ready();
    bool is_ready() const;
    int32_t read(void) const;

private:
    void set_gain() const;
    void ck_low() const;
    void ck_hi() const;
    uint8_t read_bit() const;
    Gain m_gain;
};
#endif
