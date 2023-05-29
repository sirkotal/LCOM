#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t lsb_mask = 0x00FF;

  *lsb = val & lsb_mask;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t msb_mask = 0xFF00;

  *msb = (val & msb_mask) >> 8;

  return 0;
}

#ifdef LAB3
  extern uint32_t sys_count;
#endif

int (util_sys_inb)(int port, uint8_t *value) {
  
  #ifdef LAB3
    sys_count++;
  #endif
  
  return sys_inb(port, (uint32_t *) value);
}
