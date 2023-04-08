#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <pp.h>

#define FAIL 1
#define SUCCESS 0

int timer_hook_id = 0;

enum l3223_time_units {
l3223_microsecond,
l3223_millisecond,
l3223_second
};

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

void pp_print_alarm() {
    printf("WAKE UP\n");
}

int (get_LSB)(uint16_t val, uint8_t *lsb) {
    uint16_t mask = 0x00FF;
    *lsb = val & mask;
    return SUCCESS;
}

int (get_MSB)(uint16_t val, uint8_t *msb) {
    uint16_t mask = 0xFF00;
    *msb = (val & mask) >> 8;
    return SUCCESS;
}

int (util_sys_inb)(int port, uint8_t *value) {
  return sys_inb(port, (uint32_t*) value);
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(timer_hook_id);
  
  if (sys_irqsetpolicy(IRQ_LINE, IRQ_REENABLE, &timer_hook_id)) {
    return FAIL;
  }
  return SUCCESS;
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
}

int (timer_config)(int timer, int interval, enum l3223_time_units unit) {
    if (timer > 2) {
        return FAIL;
    }

    uint8_t ctrl_timer;

    if (timer) {
        ctrl_timer = BIT(5+timer);
    }
    else {
        ctrl_timer = SEL_TIMER0;
    }

    uint8_t ctrl_word;
    ctrl_word |= ctrl_timer;
    ctrl_word |= ALARM_MODE;

    switch(unit){
    case l3223_microsecond:
        ctrl_word |= MICROSECOND;
        break;
    case l3223_milisecond:
        ctrl_word |= MILISECOND;
        break;
    case l3223_second:
        ctrl_word |= SECOND;
        break;
    default:
        break;
  }


    if (sys_outb(TIMER_CTRL, ctrl_word)) {
        return FAIL;
    }

    uint8_t mval;
    uint8_t lval;

    get_MSB(interval, &mval);
    get_LSB(interval, &lval);

    if (sys_outb(TIMER_0+timer, lval)) {
        return FAIL;
    }

    if (sys_outb(TIMER_0+timer, mval)) {
        return FAIL;
    }

    return SUCCESS;
}

int (pp_test_alarm)(int timer, int interval, enum l3223_time_units unit) {
    uint8_t bit_no;

    if (timer_subscribe_int(&bit_no)) {
        return FAIL;
    }

    if (timer_config(timer, interval, unit)) {
        return FAIL;
    }

    int ipc_status;
    message msg;
    int r;

    bool done = false;

    while (!done) {

    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & bit_no) { /* subscribed interrupt */
            uint8_t status;
            if (util_sys_inb(TIMER_CTRL, &status)) {
                return FAIL;
            }
            if (status & BIT(timer)) {
            pp_print_alarm();
            done = true;
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

  return timer_unsubscribe_int();
}