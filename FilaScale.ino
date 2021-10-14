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

#include <SoftwareSerial.h>

#include "bme280_if.h"
#include "tag_protocol.h"
#include "octo_protocol.h"
#include "scale.h"
#include "control.h"
#include "vnew.h"

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

SoftwareSerial softSerial(2, 3);

void setup(void)
{
    Serial.begin(115200);
    softSerial.begin(9600);
    while(!Serial) {} // Wait for serial port

    scale = new(scale_buff) Scale();
    bme280 = new(bme_buff) BME280_IF();
    tag_protocol = new(tagproto_buff) TagProtocol(softSerial);
    octo_protocol = new(octo_proto_buff) OctoProtocol();
    control = new(control_buff) Control(*scale, *bme280, *tag_protocol, octo_protocol);
}

void loop()
{

    // Serial.println("scale loop");
    scale->loop();
    // Serial.println("bme loop");
    bme280->loop();
    // Serial.println("tag loop");
    tag_protocol->loop();
    // Serial.println("control loop");
    control->loop();
}

