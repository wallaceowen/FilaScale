#include "bme280_if.h"
#include "display.h"
#include "protocol.h"
#include "scale.h"
#include "control.h"

Display *display = 0;
char display_buff[sizeof(Scale)];

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
    display = new(display_buff) Display();

    scale = new(scale_buff) Scale();

    bme280 = new(bme_buff) BME280_IF();

    protocol = new(proto_buff) Protocol(*scale);

    control = new(control_buff) Control(*scale, *display, *bme280, *protocol);
}

void loop()
{
    scale->loop();
    bme280->loop();
    protocol->loop();
    control->loop();
    display->loop();
}

