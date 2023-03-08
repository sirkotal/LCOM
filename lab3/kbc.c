#include <lcom/lcf.h>
#include <i8042.h>
#include <lcom/kbc.h>

int (read_KBC_status)(uint8_t* status) {
    return util_sys_inb(KBC_STATUS_REG, status);
}

int (read_KBC_output)(uint8_t port, uint8_t *output) {
    return util_sys_inb(port, output);
}