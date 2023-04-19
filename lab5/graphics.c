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

int (set_frame_buffer)(uint16_t mode) {
  memset(&mode_info, 0, sizeof(mode_info));   // get mode info
  
  if (vbe_get_mode_info(mode, &mode_info)) {
    return FAIL;
  }

  unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;
  
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr;
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size;
  
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
    printf("Failed to allocate physical memory\n");
    return FAIL;
  }

  frame_buffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);
  if (frame_buffer == NULL) {
    printf("Failed to allocate virtual memory\n");
    return FAIL;
  }

  return SUCCESS;
}