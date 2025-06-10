#include "cpu.h"

cpu_ctx_t ctx = {};

void cpu_init(cpu_ctx_t *p_ctx) {
  /* Allows passing a test context in the test suite */
  if (!p_ctx) {
    p_ctx = &ctx;
  }

  /* Instructions start at 0x100 in carts */
  p_ctx->regs.pc = 0x100;
  /* https://gbdev.io/pandocs/Power_Up_Sequence.html#monochrome-models-dmg0-dmg-mgb
   */
  p_ctx->regs.a = 0x01;
}
