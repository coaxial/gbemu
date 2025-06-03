#include "common.h"

typedef struct cart_metadata {
  u32 entry;
  u8 logo[48]; // Or 0x30 bytes: the logo is 3 lines of 16 bytes; or 3 lines
  // of 0x10 bytes (16 decimal into hex) times 3 = 0x30.
  char title[16];
  // TODO:
  // https://gbdev.io/pandocs/The_Cartridge_Header.html#01440145--new-licensee-code
  // to add code to name table
  u16 new_licensee_code;
  u8 sgb_flag;
  u8 cart_type;
  // TODO: Implement number of banks calculation
  u8 rom_size;
  u8 ram_size;
  u8 destination_code;
  // TODO: Translate code to name
  // https://gbdev.io/pandocs/The_Cartridge_Header.html#014b--old-licensee-code
  u8 old_licensee_code;
  u8 version;
  u8 checksum;
  u16 global_checksum;
} cart_metadata_t;

typedef struct cart {
  char filename[1024];
  u32 rom_size;
  cart_metadata_t *rom_metadata;
  u8 *rom_data;
} cart_t;

void format_cart_metadata(char *buf, size_t buflen, cart_metadata_t metadata);
static void display_cart_metadata();
cart_t load_cart(char *p_cart_path);
