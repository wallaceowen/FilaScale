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

// Used by TFT_eSPI
#define TFT_CS   27     // Chip select control pin to TFT CS
#define TFT_DC   26     // Data Command control pin to TFT DC (may be labelled RS = Register Select)
#define TFT_RST  25     // Reset pin to TFT RST (or RESET)
#define TOUCH_CS 32     // Connect to T_CS (Touch controller chip select) if XPT2046 is connected to SPI bus


#define SD_CS 14

#endif

#endif
