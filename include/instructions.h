#pragma once

#include "common.h"

/**
 * Groups instructions by families which can express the whole instruction
 * set when combined with the addressing modes (below).
 */
typedef enum instruction_families {
  IN_NONE,
  IN_NOP,
  IN_LD,
  IN_INC,
  IN_DEC,
  IN_RLCA,
  IN_ADD,
  IN_RRCA,
  IN_STOP,
  IN_RLA,
  IN_JR,
  IN_RRA,
  IN_DAA,
  IN_CPL,
  IN_SCF,
  IN_CCF,
  IN_HALT,
  IN_ADC,
  IN_SUB,
  IN_SBC,
  IN_AND,
  IN_XOR,
  IN_OR,
  IN_CP,
  IN_POP,
  IN_JP,
  IN_PUSH,
  IN_RET,
  IN_CB,
  IN_CALL,
  IN_RETI,
  IN_LDH,
  IN_JPHL,
  IN_DI,
  IN_EI,
  IN_RST,
  IN_ERR,
  /* CB instructions set */
  IN_RLC,
  IN_RRC,
  IN_RL,
  IN_RR,
  IN_SLA,
  IN_SRA,
  IN_SWAP,
  IN_SRL,
  IN_BIT,
  IN_RES,
  IN_SET
} instruction_families_t;

/**
 * Describes the addressing modes for each instruction.
 * IMP   Implicit, no operand
 * R     Register
 * M     Memory
 * MR    Memory to Register
 * HL    HL register
 * HLI   HL register increment
 * HLD   HL register decrement
 * D8    8-bit direct value
 * D16   16-bit direct value (little endian)
 * A8    8-bit address value
 * A16   16-bit addresse value (little endian)
 * SPR   Stack pointer register
 */
typedef enum addressing_modes {
  AM_IMP,
  AM_R_D16,
  AM_R_R,
  AM_MR_R,
  AM_R,
  AM_R_D8,
  AM_R_MR,
  AM_R_HLI,
  AM_R_HLD,
  AM_HLI_R,
  AM_HLD_R,
  AM_R_A8,
  AM_A8_R,
  AM_HL_SPR,
  AM_D16,
  AM_D8,
  AM_D16_R,
  AM_MR_D8,
  AM_MR,
  AM_A16_R,
  AM_R_A16
} addressing_modes_t;

/**
 * Describes the registers that can be used with each instruction.
 * RT_AF   AF register
 * RT_BC   BC register
 * RT_DE   DE register
 * RT_HL   HL register
 * RT_SP   Stack pointer
 * RT_PC   Program counter
 */
typedef enum register_types {
  RT_A,
  RT_B,
  RT_C,
  RT_D,
  RT_E,
  RT_F,
  RT_H,
  RT_L,
  RT_AF,
  RT_BC,
  RT_DE,
  RT_HL,
  RT_SP,
  RT_PC
} register_types_t;

/**
 * Describes the condition types that can be used with conditional
 * instructions.
 * CT_NONE   No condition
 * CT_NZ     Not zero
 * CT_Z      Zero
 * CT_NC     No carry
 * CT_C      Carry
 */
typedef enum condition_types {
  CT_NONE,
  CT_NZ,
  CT_Z,
  CT_NC,
  CT_C
} condition_types_t;

/**
 * Represents an instruction (see https://gbdev.io/gb-opcodes/optables/)
 * Some members are not used for all instructions.
 */
typedef struct instruction {
  instruction_families_t family;
  addressing_modes_t mode;
  register_types_t reg_1;
  register_types_t reg_2;
  condition_types_t cond;
  u8 param;
} instruction_t;
