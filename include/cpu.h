#pragma once

#include "common.h"
#include "instructions.h"

typedef struct registers {
  u8 a, b, c, d, e, f, h, l;
  u16 pc, sp;
} registers_t;

typedef enum { FLAG_ZERO, FLAG_SUBTRACT, FLAG_HALF_CARRY, FLAG_CARRY } flag_t;

typedef struct ctx {
  registers_t regs;

  u16 fetched_data;
  u16 mem_dest;
  u8 current_opcode;
  instruction_t current_instruction;

  bool halted;
  bool stepping;
} cpu_ctx_t;

/**
 * @brief Combined register getter
 * @param regs_p Pointer to registers
 * @param pair Register pair code
 * @param value 16-bit value to set
 * @return returns register value or 0xFFFF if invalid
 */
static inline bool set_reg(registers_t *regs_p, register_types_t reg,
                           u8 value) {
  switch (reg) {
  case RT_A:
    regs_p->a = value;
    break;
  case RT_B:
    regs_p->b = value;
    break;
  case RT_C:
    regs_p->c = value;
    break;
  case RT_D:
    regs_p->d = value;
    break;
  case RT_E:
    regs_p->e = value;
    break;
  case RT_F:
    regs_p->f = value;
    break;
  case RT_H:
    regs_p->h = value;
    break;
  case RT_L:
    regs_p->l = value;
    break;
  case RT_AF:
  case RT_BC:
  case RT_DE:
  case RT_HL:
  case RT_SP:
  case RT_PC:
  default:
    return false;
  }
  return true;
}

/**
 * @brief Combined register setter
 * @param regs_p Pointer to registers
 * @param pair Register pair code
 * @return 16-bit value of register pair or false if invalid
 */
static inline bool set_reg_pair(registers_t *regs_p, register_types_t reg,
                                u16 value) {
  switch (reg) {
  case RT_AF:
    regs_p->a = (value >> 8) & 0xFF;
    regs_p->f = value & 0xFF;
    break;
  case RT_BC:
    regs_p->b = (value >> 8) & 0xFF;
    regs_p->c = value & 0xFF;
    break;
  case RT_DE:
    regs_p->d = (value >> 8) & 0xFF;
    regs_p->e = value & 0xFF;
    break;
  case RT_HL:
    regs_p->h = (value >> 8) & 0xFF;
    regs_p->l = value & 0xFF;
    break;
  default:
    return false;
  }
  return true;
}

/**
 * @brief Single register getter
 * @param regs_p Pointer to registers
 * @param reg Register type code
 * @return 8-bit value of register or 0xFF if invalid
 */
static inline u8 get_reg(registers_t *regs_p, register_types_t reg) {
  switch (reg) {
  case RT_A:
    return regs_p->a;
  case RT_B:
    return regs_p->b;
  case RT_C:
    return regs_p->c;
  case RT_D:
    return regs_p->d;
  case RT_E:
    return regs_p->e;
  case RT_F:
    return regs_p->f;
  case RT_H:
    return regs_p->h;
  case RT_L:
    return regs_p->l;
  case RT_AF:
  case RT_BC:
  case RT_DE:
  case RT_HL:
  case RT_SP:
  case RT_PC:
  default:
    return 0xFF;
  }
}

/**
 * @brief Combined register getter
 * @param regs_p Pointer to registers
 * @param pair Register pair code
 * @param value 16-bit value to set
 * @return returns register value or 0xFFFF if invalid
 */
static inline u16 get_reg_pair(const registers_t *regs_p,
                               register_types_t reg) {
  switch (reg) {
  case RT_AF:
    return ((u16)regs_p->a << 8) | regs_p->f;
  case RT_BC:
    return ((u16)regs_p->b << 8) | regs_p->c;
  case RT_DE:
    return ((u16)regs_p->d << 8) | regs_p->e;
  case RT_HL:
    return ((u16)regs_p->h << 8) | regs_p->l;
  default:
    return 0xFFFF;
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
    return false;
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
void cpu_step();
void fetch_instruction();
void fetch_data();
void execute();
