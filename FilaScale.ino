/*
 * Filament Monitor for print farm arrays
 *
 * Monitors filament temp, humidity, weight, listens for RFID tags.
 *
 * Displays this info on the status screen
 *
 * TODO: Activate a low-wattage heater to dry out overly humid filament using a crydom SSR switching a
 *       terrarium heater
 *
 * This is as well the first client of a small menu/dialog/buttons/sliders widget set, presented here
 * tightly integrated with the application.  This needs some polish, but is light-weight, performant
 * and easy (for me) to code against.
 *
 */

#include <iostream>
#include <FS.h>
#include <SD.h>

#include "bme280_if.h"
#include "display.h"
#include "tag_protocol.h"
#include "octo_protocol.h"
#include "scale.h"
#include "control.h"
#include "fila_config.h"
#include "sd_if.h"

// For storing config, logging humidity over time, etc.
SD_IF *sd_if = 0;
char sdif_buff[sizeof(SD_IF)];

// The filament monitor config object, which uses the SD interface
FilaConfig *fila_config = 0;
char config_buff[sizeof(FilaConfig)];

// The display
Display *display = 0;
char display_buff[sizeof(Display)];

// The scale code (except the bit-banging)
Scale *scale = 0;
char scale_buff[sizeof(Scale)];

// Stock Adafruit BME280 lib
BME280_IF *bme280 = 0;
char bme_buff[sizeof(BME280_IF)];

// The RFID tag reader
TagProtocol *tag_protocol = 0;
char tagproto_buff[sizeof(TagProtocol)];

// Octoprint interface
OctoProtocol *octo_protocol = 0;
char octo_proto_buff[sizeof(OctoProtocol)];

// Filament Monitor control loop is where the action is
Control *control = 0;
char control_buff[sizeof(Control)];

void setup(void)
{
#ifdef TAG_PORT_IS_SERIAL
    Serial.begin(9600);
#elif defined(TAG_PORT_IS_SERIAL2)
    Serial.begin(115200);
    Serial2.begin(9600);
#endif
    while(!Serial) {} // Wait for serial port

    sd_if = new(sdif_buff) SD_IF();
    Serial.println("sd_if made");

    if (sd_if->card_present())
    {
        uint8_t cardType = SD.cardType();
        Serial.print("card type ");
        Serial.println(cardType);
    }

    fila_config = new(config_buff) FilaConfig(sd_if);
    display = new(display_buff) Display(fila_config);
    scale = new(scale_buff) Scale(fila_config);
    bme280 = new(bme_buff) BME280_IF(fila_config);
    tag_protocol = new(tagproto_buff) TagProtocol(fila_config);
    octo_protocol = new(octo_proto_buff) OctoProtocol();
    control = new(control_buff) Control(*scale, *display, *bme280, *tag_protocol, fila_config, octo_protocol);
}

void loop()
{
    scale->loop();
    bme280->loop();
    tag_protocol->loop();
    control->loop();
    display->loop();
}

