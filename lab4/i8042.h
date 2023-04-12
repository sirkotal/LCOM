#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KEYBOARD    1
#define IRQ_MOUSE       12

/* I/O port addresses */

#define KBD_STAT_REG        0x64 /**< @brief KBC status register */
#define KBD_OUT_BUF         0x60 /**< @brief KBC output buffer */
#define KBD_IN_BUF          0x60 /**< @brief KBC input buffer */
#define KBD_CMD_REG         0x64 /**< @brief KBC command register */

/* Keyboard Status Register */

#define KBD_STAT_PARITY     BIT(7) /**< @brief Parity error */
#define KBD_STAT_TIMEOUT    BIT(6) /**< @brief Timeout error */
#define KBD_STAT_AUX        BIT(5) /**< @brief Mouse data */
#define KBD_STAT_INH        BIT(4) /**< @brief Inhibit flag */
#define KBD_STAT_A2         BIT(3) /**< @brief A2 input line (not needed for LCOM) */
#define KBD_STAT_SYS        BIT(2) /**< @brief System flag (not needed for LCOM) */
#define KBD_STAT_IBF        BIT(1) /**< @brief Input buffer full */
#define KBD_STAT_OBF        BIT(0) /**< @brief Output buffer full */

/* Keyboard Commands */

#define KBD_CMD_ENABLE      0xAE /**< @brief Enable KBC interface */
#define KBD_CMD_DISABLE     0xAD /**< @brief Disable KBC interface */
#define KBD_CMD_CHECK_KBD   0xAB /**< @brief Check keyboard interface */
#define KBD_CMD_CHECK       0xAA /**< @brief Check KBC interface */
#define KBD_CMD_WRITE_CMD   0x60 /**< @brief Write command byte */
#define KBD_CMD_READ_CMD    0x20 /**< @brief Read command byte */

/* Keyboard Command Byte */

#define KBD_CMD_DIS2        BIT(5) /**< @brief Disable mouse interface */
#define KBD_CMD_DIS         BIT(4) /**< @brief Disable keyboard interface */
#define KBD_CMD_EN2         BIT(1) /**< @brief Enable mouse interface */
#define KBD_CMD_INT         BIT(0) /**< @brief Keyboard interrupt */

#define WAIT_KBC        20000
#define MAX_ATTEMPS     10

/* Mouse Control Byte */
#define MOUSE_Y_OVERFLOW      BIT(7)
#define MOUSE_X_OVERFLOW      BIT(6)
#define MOUSE_Y_SIGN          BIT(5)
#define MOUSE_X_SIGN          BIT(4)
#define CONTROL_BIT           BIT(3)
#define MOUSE_MB              BIT(2)
#define MOUSE_RB              BIT(1)
#define MOUSE_LB              BIT(0)

#endif /* _LCOM_I8042_H_ */
