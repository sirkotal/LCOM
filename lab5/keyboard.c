#include "i8042.h"
#include "keyboard.h"
#include "kbc.h"

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
    return read_KBC_output(KBC_OUT_CMD, &scancode, 0);
}

int (keyboard_enable_ints)() { //enable int
    uint8_t commandByte;

    if (write_KBC_command(KBC_IN_CMD, KBC_READ_CMD)) {
        return FAIL;
    }         

    if (read_KBC_output(KBC_OUT_CMD, &commandByte, 0)) {
        return FAIL;
    }

    commandByte |= ENABLE_INT;  

    if (write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD)) {
        return FAIL;
    } 

    if (write_KBC_command(KBC_WRITE_CMD, commandByte)) {
        return FAIL;
    }
    return SUCCESS;
}
