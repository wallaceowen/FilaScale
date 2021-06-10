#include "bme280_if.h"
#include "display.h"
#include "protocol.h"
#include "scale.h"
#include "control.h"

// #define BOZO

Display *display = 0;
char display_buff[sizeof(Display)];

Scale *scale = 0;
char scale_buff[sizeof(Scale)];

BME280_IF *bme280 = 0;
char bme_buff[sizeof(BME280_IF)];

Protocol *protocol = 0;
char proto_buff[sizeof(Protocol)];

Control *control = 0;
char control_buff[sizeof(Control)];

void setup(void)
{
    Serial.begin(115200);
    while(!Serial) {} // Wait for serial port

    display = new(display_buff) Display();
    Serial.println("display made");

    scale = new(scale_buff) Scale();
    Serial.println("scale made");

    bme280 = new(bme_buff) BME280_IF();
    Serial.println("bme made");

    protocol = new(proto_buff) Protocol(*scale);
    Serial.println("proto made");

    control = new(control_buff) Control(*scale, *display, *bme280, *protocol);
    Serial.println("control made");

    delay(500);
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
    protocol->loop();
    // Serial.println("after proto loop");
    control->loop();
    // Serial.println("after control loop");
    display->loop();
    // Serial.println("after display loop");
}

