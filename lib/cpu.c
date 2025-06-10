#include "cpu.h"

cpu_ctx_t ctx = {};

void cpu_init(cpu_ctx_t *ctx_p) {
  /* Allows passing a test context in the test suite */
  if (!ctx_p) {
    ctx_p = &ctx;
  }

  /* Instructions start at 0x100 in carts */
  ctx_p->regs.pc = 0x100;
  /* https://gbdev.io/pandocs/Power_Up_Sequence.html#monochrome-models-dmg0-dmg-mgb
   */
  ctx_p->regs.a = 0x01;
}
