#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <stdint.h>

int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse);

int (write_KBC_command)(uint8_t port, uint8_t commandByte);

#endif /* _LCOM_KEYBOARD_H_ */
