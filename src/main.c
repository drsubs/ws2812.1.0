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
 *  Library to control the led rgb in WS2812 strips.
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
#include "patterns.h"


/**
 * @brief Hold the strips (see ws2812 for definition).
 * 
 */
strip strips[NUM_STRIPS];

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
 * @brief Do the generate patterns and output them.
 * @details We only have 4 pattern buffers, the last 4 out re-use the first 4.
 * @param id Not used.
 * @param user_data Not used.
 * @return int64_t Return the time to next pump call in µS.
 */
int64_t tx_pump(alarm_id_t id, void *user_data)
{    
/*    sinusbue();
    sinusbue_r();
    rand_ptrn(ptrn3,16,4);
    tivoli(ptrn3,16);
    rand_ptrn(ptrn4,16,32);
*/
    generate_patterns(strips);
    strips_tx(strips);
    strips_reset(strips);
    return 250 * 1000;
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
