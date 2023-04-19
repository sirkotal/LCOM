#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include "graphics.h"
#include "keyboard.h"

#include <stdbool.h>
#include <stdint.h>


#define FAIL 1
#define SUCCESS 0

extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (waiting_escape)() {
  uint8_t bit_no;

  if ((KBC_subscribe_ints(&bit_no))) {
    return FAIL;
  }

  int ipc_status;
  message msg;

  while (scancode != ESC_BREAK_CODE){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & bit_no) 
            KBC_int_handler();
            break;
        default:
          break; 
      }
    }
  }

  return KBC_unsubscribe_ints();
}

int (video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_graphic_mode(mode)) {
    return FAIL;
  }
  sleep(delay);

  if (vg_exit()) {
    return FAIL;
  }
  return SUCCESS;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (vg_exit()) {
    return FAIL;
  }

  return SUCCESS;
}