#include "cpu.h"
#include "bus.h"
#include "emu.h"

cpu_ctx_t ctx = {};

void fetch_instruction() {
  ctx.current_opcode = bus_read(ctx.regs.pc++);
  ctx.current_instruction = *instruction_by_opcode(ctx.current_opcode);
}

void fetch_data() {
  switch (ctx.current_instruction.mode) {
  case AM_IMP:
    return;
  case AM_D8:
    ctx.fetched_data = bus_read(ctx.regs.pc);
    emu_cycles(1);
    return;

  case AM_D16:
    /* Fetching a 16-bit value, 8-bits at a time */
    ctx.fetched_data = bus_read(ctx.regs.pc++);
    emu_cycles(1);
    ctx.fetched_data |= bus_read(ctx.regs.pc++) << 8;
    emu_cycles(1);
    return;
  case AM_R:
    ctx.fetched_data = get_reg(&ctx.regs, ctx.current_instruction.reg_1);
    return;
  case AM_R_D16:
  case AM_R_R:
  case AM_MR_R:
  case AM_R_D8:
  case AM_R_MR:
  case AM_R_HLI:
  case AM_R_HLD:
  case AM_HLI_R:
  case AM_HLD_R:
  case AM_R_A8:
  case AM_A8_R:
  case AM_HL_SPR:
  case AM_D16_R:
  case AM_MR_D8:
  case AM_MR:
  case AM_A16_R:
  case AM_R_A16:
    return;
  }
};
void execute() { printf("Not implemented yet\n"); };

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

void cpu_step() {
  if (!ctx.halted) {
    fetch_instruction();
    fetch_data();
    execute();
  }
}
