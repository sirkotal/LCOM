#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"
#include "keyboard.h"
#include "i8042.h"
#include "machine.h"

#define FAIL 1
#define SUCCESS 0

extern uint8_t byte_index;     
extern struct packet mouse_packet; 

extern int timer_int_counter;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
  uint8_t bit_no;
  
  if (mouse_subscribe_int(&bit_no)) {
    return FAIL;
  }

  int ipc_status;
  message msg;
  int r;

  if (mouse_write(ENABLE_DATA_REPORT)) {
    return FAIL;
  }

  while (cnt) {

    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & bit_no) { /* subscribed interrupt */
            mouse_ih();
            mouse_check_bytes();

            if (byte_index == 3) {   
              if (mouse_bytes_into_packet()) {
                return FAIL;
              }                             
              mouse_print_packet(&mouse_packet);    
              byte_index = 0;
              cnt--;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    } 
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_write(DISABLE_DATA_REPORT)) {
    return FAIL;
  }

  return mouse_unsubscribe_int();
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t seconds = 0;
  uint8_t mouse_bit_no; 
  uint8_t timer_bit_no;
  uint16_t timer_frequency = sys_hz();

  if (mouse_subscribe_int(&mouse_bit_no)) {
    return FAIL;
  }

  if (timer_subscribe_int(&timer_bit_no)) {
    return FAIL;
  }

  int ipc_status;
  message msg;
  //int r;

  if (mouse_write(ENABLE_DATA_REPORT)) {
    return FAIL;
  }

  while (seconds < idle_time) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 

          if (msg.m_notify.interrupts & timer_bit_no) { /* subscribed interrupt */
            timer_int_handler();
            if (timer_int_counter % timer_frequency == 0) {
              seconds++;
            }
          }

          if (msg.m_notify.interrupts & mouse_bit_no) { /* subscribed interrupt */
            mouse_ih();
            mouse_check_bytes();

            if (byte_index == 3) {   
              if (mouse_bytes_into_packet()) {
                return FAIL;
              }                             
              mouse_print_packet(&mouse_packet);    
              byte_index = 0;
            }

            timer_int_counter = 0;
            seconds = 0;
          }
      }
    }
  }

  if (mouse_write(DISABLE_DATA_REPORT)) {
    return FAIL;
  }

  if (timer_unsubscribe_int()) {
    return FAIL;
  }

  return mouse_unsubscribe_int();
}


int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t bit_no;
  
  if (mouse_subscribe_int(&bit_no)) {
    return FAIL;
  }

  struct mouse_ev* event;
  uint16_t delta_x;
  uint16_t delta_y;

  state_machina state = START;

  int ipc_status;
  message msg;
  int r;

  if (mouse_write(ENABLE_DATA_REPORT)) {
    return FAIL;
  }

  while (state != END) {

    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & bit_no) { /* subscribed interrupt */
            mouse_ih();
            mouse_check_bytes();

            if (byte_index == 3) {   
              if (mouse_bytes_into_packet()) {
                return FAIL;
              }              
              event = mouse_detect_event(&mouse_packet);
              handle_state(event, &state, x_len, tolerance, &delta_x, &delta_y);               
              mouse_print_packet(&mouse_packet);    
              byte_index = 0;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    } 
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_write(DISABLE_DATA_REPORT)) {
    return FAIL;
  }

  return mouse_unsubscribe_int();
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
