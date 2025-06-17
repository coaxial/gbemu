#include "instructions.h"

instruction_t instructions[0x100] = {
    [0x00] = {IN_NOP, AM_IMP},
    [0xC3] = {IN_JP, AM_D16},
};

instruction_t *instruction_by_opcode(u8 opcode) {
  /* Ensure the opcode is valid as some bytes are 0x00 in the ROM which makes
   * an invalid opcode */
  if (instructions[opcode].family == IN_NONE) {
    return NULL;
  }

  return &instructions[opcode];
}
