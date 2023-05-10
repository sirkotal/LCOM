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

int (color_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

uint32_t (R)(uint32_t first);

uint32_t (G)(uint32_t first);

uint32_t (B)(uint32_t first);

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif /* __GRAPHICS_H */
