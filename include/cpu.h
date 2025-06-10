#include "common.h"

typedef struct registers {
  u8 a, b, c, d, e, f, h, l;
} registers_t;

typedef enum { REG_PAIR_AF, REG_PAIR_BC, REG_PAIR_DE, REG_PAIR_HL } reg_pair_t;

const bool INVALID_REG_PAIR = false;
const bool INVALID_FLAG_CODE = false;

typedef enum { FLAG_ZERO, FLAG_SUBTRACT, FLAG_HALF_CARRY, FLAG_CARRY } flag_t;

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

/**
 * @brief Get a specific flag
 * @param p_regs Pointer to registers
 * @param flag Flag code
 * @return true if flag is set, false otherwise, and false if invalid flag.
 */
static inline bool get_flag(const registers_t *p_regs, flag_t flag) {
  switch (flag) {
  case FLAG_ZERO:
  case FLAG_SUBTRACT:
  case FLAG_HALF_CARRY:
  case FLAG_CARRY:
    return (p_regs->f & (1U << (7 - flag)));
  default:
    return INVALID_FLAG_CODE;
  }
};

/**
 * @brief Set a specific flag
 * @param p_regs Pointer to registers
 * @param flag Flag code
 * @param value Value to set (one bit)
 * @return true if successful, false otherwise
 */
static inline bool set_flag(registers_t *p_regs, flag_t flag, bool value) {
  switch (flag) {
  case FLAG_ZERO:
  case FLAG_SUBTRACT:
  case FLAG_HALF_CARRY:
  case FLAG_CARRY:
    /**
     * 1U << (7 - flag) is creating a mask which is then inverted with ~ so that
     * we only select the bit to change. This is then ORed with `true`
     * shifted left by the same amount to set the bit value.
     * This is so that only the bit corresponding to the flag is
     * changed, without affecting the other bits.
     */
    p_regs->f = (p_regs->f & ~(1U << (7 - flag))) | (value << (7 - flag));
    return true;
  default:
    return false;
  }
};
