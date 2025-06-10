#pragma once

#include "common.h"

typedef struct registers {
  u8 a, b, c, d, e, f, h, l;
  u16 pc, sp;
} registers_t;

typedef enum { REG_PAIR_AF, REG_PAIR_BC, REG_PAIR_DE, REG_PAIR_HL } reg_pair_t;

static const bool INVALID_REG_PAIR = false;
static const bool INVALID_FLAG_CODE = false;

typedef enum { FLAG_ZERO, FLAG_SUBTRACT, FLAG_HALF_CARRY, FLAG_CARRY } flag_t;

typedef struct ctx {
  registers_t regs;
} cpu_ctx_t;

/**
 * @brief Combined register setter
 * @param regs_p Pointer to registers
 * @param pair Register pair code
 * @return 16-bit value of register pair or false if invalid
 */
static inline bool set_reg_pair(registers_t *regs_p, reg_pair_t pair,
                                u16 value) {
  switch (pair) {
  case REG_PAIR_AF:
    regs_p->a = (value >> 8) & 0xFF;
    regs_p->f = value & 0xFF;
    break;
  case REG_PAIR_BC:
    regs_p->b = (value >> 8) & 0xFF;
    regs_p->c = value & 0xFF;
    break;
  case REG_PAIR_DE:
    regs_p->d = (value >> 8) & 0xFF;
    regs_p->e = value & 0xFF;
    break;
  case REG_PAIR_HL:
    regs_p->h = (value >> 8) & 0xFF;
    regs_p->l = value & 0xFF;
    break;
  default:
    return INVALID_REG_PAIR;
  }
  return true;
}

/**
 * @brief Combined register getter
 * @param regs_p Pointer to registers
 * @param pair Register pair code
 * @param value 16-bit value to set
 * @return true if successful, false otherwise
 */
static inline u16 get_reg_pair(const registers_t *regs_p, reg_pair_t pair) {
  switch (pair) {
  case REG_PAIR_AF:
    return ((u16)regs_p->a << 8) | regs_p->f;
  case REG_PAIR_BC:
    return ((u16)regs_p->b << 8) | regs_p->c;
  case REG_PAIR_DE:
    return ((u16)regs_p->d << 8) | regs_p->e;
  case REG_PAIR_HL:
    return ((u16)regs_p->h << 8) | regs_p->l;
  default:
    return INVALID_REG_PAIR;
  }
}

/**
 * @brief Get a specific flag
 * @param regs_p Pointer to registers
 * @param flag Flag code
 * @return true if flag is set, false otherwise, and false if invalid flag.
 */
static inline bool get_flag(const registers_t *regs_p, flag_t flag) {
  switch (flag) {
  case FLAG_ZERO:
  case FLAG_SUBTRACT:
  case FLAG_HALF_CARRY:
  case FLAG_CARRY:
    return (regs_p->f & (1U << (7 - flag)));
  default:
    return INVALID_FLAG_CODE;
  }
};

/**
 * @brief Set a specific flag
 * @param regs_p Pointer to registers
 * @param flag Flag code
 * @param value Value to set (one bit)
 * @return true if successful, false otherwise
 */
static inline bool set_flag(registers_t *regs_p, flag_t flag, bool value) {
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
    regs_p->f = (regs_p->f & ~(1U << (7 - flag))) | (value << (7 - flag));
    return true;
  default:
    return false;
  }
};

void cpu_init(cpu_ctx_t *ctx_p);
