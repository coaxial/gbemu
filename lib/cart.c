/**
 * @file cart.c
 * @brief Gameboy cartridge operations
 * @author Coaxial
 * @date 2025-06-03
 */

#include "cart.h"
#include <string.h>

/**
 * @brief Load cart/ROM from a file
 * @param cart_path Path to the ROM file
 * @return true if the cartridge was loaded successfully, false otherwise
 */

typedef struct cart_context {
  char filename[1024];
  u32 rom_size;
  cart_metadata_t rom_metadata;
  u8 *rom_data;
} cart_context_t;

static cart_context_t cart_ctx;

bool load_cart(char *p_cart_path) {
  strncpy(cart_ctx.filename, p_cart_path, sizeof(cart_ctx.filename) - 1);

  FILE *rom_file = fopen(cart_ctx.filename, "rb");

  if (rom_file == NULL) {
    printf("Error opening file: %s\n", cart_ctx.filename);
    return false;
  }

  printf("Opened file: %s\n", cart_ctx.filename);

  fseek(rom_file, 0, SEEK_END);

  cart_ctx.rom_size = ftell(rom_file);

  rewind(rom_file);

  cart_ctx.rom_data = malloc(cart_ctx.rom_size);
  if (fread(cart_ctx.rom_data, 1, cart_ctx.rom_size, rom_file) !=
      cart_ctx.rom_size) {
    printf("Error reading file: %s\n", cart_ctx.filename);
    return false;
  }
  fclose(rom_file);

  cart_ctx.rom_metadata = *(cart_metadata_t *)cart_ctx.rom_data;
  // Terminate the title string in case it isn't (like when it's using the
  // old licensee code)
  cart_ctx.rom_metadata.title[15] = '\0';

  display_cart_metadata();

  return true;
};

void format_cart_metadata(char *buf, size_t buflen, cart_metadata_t metadata) {
  snprintf(buf, buflen, "Cart type: 0x%02X", metadata.cart_type);
};

static void display_cart_metadata() {
  char metadata_buf[1024];
  format_cart_metadata(metadata_buf, sizeof(metadata_buf),
                       cart_ctx.rom_metadata);
  printf("%s\n", metadata_buf);
};
