#include <lcom/lcf.h>
#include <i8042.h>
#include <lcom/kbc.h>

int (read_KBC_status)(uint8_t* status) {
    return util_sys_inb(KBC_STATUS_REG, status);
}