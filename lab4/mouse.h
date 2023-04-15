#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int (mouse_int_handler)();

// void (mouse_check_bytes)();

void (mouse_bytes_into_packet)();

int (mouse_write)(uint8_t command);

#endif /* _LCOM_MOUSE_H_ */
