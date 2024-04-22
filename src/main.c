/*
 * -------------------------------------------------------------------------------
 *
 * Copyright (c) 2024, Bo Boye
 * All rights reserved.
 *
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * -------------------------------------------------------------------------------
 *
 *  Library to control the led rgb WS2812
 *  Useing multiply PIO programs.
 *  
 *
 * -------------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "ws2812.h"

/**
 * @brief The number of led strips.
 * 
 */
#define NUM_STRIPS 8

#define pi 3.1415926535897931

/**
 * @brief Hold the strips (see ws2812 for definition).
 * 
 */
strip strips[NUM_STRIPS];

/**
 * @brief both idx and idx2 is used to make the rainbow rotate.
 * 
 */
static int idx=0;
static int idx2=0;

/// @brief Pattern buffers
rgb_t ptrn[16];
rgb_t ptrn2[16];
rgb_t ptrn3[16];
rgb_t ptrn4[16];

/**
 * @brief Transmit all pattern buffers. We iterate all strips until all returns true(all strips are done).
 * 
 * @param ss Array of strips.
 */
void strips_tx(strip *ss) {
  bool done=false;
  while(!done) {
    done=true;
    for(int i=0;i<NUM_STRIPS;i++) {
      if(!strip_tx(ss+i)) done=false;
    }
  }
}
/**
 * @brief Reset all strips
 * 
 * @param ss Array of strips.
 */
void strips_reset(strip *ss) {
  for(int i=0;i<NUM_STRIPS;i++) strip_reset(ss+i);
}

/**
 * @brief Fake deg2rad with 16 degree circle. There is 16 leds in Adafruit NeoPixel ring.
 * 
 * @param x Degree in 16 degree circle.
 * @return float Return degree in radian. 
 */

float deg2rad(float x) {
  return (x*pi)/8.0;
}
#define value 4.0

/**
 * @brief Generate a random pattern.
 * 
 * @param p Pattern
 * @param len Length of pattern.
 * @param scl Scale result.
 */
void rand_ptrn(rgb_t *p,int len,int scl) {
  for(int i=0;i < len;i++) {
    p[i].r=(rand() & 0xff)/scl;
    p[i].g=(rand() & 0xff)/scl;
    p[i].b=(rand() & 0xff)/scl;
  }
}

/**
 * @brief Generate a rainbow pattern CW.
 * 
 */
void sinusbue() {
  for(int i=0;i<16;i++) {
    int ii=i+(idx); // Rotate CW.
    float r=1+sin(deg2rad((double)ii));
    float g=1+sin(deg2rad(5.3333+(double)ii)); // Plus 120 deg.
    float b=1+sin(deg2rad(10.6666+(double)ii)); // Plus 240 deg.

    ptrn[i].r=(int8_t) (r*value);
    ptrn[i].g=(int8_t) (g*value);
    ptrn[i].b=(int8_t) (b*value);
  }
  if(idx++>=15) idx=0;
}
/**
 * @brief Generate a rainbow CCW.
 * 
 */
void sinusbue_r() {
  for(int i=0;i<16;i++) {
    int ii=i-(idx2); // Rotate CCW.

    float r=1.0+sin(deg2rad((double)ii));
    float g=1.0+sin(deg2rad(5.3333+(double)ii)); // Plus 120 deg.
    float b=1.0+sin(deg2rad(10.6666+(double)ii)); // Plus 240 deg.

    ptrn2[i].r=(int8_t) (r*value);
    ptrn2[i].g=(int8_t) (g*value);
    ptrn2[i].b=(int8_t) (b*value);
  }
  if(idx2++>=15) idx2=0;
}

/**
 * @brief Do the generate patterns and output them.
 * @details We only have 4 pattern buffers, the last 4 out re-use the first 4.
 * @param id Not used.
 * @param user_data Not used.
 * @return int64_t Return the time to next pump call in µS.
 */
int64_t tx_pump(alarm_id_t id, void *user_data)
{
    sinusbue();
    sinusbue_r();
    rand_ptrn(ptrn3,16,4);
    rand_ptrn(ptrn4,16,32);

    strips_tx(strips);
    strips_reset(strips);
    return 25 * 1000;
}

/**
 * @brief First we init the system, setup the 8 PIO programs,  and then fire the tx_pump.
 * 
 * @return int Never gona happend.
 */
int main()
{    
    PIO pio_0 = pio0; // values: pio0, pio1
    PIO pio_1 = pio1; 
    const uint WS2812_PIN1 = 16;
    const uint WS2812_PIN2 = 15;
    const uint WS2812_PIN3 = 14;
    const uint WS2812_PIN4 = 13;
    const uint WS2812_PIN5 = 12;
    const uint WS2812_PIN6 = 11;
    const uint WS2812_PIN7 = 10;
    const uint WS2812_PIN8 = 9;

    stdio_init_all();
    /*
      Four strips can be assign to each pio.
      We only have 4 pattern buffers, the last 4 out re-use the first 4.
    */
    strip_init(strips,pio_0,WS2812_PIN1,16,ptrn);
    strip_init(strips+1,pio_0,WS2812_PIN2,16,ptrn2);
    strip_init(strips+2,pio_0,WS2812_PIN3,16,ptrn3);
    strip_init(strips+3,pio_0,WS2812_PIN4,16,ptrn4);

    strip_init(strips+4,pio_1,WS2812_PIN5,16,ptrn);
    strip_init(strips+5,pio_1,WS2812_PIN6,16,ptrn2);
    strip_init(strips+6,pio_1,WS2812_PIN7,16,ptrn3);
    strip_init(strips+7,pio_1,WS2812_PIN8,16,ptrn4);

    add_alarm_in_ms(25, tx_pump, NULL, false);
    
    while (1)
    {
      tight_loop_contents();
    }
}
