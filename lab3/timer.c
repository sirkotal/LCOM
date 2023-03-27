#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "../lab2/i8254.h"

#define FAIL 1
#define SUCCESS 0

int timer_hook_id = 0;
int timer_int_counter = 0;

uint16_t (bin_to_bcd)(uint16_t bin) {
  uint16_t bcd = 0;

  while (bin > 0) {
    bcd = (bcd << 4) + (bin % 10);
    bin /= 10;
  }

  return bcd;
}


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2) {
    return FAIL;
  }

  uint8_t st;
  uint16_t timer_freq = (uint16_t) (TIMER_FREQ / freq);

  if (timer_get_conf(timer, &st)) {
    return FAIL;
  }

  /*max freq is 0xFFFF for binary count, 0x9999 for BCD */
  if (freq < TIMER_FREQ / 0xFFFF) {
    return FAIL;
  } 
  else if ((st & BIT(0)) && freq < TIMER_FREQ / 0x9999) {
    return FAIL;
  }

  if (st & BIT(0)) {
    timer_freq = bin_to_bcd(timer_freq);
  }

  uint8_t ctrl_timer; 
  if (timer) {
    ctrl_timer = BIT(5 + timer);
  }
  else {
    ctrl_timer = TIMER_SEL0;
  }

  uint8_t ctrl_word = (ctrl_timer | TIMER_LSB_MSB | (st & (BIT(3) | BIT(2) | BIT(1))) | (st & BIT(0)));

  if (sys_outb(TIMER_CTRL, ctrl_word)) {
    return FAIL;
  }

  uint8_t freq_lsb;
  util_get_LSB(timer_freq, &freq_lsb);

  uint8_t freq_msb;
  util_get_MSB(timer_freq, &freq_msb);

  if (sys_outb(TIMER_0 + timer, freq_lsb)) {
    return FAIL;
  }

  if (sys_outb(TIMER_0 + timer, freq_msb)) {
    return FAIL;
  }

  return SUCCESS;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(timer_hook_id);
  
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)) {
    return FAIL;
  }
  return SUCCESS;
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
  timer_int_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2) {
    return FAIL;
  }

  if (sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_)) {
    return FAIL;
  }
  
  return util_sys_inb(TIMER_0 + timer, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer > 2) {
    return FAIL;
  }
  
  union timer_status_field_val conf;

  switch(field) {
      case tsf_all:
        conf.byte = st;
        break;
      case tsf_initial:
        conf.in_mode = (st & TIMER_LSB_MSB) >> 4;
        break;
      case tsf_mode:
        conf.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
        if (conf.count_mode == 6) {
          conf.count_mode = 2;
        }
        else if (conf.count_mode == 7) {
          conf.count_mode = 3;
        }

        break;
      case tsf_base:
        conf.bcd = st & BIT(0);
        break;
  }

  return timer_print_config(timer, field, conf);
}
