#include "common.h"

typedef struct registers {
  u8 a, b, c, d, e, f, h, l;
} registers_t;

typedef enum { REG_PAIR_AF, REG_PAIR_BC, REG_PAIR_DE, REG_PAIR_HL } reg_pair_t;

const int INVALID_REG_PAIR = false;

/**
 * @brief Combined register accessor
 * @param regs Pointer to registers
 * @param pair Register pair code
 * @param value 16-bit value to set
 * @return true if successful, false otherwise
 */
static inline bool set_reg_pair(registers_t *regs, reg_pair_t pair, u16 value) {
  switch (pair) {
  case REG_PAIR_AF:
    regs->a = (value >> 8) & 0xFF;
    regs->f = value & 0xFF;
    break;
  case REG_PAIR_BC:
    regs->b = (value >> 8) & 0xFF;
    regs->c = value & 0xFF;
    break;
  case REG_PAIR_DE:
    regs->d = (value >> 8) & 0xFF;
    regs->e = value & 0xFF;
    break;
  case REG_PAIR_HL:
    regs->h = (value >> 8) & 0xFF;
    regs->l = value & 0xFF;
    break;
  default:
    return INVALID_REG_PAIR;
  }
  return true;
}

/**
 * @brief Combined register setter
 * @param regs Pointer to registers
 * @param pair Register pair code
 * @return 16-bit value of register pair or false if invalid
 */
static inline u16 get_reg_pair(const registers_t *regs, reg_pair_t pair) {
  switch (pair) {
  case REG_PAIR_AF:
    return ((u16)regs->a << 8) | regs->f;
  case REG_PAIR_BC:
    return ((u16)regs->b << 8) | regs->c;
  case REG_PAIR_DE:
    return ((u16)regs->d << 8) | regs->e;
  case REG_PAIR_HL:
    return ((u16)regs->h << 8) | regs->l;
  default:
    return INVALID_REG_PAIR;
  }
}
