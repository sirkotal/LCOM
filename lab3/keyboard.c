#include <lcom/lcf.h>
#include "keyboard.h"
#include "i8042.h"

int kbc_hook = 1;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no); {
    *bit_no = BIT(kbc_hook);
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbc_hook);
}