#include "i8042.h"
#include "keyboard.h"

#define FAIL 1
#define SUCCESS 0

int read_KBC_output(uint8_t port, uint8_t *output, uint8_t mouse) {
  uint8_t status;
    
  while (true) {

    if (util_sys_inb(KBC_STATUS_REG, &status)) {            
      return FAIL;
    }

    if ((status & BIT(0))) {
      if(util_sys_inb(port, output)) {            
        return FAIL;
      }
      
      if((status & BIT(7))) {      
        return FAIL;
      }

      if((status & BIT(6))) {                 
        return FAIL;
      }

      if (mouse && !(status & BIT(5))) {    
        return FAIL;
      } 

      if (!mouse && (status & BIT(5))) {   
        return FAIL;
      } 

      return SUCCESS;
    }
    
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  
  return FAIL;
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte) {
    uint8_t status;

    while (true) {

      if (util_sys_inb(KBC_STATUS_REG, &status)) {
        return FAIL;
      }

      if ((status & FULL_IN_BUFFER) == 0) {

        if (sys_outb(port, commandByte)) {
          return FAIL;
        }

        return SUCCESS;
      }
      
      tickdelay(micros_to_ticks(WAIT_KBC));
    }
    return FAIL;
}
