/**
 * @file ws2812.c
 * @author Bo Boye
 * @brief Hardware interface for ws2812 led strips, using PIO.
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

#include "ws2812.h"

/**
 * @brief Pack a color.
 * 
 * @param r - Red.
 * @param g - Green.
 * @param b - Blue.
 * @return uint32_t Packet color word in green,red,blue order. 
 */
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

/**
 * @brief Initialise a strip and allocate a state machine too it.
 * 
 * @param s     A strip.
 * @param pio   PIO the strip will be asigned too.
 * @param pin   Out pin for the strip.
 * @param len   Buffer length.
 * @param p     Pattern buffer.
 * @return strip
 */
strip *strip_init(strip *s,PIO pio,uint pin,int len,rgb_t *p) {
  s->done=false;
  s->cur=0;
  s->pio=pio;
  s->pin=pin;
  s->ptrn=p;
  s->len=len;
  s->sm=ws2812_init(pio, pin, 800000);
  return s;
}

/**
 * @brief Reset a strip.
 * 
 * @param s A strip.
 */
void strip_reset(strip *s) {
  s->cur=0;
  s->done=false;
}

/**
 * @brief Transmit one strip.
 * @details First check if FIFO buffer is full, if it is just return with false(not done).
    If done flag is on return true (is done).
    Send RGB word(32 bit) to FIFO buffer.
    When buffer is exhausted, set done flag and return true, else return false(not done).
  * @param s One strip.
*/
bool strip_tx(strip *s) {
  if(pio_sm_is_tx_fifo_full(s->pio,s->sm)) return false;
  if(s->done) return true;
  pio_sm_put(s->pio,s->sm, urgb_u32((s->ptrn[s->cur]).r, (s->ptrn[s->cur]).g, (s->ptrn[s->cur]).b) << 8u);
  s->cur++;
  if(s->cur >= s->len) {
    s->done=true;
    return true;
  }
  return false;
}

/**
 * @brief Init PIO.
 * @details It is the code from pico SDK the example, exept it returns the state machine handle.  
 * 
 * @param pio   The PIO it is assigned too.
 * @param pin   Output pin.
 * @param freq  Baudrate (800000kbps).
 * @return uint State machine.
 */
uint ws2812_init(PIO pio, uint pin, float freq)
{
    uint sm_;
    uint offset = pio_add_program(pio, &ws2812_program);
    sm_ = pio_claim_unused_sm(pio, true);
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm_, pin, 1, true);
    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    int cycles_per_bit = ws2812_T1 + ws2812_T2 + ws2812_T3;
    float div = clock_get_hz(clk_sys) / (freq * cycles_per_bit);
    sm_config_set_clkdiv(&c, div);

    pio_sm_init(pio, sm_, offset, &c);
    pio_sm_set_enabled(pio, sm_, true);
    return sm_;
}

