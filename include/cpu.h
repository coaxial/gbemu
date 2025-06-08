#include "common.h"

typedef struct registers {
  u8 a, b, c, d, e, f, h, l;
} registers_t;

typedef enum { REG_PAIR_AF, REG_PAIR_BC, REG_PAIR_DE, REG_PAIR_HL } reg_pair_t;

const bool INVALID_REG_PAIR = false;

/**
 * @brief Combined register setter
 * @param p_regs Pointer to registers
 * @param pair Register pair code
 * @return 16-bit value of register pair or false if invalid
 */
static inline bool set_reg_pair(registers_t *p_regs, reg_pair_t pair,
                                u16 value) {
  switch (pair) {
  case REG_PAIR_AF:
    p_regs->a = (value >> 8) & 0xFF;
    p_regs->f = value & 0xFF;
    break;
  case REG_PAIR_BC:
    p_regs->b = (value >> 8) & 0xFF;
    p_regs->c = value & 0xFF;
    break;
  case REG_PAIR_DE:
    p_regs->d = (value >> 8) & 0xFF;
    p_regs->e = value & 0xFF;
    break;
  case REG_PAIR_HL:
    p_regs->h = (value >> 8) & 0xFF;
    p_regs->l = value & 0xFF;
    break;
  default:
    return INVALID_REG_PAIR;
  }
  return true;
}

/**
 * @brief Combined register getter
 * @param p_regs Pointer to registers
 * @param pair Register pair code
 * @param value 16-bit value to set
 * @return true if successful, false otherwise
 */
static inline u16 get_reg_pair(const registers_t *p_regs, reg_pair_t pair) {
  switch (pair) {
  case REG_PAIR_AF:
    return ((u16)p_regs->a << 8) | p_regs->f;
  case REG_PAIR_BC:
    return ((u16)p_regs->b << 8) | p_regs->c;
  case REG_PAIR_DE:
    return ((u16)p_regs->d << 8) | p_regs->e;
  case REG_PAIR_HL:
    return ((u16)p_regs->h << 8) | p_regs->l;
  default:
    return INVALID_REG_PAIR;
  }
}
