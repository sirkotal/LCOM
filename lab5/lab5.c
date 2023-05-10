#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include "graphics.h"
#include "keyboard.h"

#include <stdbool.h>
#include <stdint.h>


#define FAIL 1
#define SUCCESS 0

extern uint8_t scancode;
extern vbe_mode_info_t mode_info;

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

void (kbc_ih)() {
  KBC_int_handler();
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
            kbc_ih();
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
  uint32_t next_color;

  if (set_frame_buffer(mode)) {
    return FAIL;
  }

  if (set_graphic_mode(mode)) {
    return FAIL;
  }

  if (mode_info.BitsPerPixel == 32) {
    next_color = color;
  } else {
    next_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }

  if (vg_draw_rectangle(x, y, width, height, next_color)) {
    return FAIL;
  }

  if (waiting_escape()) {
    return FAIL;
  }

  if (vg_exit()) {
    return FAIL;
  }

  return SUCCESS;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (set_frame_buffer(mode)) {
    return FAIL;
  }

  if (set_graphic_mode(mode)) {
    return FAIL;
  }

  int vrt = mode_info.YResolution / no_rectangles; // vertical
  int hrz = mode_info.XResolution / no_rectangles; // horizontal

  for (int i = 0 ; i < no_rectangles ; i++) {
    for (int j = 0 ; j < no_rectangles ; j++) {

      uint32_t color;

      if (mode_info.MemoryModel == DIRECT_COLOR) {
        uint32_t red = (R(first) + j * step) % (1 << mode_info.RedMaskSize);
        uint32_t green = (G(first) + i * step) % (1 << mode_info.GreenMaskSize);
        uint32_t blue = (B(first) + (i + j) * step) % (1 << mode_info.BlueMaskSize);
        
        color = (red << mode_info.RedFieldPosition) | (green << mode_info.GreenFieldPosition) | (blue << mode_info.BlueFieldPosition);
      } 
      else {
        color = (first + (i * no_rectangles + j) * step) % (1 << mode_info.BitsPerPixel); // Indexed Mode
      }

      if (vg_draw_rectangle(j * hrz, i * vrt, hrz, vrt, color)) {
        return FAIL;
      }
    }
  }
  
  if (waiting_escape()) {
    return FAIL;
  }

  if (vg_exit()) {
    return FAIL;
  }

  return SUCCESS;
}

int (video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  if (set_frame_buffer(VBE_768p_INDEXED)) {
    return FAIL;
  }
  
  if (set_graphic_mode(VBE_768p_INDEXED)) {
    return FAIL;
  }

  if (print_xpm(xpm, x, y)) {
    return FAIL;
  }

  if (waiting_escape()) {
    return FAIL;
  }

  if (vg_exit()) {
    return FAIL;
  }

  return SUCCESS;
}

int (video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  uint8_t timer_bit_no, kbc_bit_no;
  if (KBC_subscribe_ints(&kbc_bit_no)) {
    return FAIL;
  }
  if (timer_subscribe_int(&timer_bit_no)) {
    return FAIL;
  }


  // movement is either vertical or horizontal
  uint8_t vertical_direction;
  if (xi == xf && yi < yf) {
    vertical_direction = 1;
  }  
  else if (yi == yf && xi < xf) {
    vertical_direction = 0;
  }  
  else {
    return FAIL;
  }

  if (timer_set_frequency(0, fr_rate)) {
    return FAIL;
  }   

  if (set_frame_buffer(VBE_768p_INDEXED)) {
    return FAIL;
  }
  
  if (set_graphic_mode(VBE_768p_INDEXED)) {
    return FAIL;
  } 

  if (print_xpm(xpm, xi, yi)) {
    return FAIL;
  }


  int ipc_status, r;
  message msg;

  while (scancode != BREAK_ESC && (xi < xf || yi < yf)) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {   /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */	
          if (msg.m_notify.interrupts & kbc_bit_no) { /* subscribed interrupt */
            kbc_ih(); // update scancode
          }

          if (msg.m_notify.interrupts & timer_bit_no) { /* subscribed interrupt */

            // erase previous drawing (movement feeling)
            if (vg_draw_rectangle(xi, yi, 100, 100, 0xFFFFFF)) {
              return FAIL;
            }

            if (vertical_direction) {
              yi += speed;
              if (yi > yf) {
                yi = yf;
              }  
            } 
            else {
              xi += speed;
              if (xi > xf) {
                xi = xf;
              }  
            }

            // image has moved
            if (print_xpm(xpm, xi, yi)) {
              return FAIL;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    }
  }

  // Back to MINIX text mode
  if (vg_exit()) {
    return FAIL;
  }

  if (timer_unsubscribe_int()) {
    return FAIL;
  }

  if (KBC_unsubscribe_ints()) {
    return FAIL;
  }

  return SUCCESS;
}
