/**
 * @file emu.c
 * @brief Gameboy emulator
 * @author Coaxial
 * @date 2025-06-03
 */

#include "emu.h"
#include "cart.h"
#include "cpu.h"

int emu_run(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: emu <rom_file>\n");
    return -1;
  }

  cart_t cart = load_cart(argv[1]);
  print_cart_metadata();

  cpu_init(NULL);

  return 0;
}
