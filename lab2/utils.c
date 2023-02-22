#include <lcom/lcf.h>
#include "i8254.h"

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint16_t mask = 0b1; 

  if (val & mask) {
    *lsb = 0b1;
  }
  else {
    *lsb = 0b0;
  }

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  uint16_t mask = (0b1 << 15);

  *msb = val & mask;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  return sys_inb(port, (uint32_t*) value);
}
