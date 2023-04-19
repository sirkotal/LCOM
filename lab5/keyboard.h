#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>


#define ESC_BREAK_CODE    0x81 // 0x81 is the break code of the ESC key
#define TWO_BYTE_CODE     0xE0 // 0xE0 is the first byte of a two byte code

int (KBC_subscribe_ints)(uint8_t *bit_no);

int (KBC_unsubscribe_ints)();

int (KBC_int_handler)();

int (keyboard_enable_ints)();

#endif /* _LCOM_KEYBOARD_H_ */
