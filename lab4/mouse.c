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
    if (KBC_read_data(KBD_OUT_BUF, &current_byte, true)) {
        return FAIL;
    }

    if ((byte_index == 0 && (current_byte & BIT(3))) || byte_index > 0) {
        mouse_packet.bytes[byte_index] = current_byte;
        byte_index++;
    }


    return SUCCESS;
}

/*void (mouse_check_bytes)() {
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
}*/

void (mouse_bytes_into_packet)() {
    mouse_packet.lb = mouse_packet.bytes[0] & MOUSE_LB;
    mouse_packet.rb = mouse_packet.bytes[0] & MOUSE_RB;
    mouse_packet.mb = mouse_packet.bytes[0] & MOUSE_MB;

    if (mouse_packet.bytes[0] & MOUSE_X_SIGN) {
        mouse_packet.delta_x = 0xFF00 | mouse_packet.bytes[1];
    }
    else {
        mouse_packet.delta_x = mouse_packet.bytes[1];
    }

    if (mouse_packet.bytes[0] & MOUSE_Y_SIGN) {
        mouse_packet.delta_y = 0xFF00 | mouse_packet.bytes[2];
    }
    else {
        mouse_packet.delta_y = mouse_packet.bytes[2];
    }

    mouse_packet.x_ov = mouse_packet.bytes[0] & MOUSE_X_OVERFLOW;
    mouse_packet.y_ov = mouse_packet.bytes[0] & MOUSE_Y_OVERFLOW;
}

int (mouse_write)(uint8_t command) {

  uint8_t mouse_response;

  do {

    if (write_KBC_command(KBD_CMD_REG, WRITE_BYTE_MOUSE)) return FAIL;
    if (write_KBC_command(KBD_IN_BUF, command)) return FAIL;

    if (util_sys_inb(KBD_OUT_BUF, &mouse_response)) return FAIL;
    if (mouse_response == ACK) return SUCCESS;

  } while (mouse_response != ACK);

  return FAIL;
}
