/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>

#include "bme280_if.h"

//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

static Adafruit_BME280 bme; // I2C

BME280_IF::BME280_IF(FilaConfig *fc) :
    m_fc(fc),
    m_temp(0.0),
    m_humid(0.0),
    m_pres(0.0),
    m_alt(0.0)
{
    m_initialized = bme.begin(0x76)?true:false;
}

void BME280_IF::loop()
{ 
    m_temp = bme.readTemperature();
    m_pres = bme.readPressure();
    m_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    m_humid = bme.readHumidity();
}
