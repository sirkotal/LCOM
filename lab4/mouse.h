#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"

int (mouse_subscribe_ints)(uint8_t *bit_no);

int (mouse_unsubscribe_ints)();

int (mouse_int_handler)();

void (mouse_check_bytes)();

#endif /* _LCOM_MOUSE_H_ */