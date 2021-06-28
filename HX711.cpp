/* HX711.cpp */

#include <Arduino.h>
#include "HX711.h"
#include "Averager.h"
#include "ports.h"

void init_spi()
{
    // Make this an input pin
    pinMode(PD_SCK_PIN, OUTPUT);
    pinMode(PD_DOUT_PIN, INPUT);
}

static uint8_t spdr;

uint8_t transfer(uint8_t val)
{
    spdr = 0;

    for (uint8_t i = 0; i < 8; ++i)
    {
        // Raise the clock
        // PD_SCK_PORT |= _BV(PD_SCK_PIN);
        digitalWrite(PD_SCK_PIN, 1);

        delayMicroseconds(1);

        // Read the bit
        if (digitalRead(PD_DOUT_PIN))
            spdr |= _BV(7-i);

        // Lower the clock
        // PD_SCK_PORT &= ~_BV(PD_SCK_PIN);
        digitalWrite(PD_SCK_PIN, 0);

        delayMicroseconds(2);
    }

    return spdr;
}

// Initialize the HX711
HX711::HX711(Gain gain) :
    m_gain(gain)
{
    init_spi();
    ck_low();
    read();
}

// If DOUT is hi we're not ready
bool HX711::is_ready() const
{
	if (digitalRead(PD_DOUT_PIN))
        return false;
    else
        return true;
}

void HX711::ck_low() const
{
    digitalWrite(PD_SCK_PIN, 0);
}

void HX711::ck_hi() const
{
    digitalWrite(PD_SCK_PIN, 1);
}

void HX711::set_gain() const
{
    for (uint16_t i = 0; i < m_gain; ++i)
    {
        ck_hi();
        ck_low();
    }
}

void HX711::wait_for_ready()
{
    ck_low();
    while (digitalRead(PD_DOUT_PIN))
        ;
}

int32_t HX711::read(void) const
{
    int32_t result = 0L;

    for (uint16_t i = 0; i < 3; ++i)
    {
        uint32_t val = transfer(0);
        result |= (val << (8*(3-i)));
    }

    result >>= 8;

    // Set the gain
    set_gain();

    return result;
}
