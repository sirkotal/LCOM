#include <lcom/lcf.h>

#include <stdint.h>

#ifdef LAB3
  extern uint32_t sys_count;
#endif


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t mask = 0x00FF; 

  *lsb = val & mask;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t mask = 0xFF00;

  *msb = (val & mask) >> 8;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  #ifdef LAB3
    sys_count++;
  #endif
  return sys_inb(port, (uint32_t*) value);
}

uint16_t (bin_to_bcd)(uint16_t bin) {
  uint16_t bcd = 0;

  while (bin > 0) {
    bcd = (bcd << 4) + (bin % 10);
    bin /= 10;
  }

  return bcd;
}
