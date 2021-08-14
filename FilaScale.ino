#include <iostream>

#include "bme280_if.h"
#include "display.h"
#include "tag_protocol.h"
#include "scale.h"
#include "control.h"

Display *display = 0;
char display_buff[sizeof(Display)];

Scale *scale = 0;
char scale_buff[sizeof(Scale)];

BME280_IF *bme280 = 0;
char bme_buff[sizeof(BME280_IF)];

TagProtocol *tag_protocol = 0;
char tagproto_buff[sizeof(TagProtocol)];

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

    display = new(display_buff) Display();
    // Serial.println("display made");

    scale = new(scale_buff) Scale();
    Serial.println("scale made");

    bme280 = new(bme_buff) BME280_IF();
    Serial.println("bme made");

    tag_protocol = new(tagproto_buff) TagProtocol();
    Serial.println("proto made");

    control = new(control_buff) Control(*scale, *display, *bme280, *tag_protocol);
    Serial.println("control made");

    delay(500);

    // display->calibrate();
    uint16_t dparams[5] = {508, 3292, 648, 2851, 1 };
    display->set_calibration(dparams);
}

void loop()
{
    scale->loop();
    bme280->loop();
    tag_protocol->loop();
    control->loop();
    display->loop();
}

