#include "i8042.h"
#include "keyboard.h"

#define FAIL 1
#define SUCCESS 0

int kbc_hook_id = 1;
uint8_t scancode = 0;

int (KBC_read_data)(int port, uint8_t *data, bool mouse_check) {
  uint8_t status;

  while (true) {
    if (util_sys_inb(KBD_STAT_REG, &status)) {
      return FAIL;
    }

    if (status & KBD_STAT_OBF) {

      if (util_sys_inb(port, data)) {
        return FAIL;
      }

      // If there is an error, we need to read the byte from the output buffer
      if (status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT)) {
        return FAIL;
      }

      if (mouse_check && !(status & KBD_STAT_AUX)) {
        return FAIL;
      }

      if (!mouse_check && (status & KBD_STAT_AUX)) {
        return FAIL;
      }

      return SUCCESS;
      // return util_sys_inb(KBD_OUT_BUF, data);
    }  
    tickdelay(micros_to_ticks(WAIT_KBC));
  }

  return FAIL;
}


int (write_KBC_command)(int port, uint8_t commandByte) {
    uint8_t status;

    while (true) {
        if (util_sys_inb(KBD_STAT_REG, &status)){
          return FAIL;
        }

        if ((status & KBD_STAT_IBF)){
          tickdelay(micros_to_ticks(WAIT_KBC));
          continue;
        }
        
        if (sys_outb(port, commandByte)) {
          return FAIL;
        }

        return SUCCESS;
    }
    
    return FAIL;
}
