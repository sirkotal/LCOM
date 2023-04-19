#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"
#include "kbc.h"
#include "keyboard.h"

vbe_mode_info_t mode_info;
uint8_t* frame_buffer;

int (set_graphic_mode)(uint16_t submode);

int (set_frame_buffer)(uint16_t mode);

#endif /* __GRAPHICS_H */
