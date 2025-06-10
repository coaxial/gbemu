#pragma once

#include "common.h"

typedef struct cart_metadata {
  u32 entry;
  // 48 or 0x30 bytes: the logo is 3 lines of 16 bytes; or 3 lines of 0x10
  // bytes(16 decimal into hex) times 3 = 0x30.
  u8 logo[48];
  char title[16];
  u16 new_licensee_code;
  u8 sgb_flag;
  u8 cart_type;
  // TODO : Implement number of banks calculation
  u8 rom_size_code;
  u8 ram_size_code;
  u8 destination_code;
  u8 old_licensee_code;
  u8 version;
  u8 checksum;
  u16 global_checksum;
} cart_metadata_t;

typedef struct cart {
  char filename[1024];
  u32 rom_size_bytes;
  cart_metadata_t *metadata;
  u8 *p_rom;
} cart_t;

void format_cart_metadata(char *p_buf, size_t buflen, cart_metadata_t metadata);
void print_cart_metadata();
cart_t load_cart(char *p_cart_path);
const char *lookup_new_licensee_name(char *p_code);
const char *get_licensee_name(u8 old_lic_code, u16 new_lic_code);
void get_human_rom_size(char *p_buf, size_t buflen, u8 rom_size_code);
int get_ram_size_kib(u8 ram_size_code);
