#include "machine.h"

void (handle_state)(struct mouse_ev* event, state_machina* state, uint8_t x_len, uint8_t tol, uint16_t* x_delta, uint16_t* y_delta) {
  switch (*state) {
    case START:
      *x_delta = 0;
      *y_delta = 0;

      if (event->type == LB_PRESSED) {
        *state = DRAW_UP;
      }
      break;
    case DRAW_UP:
      if (event->type == LB_RELEASED) {
        if (*x_delta >= x_len) {
          *state = TOP;
          *x_delta = 0;
          *y_delta = 0;
        } 
        else {
          *state = START;
        }
      } 
      else if (event->type == MOUSE_MOV) {
        if (abs(event->delta_x) <= tol && abs(event->delta_y) <= tol) {
          *x_delta += event->delta_x;
          *y_delta += event->delta_y;
        } 
        else {
          *state = START;
        }
      } 
      else {
        *state = START;
      }
      break;
    case TOP:
      if (event->type == RB_PRESSED) {
        *state = DRAW_DOWN;
      } 
      else if (event->type == LB_PRESSED) {
        *state = DRAW_UP;
      } 
      else {
        *state = START;
      }
      break;
    case DRAW_DOWN:
      if (event->type == RB_RELEASED) {
        if (*x_delta >= x_len) {
          *state = END;
          *x_delta = 0;
          *y_delta = 0;
        } 
        else {
          *state = START;
        }
      } 
      else if (event->type == MOUSE_MOV) {
        if (abs(event->delta_x) <= tol && abs(event->delta_y) <= tol) {
          *x_delta += event->delta_x;
          *y_delta += event->delta_y;
        } 
        else {
          *state = START;
        }
      } 
      else {
        *state = START;
      }
      break;
    case END:
      break;
  }
}
