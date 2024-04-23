/**
 * @file patterns.c
 * @author Bo Boye
 * @brief Create patterns
 * @version 1.0
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 *
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * -------------------------------------------------------------------------------
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ws2812.h"
#include "patterns.h"

/**
 * @brief both idx and idx2 is used to make the rainbow rotate.
 * 
 */
static int idx=0;
static int idx2=0;

/**
 * @brief Fake deg2rad with 16 degree circle. There is 16 leds in Adafruit NeoPixel ring.
 * 
 * @param x Degree in 16 degree circle.
 * @return float Return degree in radian. 
 */

float deg2rad(float x) {
  return (x*pi)/8.0;
}

/**
 * @brief Generate a random pattern.
 * 
 * @param s Strip
 */
void rand_ptrn(strip *s) {
    int scl=16;
    rgb_t *p=s->ptrn;
    for(int i=0;i < s->len;i++) {
        p[i].r=(rand() & 0xff)/scl;
        p[i].g=(rand() & 0xff)/scl;
        p[i].b=(rand() & 0xff)/scl;
    }
}

rgb_t color1={16,0,0};
rgb_t color2={4,16,0};
rgb_t color3={0,4,16};
rgb_t color4={8,0,16};

/**
 * @brief A Tivoli light strip.
 * 
 * @param s Strip.
 */
void tivoli(strip *s) {
    rgb_t *p=s->ptrn;
    rgb_t t=color1; // Rotate
    color1=color2;  //   the
    color2=color3;  // colors
    color3=color4;  //   cw
    color4=t;       //

    for(int i=0;i<s->len;i+=4) {
        p[i]=color1;
        p[i+1]=color2;
        p[i+2]=color3;
        p[i+3]=color4;
    }
}

#define value 4.0

/**
 * @brief Generate a rainbow pattern CW.
 * 
 * @param s strip.
 * 
 */
void sinusbue(strip *s) {
    
    rgb_t *ptrn=s->ptrn;

      for(int i=0;i<s->len;i++) {
        int ii=i+(idx); // Rotate CW.
        float r=1+sin(deg2rad((double)ii));
        float g=1+sin(deg2rad(5.3333+(double)ii)); // Plus 120 deg.
        float b=1+sin(deg2rad(10.6666+(double)ii)); // Plus 240 deg.

        ptrn[i].r=(int8_t) (r*value);
        ptrn[i].g=(int8_t) (g*value);
        ptrn[i].b=(int8_t) (b*value);
        }

  if(idx++>=(s->len-1)) idx=0;
}
/**
 * @brief Generate a rainbow CCW.
 * 
 * @param s Strip.
 * 
 */
void sinusbue_r(strip *s) {
    rgb_t *ptrn=s->ptrn;
      for(int i=0;i<s->len;i++) {
        int ii=i-(idx2); // Rotate CCW.

        float r=1.0+sin(deg2rad((double)ii));
        float g=1.0+sin(deg2rad(5.3333+(double)ii)); // Plus 120 deg.
        float b=1.0+sin(deg2rad(10.6666+(double)ii)); // Plus 240 deg.

        ptrn[i].r=(int8_t) (r*value);
        ptrn[i].g=(int8_t) (g*value);
        ptrn[i].b=(int8_t) (b*value);
  }
  if(idx2++>=(s->len-1)) idx2=0;
}

/**
 * @brief Generate patterns.
 * 
 * @param s Array of strips.
 */
void generate_patterns(strip *s) {
    sinusbue(s);
    sinusbue_r(s+1);
    tivoli(s+2);
    rand_ptrn(s+3);
}
