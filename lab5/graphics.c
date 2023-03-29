#include "graphics.h"

#define FAIL 1
#define SUCCESS 0

int (set_graphic_mode)(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); // initialize all values at 0
    reg86.intno = 0x10;               // intno is always 0x10      
    reg86.ah = 0x4F;                  // AX MSB
    reg86.al = 0x02;                  // AX LSB (graphic mode is 0x02)

    reg86.bx = submode | BIT(14);     // linear memory addressing
    if (sys_int86(&reg86)) {     
        printf("Set graphic mode failed\n");
        return FAIL;
    }
    return SUCCESS;
}
