#include <iostream>

#include "bme280_if.h"
#include "display.h"
#include "tag_protocol.h"
#include "scale.h"
#include "control.h"

// #define BOZO

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

void tag_handler(char tag[MSGLEN], void *user_data)
{
    // tag looks like "010A0786AF25"
    // Serial.print("Got tag: ");
    // Serial.println(tag);
    // Serial.println("\r\n\r\n");

    uint64_t tag_val = strtoull(tag, 0, 16);
    std::cout << "RECEIVED TAG " << std::hex << tag_val << std::endl;
}

void setup(void)
{
    Serial.begin(115200);
    Serial2.begin(9600);
    while(!Serial) {} // Wait for serial port

    display = new(display_buff) Display();
    // Serial.println("display made");

    scale = new(scale_buff) Scale();
    // Serial.println("scale made");

    bme280 = new(bme_buff) BME280_IF();
    // Serial.println("bme made");

    tag_protocol = new(tagproto_buff) TagProtocol(tag_handler, 0);
    // Serial.println("proto made");

    control = new(control_buff) Control(*scale, *display, *bme280, *tag_protocol);
    // Serial.println("control made");

    delay(500);

    // display->calibrate();
    uint16_t dparams[5] = {508, 3292, 648, 2851, 1 };
    display->set_calibration(dparams);
}

void loop()
{
#ifdef BOZO
    for (;;)
        Serial.println("Test");
#endif
    scale->loop();
    // Serial.println("after scale loop");
    bme280->loop();
    // Serial.println("after bme loop");
    tag_protocol->loop();
    // Serial.println("after proto loop");
    control->loop();
    // Serial.println("after control loop");
    display->loop();
    // Serial.println("after display loop");
}

