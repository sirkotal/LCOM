#include "mouse.h"

#define FAIL 1
#define SUCCESS 0

int mouse_hook_id = 2;

int (mouse_subscribe_ints)(uint8_t *bit_no) {
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_ints)() {
    if (sys_irqrmpolicy(&mouse_hook_id)) {
        return FAIL;
    }
    return SUCCESS;
}

int (mouse_int_handler)() {
    return SUCCESS;
}