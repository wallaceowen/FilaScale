// ports.h

#ifndef __ports_h
#define __ports_h

#define CPU_ESP32

#ifdef CPU_ESP8266

// HX711 clk, data pins
#define PD_SCK_PIN D3
#define PD_DOUT_PIN D4

#elif defined(CPU_STM32F103)

// HX711 clk, data pins
#define PD_SCK_PIN A0
#define PD_DOUT_PIN A1


#elif defined(CPU_ESP32)

// HX711 clk, data pins
#define PD_SCK_PIN 33
#define PD_DOUT_PIN 34

#endif

#endif
