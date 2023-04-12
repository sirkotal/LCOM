#include "mouse.h"

#define FAIL 1
#define SUCCESS 0

int mouse_hook_id = 2;

uint8_t byte_index = 0;       // [0..2]
uint8_t packet[3];            // "data package"
uint8_t current_byte;         // most recently read byte

struct packet mouse_packet;

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
    return KBC_read_data(&current_byte);
}

void (mouse_check_bytes)() {
    // Control Byte - BIT 3 is active
    if (byte_index == 0 && (current_byte & CONTROL_BIT)) {
        packet[byte_index]= current_byte;
        byte_index++;
    }
    // gets X and Y axis displacement
    else if (byte_index > 0) {                            
        packet[byte_index] = current_byte;
        byte_index++;
    }
}