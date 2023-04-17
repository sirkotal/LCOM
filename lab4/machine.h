#include <lcom/lcf.h>

#include <stdint.h>

typedef enum {
  START,
  DRAW_UP,
  TOP,
  DRAW_DOWN,
  END
} state_machina;

void (handle_state)(struct mouse_ev* event, state_machina* state, uint8_t x_len, uint8_t tol, uint16_t* x_delta, uint16_t* y_delta);
