#include "mouse.h"

#define FAIL 1
#define SUCCESS 0

int mouse_hook_id = 2;

uint8_t byte_index = 0;       // [0..2]
uint8_t packet[3];            // "data package"
uint8_t current_byte;         // most recently read byte

struct packet mouse_packet;

int (mouse_subscribe_int)(uint8_t *bit_no) {
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id)) {
        return FAIL;
    }
    return SUCCESS;
}

int (mouse_int_handler)() {
    return KBC_read_data(&current_byte, 1);
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

void (mouse_bytes_into_packet)() {
    for (int i = 0 ; i < 3 ; i++) {
        mouse_packet.bytes[i] = packet[i];
    }

    mouse_packet.lb = packet[0] & MOUSE_LB;
    mouse_packet.rb = packet[0] & MOUSE_RB;
    mouse_packet.mb = packet[0] & MOUSE_MB;

    if (packet[0] & MOUSE_X_SIGN) {
        return SUCCESS;
    }
    else {
        mouse_packet.delta_x = packet[1];
    }

    if (packet[0] & MOUSE_Y_SIGN) {
        return SUCCESS;
    }
    else {
        mouse_packet.delta_y = packet[2];
    }
    mouse_packet.delta_x = (packet[0] & MOUSE_X_SIGN) ? (0xFF00 | packet[1]) : packet[1];
    mouse_packet.delta_y = (packet[0] & MOUSE_Y_SIGN) ? (0xFF00 | packet[2]) : packet[2];

    mouse_packet.x_ov = packet[0] & MOUSE_X_OVERFLOW;
    mouse_packet.y_ov = packet[0] & MOUSE_Y_OVERFLOW;
}