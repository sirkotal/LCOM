#include "i8042.h"
#include "keyboard.h"

#define FAIL 1
#define SUCCESS 0

int kbc_hook_id = 1;
uint8_t scancode = 0;

int (KBC_subscribe_ints)(uint8_t *bit_no) {
    *bit_no = BIT(kbc_hook_id);
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbc_hook_id);
}

int (KBC_unsubscribe_ints)() {
    if (sys_irqrmpolicy(&kbc_hook_id)) {
        return FAIL;
    }
    return SUCCESS;
}

int (KBC_int_handler)() {
    return KBC_read_data(&scancode);
}

int (keyboard_enable_ints)() { //enable int
    uint8_t commandByte;

    if (read_KBC_command(&commandByte)) {
        return FAIL;
    }

    commandByte |= KBD_CMD_INT;

    if (write_KBC_command(commandByte)) {
        return FAIL;
    }

    return SUCCESS;
}

int (KBC_read_data)(uint8_t *data) {
  uint8_t status;
  while (true) {
    if (util_sys_inb(KBD_STAT_REG, &status)) {
      return FAIL;
    }

    if (status & KBD_STAT_OBF) {
    // If there is an error, we need to read the byte from the output buffer
      if (status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT | KBD_STAT_AUX)) {
        return FAIL;
      }

      return util_sys_inb(KBD_OUT_BUF, data);
    }  
    tickdelay(micros_to_ticks(WAIT_KBC));
  }

  return FAIL;
}


int (write_KBC_command)(uint8_t commandByte) {
    for (int i = 0; i < 3; i++) {
        uint8_t status;
        if (util_sys_inb(KBD_STAT_REG, &status)) {
            return FAIL;
        }

        if (status & (KBD_STAT_PARITY | KBD_STAT_TIMEOUT | KBD_STAT_AUX)) {
            return FAIL;
        }

        // If the input buffer is full, we need to wait
        if (status & KBD_STAT_IBF) {
            tickdelay(micros_to_ticks(WAIT_KBC));
            continue;
        }

        // If the input buffer is empty, we can write the command byte
        if (sys_outb(KBD_CMD_REG, commandByte)) {
            return FAIL;
        }

        break;
    }

    if (sys_outb(KBD_CMD_REG, KBD_CMD_WRITE_CMD)) {
        return FAIL;
    }

    if (sys_outb(KBD_IN_BUF, commandByte)) {
        return FAIL;
    }

    return SUCCESS;

}

int (read_KBC_command)(uint8_t *command_byte) {
  if (write_KBC_command(KBD_CMD_READ_CMD)) {
    return FAIL;
  }

  if (util_sys_inb(KBD_OUT_BUF, command_byte)) {
    return FAIL;
  }

  return SUCCESS;
}
