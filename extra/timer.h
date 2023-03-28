#ifndef _LCOM_ALARM_H_
#define _LCOM_ALARM_H_

#include <stdint.h>

#define TIMER_0 0x20
#define TIMER_1 0x21
#define TIMER_1 0x22

#define TIMER_CTRL 0x23

#define IRQ_LINE 10 // in i8254, Timer 0 would be 0 (this is an "imaginary" device though)

#define BIT(n) (1 << n)

// Writing to port 0x23 writes to the Control Register (0x43 in i8254)
// Timer selection - bits 7 e 6
#define SEL_TIMER0 0x00
#define SEL_TIMER1 BIT(6)
#define SEL_TIMER2 BIT(7)
#define TIMER_RB_CMD (BIT(7) | BIT(6))

// Alarm - bit 4
#define PERIODIC_MODE 0x00
#define ALARM_MODE BIT(4)

// Time Units - bits 1 e 0
#define MICROSECOND 0x00
#define MILISECOND BIT(0)
#define SECOND BIT(1)

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

int (get_LSB)(uint16_t val, uint8_t *lsb);
int (get_MSB)(uint16_t val, uint8_t *msb);
int (util_sys_inb)(int port, uint8_t *value);

int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int);
void (timer_int_handler);

int (timer_config)(int timer, int interval, enum l3223_time_units unit);

int (pp_test_alarm)(int timer, int interval, enum l3223_time_units unit);

void (pp_print_alarm)();

#endif /* _LCOM_ALARM_H */