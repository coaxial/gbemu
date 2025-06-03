/**
 * @file cart.c
 * @brief Gameboy cartridge operations
 * @author Coaxial
 * @date 2025-06-03
 */

#include "cart.h"
#include <string.h>

typedef struct cart_context {
  char filename[1024];
  u32 rom_size;
  cart_metadata_t rom_metadata;
  u8 *rom_data;
} cart_context_t;

/* ROM type codes to names, as per
 * https://gbdev.io/pandocs/The_Cartridge_Header.html#0147-rom-type */
static const char *ROM_TYPES_NAMES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static cart_context_t cart_ctx;

/**
 * @brief Load a cartridge from a path
 * @param p_cart_path Path to the cartridge file
 * @return true if the cartridge was loaded successfully, false otherwise
 */
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

/**
 * @brief Format cart metadata into a string
 * @param buf Buffer to store the formatted metadata
 * @param buflen Length of the buffer
 * @param metadata Cartridge metadata
 */
void format_cart_metadata(char *buf, size_t buflen, cart_metadata_t metadata) {
  snprintf(buf, buflen, "Cart type: %s (0x%02X)",
           ROM_TYPES_NAMES[metadata.cart_type], metadata.cart_type);
};

/**
 * @brief Display cartridge metadata
 */
static void display_cart_metadata() {
  char metadata_buf[1024];
  format_cart_metadata(metadata_buf, sizeof(metadata_buf),
                       cart_ctx.rom_metadata);
  printf("%s\n", metadata_buf);
};
