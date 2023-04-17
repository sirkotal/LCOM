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

void (mouse_ih)() {
    read_KBC_output(KBC_WRITE_CMD, &current_byte, 1);
}

void (mouse_check_bytes)() {
    // Control Byte - BIT 3 is active
    if (byte_index == 0 && (current_byte & FIRST_BYTE)) {
        packet[byte_index]= current_byte;
        byte_index++;
    }
    // gets X and Y axis displacement
    else if (byte_index > 0) {                            
        packet[byte_index] = current_byte;
        byte_index++;
    }
}

int (mouse_bytes_into_packet)() {
    for (int i = 0 ; i < 3 ; i++) {
        mouse_packet.bytes[i] = packet[i];
    }

    mouse_packet.lb = packet[0] & MOUSE_LB;
    mouse_packet.rb = packet[0] & MOUSE_RB;
    mouse_packet.mb = packet[0] & MOUSE_MB;

    if (packet[0] & MOUSE_X_SIGN) {
        mouse_packet.delta_x = 0xFF00 | packet[1];
    }
    else {
        mouse_packet.delta_x = packet[1];
    }

    if (packet[0] & MOUSE_Y_SIGN) {
        mouse_packet.delta_y = 0xFF00 | packet[2];
    }
    else {
        mouse_packet.delta_y = packet[2];
    }

    mouse_packet.x_ov = packet[0] & MOUSE_X_OVERFLOW;
    mouse_packet.y_ov = packet[0] & MOUSE_Y_OVERFLOW;

    return SUCCESS;
}

int (mouse_write)(uint8_t command) {

  uint8_t mouse_response;

  do {

    if (write_KBC_command(KBC_IN_CMD, WRITE_BYTE_MOUSE)) return FAIL;
    if (write_KBC_command(KBC_OUT_CMD, command)) return FAIL;

    if (util_sys_inb(KBC_OUT_CMD, &mouse_response)) return FAIL;
    if (mouse_response == ACK) return SUCCESS;

  } while (mouse_response != ACK);

  return FAIL;
}
