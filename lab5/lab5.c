#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include "graphics.h"

#include <stdbool.h>
#include <stdint.h>


#define FAIL 1
#define SUCCESS 0

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

int(video_test_init)(uint16_t mode, uint8_t delay) {
  //char* graph_mode= vg_init(mode);

  if (set_graphic_mode(mode)) {
    return FAIL;
  }
  sleep(delay);

  if (vg_exit()) {
    return FAIL;
  }
  return SUCCESS;
}
