#ifndef WS2812_H
#define WS2812_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

/**
 * @brief The number of led strips.
 * 
 */

typedef struct rgb { 
    uint8_t r;
    uint8_t g;
    uint8_t b;

} rgb_t;

typedef struct {
  rgb_t *ptrn;
  int len;
  int cur;
  int pin;
  bool done;
  PIO pio;
  uint sm;
} strip;

strip *strip_init(strip *s,PIO pio,uint pin,int len,rgb_t *p);
void strip_reset(strip *s);
bool strip_tx(strip *s);
void strips_tx(strip *ss);
void strips_reset(strip *ss);

uint ws2812_init(PIO pio, uint pin, float freq);

#ifdef __cplusplus
}
#endif

#endif