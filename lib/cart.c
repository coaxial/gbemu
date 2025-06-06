/**
 * @file cart.c
 * @brief Gameboy cartridge operations
 * @author Coaxial
 * @date 2025-06-03
 */

#include "cart.h"

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

static const char *OLD_LICENSEE_NAME[] = {
    [0x00] = "None",
    [0x01] = "Nintendo",
    [0x08] = "Capcom",
    [0x09] = "HOT-B",
    [0x0A] = "Jaleco",
    [0x0B] = "Coconuts Japan",
    [0x0C] = "Elite Systems",
    [0x13] = "EA (Electronic Arts)",
    [0x18] = "Hudson Soft",
    [0x19] = "ITC Entertainment",
    [0x1A] = "Yanoman",
    [0x1D] = "Japan Clary",
    [0x1F] = "Virgin Games Ltd.",
    [0x24] = "PCM Complete",
    [0x25] = "San-X",
    [0x28] = "Kemco",
    [0x29] = "SETA Corporation",
    [0x30] = "Infogrames",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Indicates that the New licensee code should be used instead.",
    [0x34] = "Konami",
    [0x35] = "HectorSoft",
    [0x38] = "Capcom",
    [0x39] = "Banpresto",
    [0x3C] = "Entertainment Interactive (stub)",
    [0x3E] = "Gremlin",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu Interactive",
    [0x46] = "Angel",
    [0x47] = "Spectrum HoloByte",
    [0x49] = "Irem",
    [0x4A] = "Virgin Games Ltd.",
    [0x4D] = "Malibu Interactive",
    [0x4F] = "U.S. Gold",
    [0x50] = "Absolute",
    [0x51] = "Acclaim Entertainment",
    [0x52] = "Activision",
    [0x53] = "Sammy USA Corporation",
    [0x54] = "GameTek",
    [0x55] = "Park Place",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x59] = "Milton Bradley Company",
    [0x5A] = "Mindscape",
    [0x5B] = "Romstar",
    [0x5C] = "Naxat Soft",
    [0x5D] = "Tradewest",
    [0x60] = "Titus Interactive",
    [0x61] = "Virgin Games Ltd.",
    [0x67] = "Ocean Software",
    [0x69] = "EA (Electronic Arts)",
    [0x6E] = "Elite Systems",
    [0x6F] = "Electro Brain",
    [0x70] = "Infogrames",
    [0x71] = "Interplay Entertainment",
    [0x72] = "Broderbund",
    [0x73] = "Sculptured Software",
    [0x75] = "The Sales Curve Limited",
    [0x78] = "THQ",
    [0x79] = "Accolade1",
    [0x7A] = "Triffix Entertainment",
    [0x7C] = "MicroProse",
    [0x7F] = "Kemco",
    [0x80] = "Misawa Entertainment",
    [0x83] = "LOZC G.",
    [0x86] = "Tokuma Shoten",
    [0x8B] = "Bullet-Proof Software",
    [0x8C] = "Vic Tokai Corp.",
    [0x8E] = "Ape Inc.",
    [0x8F] = "I’Max",
    [0x91] = "Chunsoft Co.",
    [0x92] = "Video System",
    [0x93] = "Tsubaraya Productions",
    [0x95] = "Varie",
    [0x96] = "Yonezawa19/S’Pal",
    [0x97] = "Kemco",
    [0x99] = "Arc",
    [0x9A] = "Nihon Bussan",
    [0x9B] = "Tecmo",
    [0x9C] = "Imagineer",
    [0x9D] = "Banpresto",
    [0x9F] = "Nova",
    [0xA1] = "Hori Electric",
    [0xA2] = "Bandai",
    [0xA4] = "Konami",
    [0xA6] = "Kawada",
    [0xA7] = "Takara",
    [0xA9] = "Technos Japan",
    [0xAA] = "Broderbund",
    [0xAC] = "Toei Animation",
    [0xAD] = "Toho",
    [0xAF] = "Namco",
    [0xB0] = "Acclaim Entertainment",
    [0xB1] = "ASCII Corporation or Nexsoft",
    [0xB2] = "Bandai",
    [0xB4] = "Square Enix",
    [0xB6] = "HAL Laboratory",
    [0xB7] = "SNK",
    [0xB9] = "Pony Canyon",
    [0xBA] = "Culture Brain",
    [0xBB] = "Sunsoft",
    [0xBD] = "Sony Imagesoft",
    [0xBF] = "Sammy Corporation",
    [0xC0] = "Taito",
    [0xC2] = "Kemco",
    [0xC3] = "Square",
    [0xC4] = "Tokuma Shoten",
    [0xC5] = "Data East",
    [0xC6] = "Tonkin House;",
    [0xC8] = "Koei",
    [0xC9] = "UFL",
    [0xCA] = "Ultra Games",
    [0xCB] = "VAP, Inc.",
    [0xCC] = "Use Corporation",
    [0xCD] = "Meldac",
    [0xCE] = "Pony Canyon",
    [0xCF] = "Angel",
    [0xD0] = "Taito",
    [0xD1] = "SOFEL (Software Engineering Lab)",
    [0xD2] = "Quest",
    [0xD3] = "Sigma Enterprises",
    [0xD4] = "ASK Kodansha Co.",
    [0xD6] = "Naxat Soft",
    [0xD7] = "Copya System",
    [0xD9] = "Banpresto",
    [0xDA] = "Tomy",
    [0xDB] = "LJN",
    [0xDD] = "Nippon Computer Systems",
    [0xDE] = "Human Ent.",
    [0xDF] = "Altron",
    [0xE0] = "Jaleco",
    [0xE1] = "Towa Chiki",
    [0xE2] = "Yutaka # Needs more info",
    [0xE3] = "Varie",
    [0xE5] = "Epoch",
    [0xE7] = "Athena",
    [0xE8] = "Asmik Ace Entertainment",
    [0xE9] = "Natsume",
    [0xEA] = "King Records",
    [0xEB] = "Atlus",
    [0xEC] = "Epic/Sony Records",
    [0xEE] = "IGS",
    [0xF0] = "A Wave",
    [0xF3] = "Extreme Entertainment",
    [0xFF] = "LJN",
};

typedef struct new_licensee_name {
  const char *code;
  const char *name;
} new_licensee_name_t;

static new_licensee_name_t NEW_LICENSEE_NAME[] = {
    {"00", "None"},
    {"01", "Nintendo Research & Development 1"},
    {"08", "Capcom"},
    {"13", "EA (Electronic Arts)"},
    {"18", "Hudson Soft"},
    {"19", "B-AI"},
    {"20", "KSS"},
    {"22", "Planning Office WADA"},
    {"24", "PCM Complete"},
    {"25", "San-X"},
    {"28", "Kemco"},
    {"29", "SETA Corporation"},
    {"30", "Viacom"},
    {"31", "Nintendo"},
    {"32", "Bandai"},
    {"33", "Ocean Software/Acclaim Entertainment"},
    {"34", "Konami"},
    {"35", "HectorSoft"},
    {"37", "Taito"},
    {"38", "Hudson Soft"},
    {"39", "Banpresto"},
    {"41", "Ubi Soft"},
    {"42", "Atlus"},
    {"44", "Malibu Interactive"},
    {"46", "Angel"},
    {"47", "Bullet-Proof Software"},
    {"49", "Irem"},
    {"50", "Absolute"},
    {"51", "Acclaim Entertainment"},
    {"52", "Activision"},
    {"53", "Sammy USA Corporation"},
    {"54", "Konami"},
    {"55", "Hi Tech Expressions"},
    {"56", "LJN"},
    {"57", "Matchbox"},
    {"58", "Mattel"},
    {"59", "Milton Bradley Company"},
    {"60", "Titus Interactive"},
    {"61", "Virgin Games Ltd."},
    {"64", "Lucasfilm Games"},
    {"67", "Ocean Software"},
    {"69", "EA (Electronic Arts)"},
    {"70", "Infogrames"},
    {"71", "Interplay Entertainment"},
    {"72", "Broderbund"},
    {"73", "Sculptured Software"},
    {"75", "The Sales Curve Limited"},
    {"78", "THQ"},
    {"79", "Accolade"},
    {"80", "Misawa Entertainment"},
    {"83", "lozc"},
    {"86", "Tokuma Shoten"},
    {"87", "Tsukuda Original"},
    {"91", "Chunsoft Co."},
    {"92", "Video System"},
    {"93", "Ocean Software/Acclaim Entertainment"},
    {"95", "Varie"},
    {"96", "Yonezawa/S’Pal"},
    {"97", "Kaneko"},
    {"99", "Pack-In-Video"},
    {"9H", "Bottom Up"},
    {"A4", "Konami (Yu-Gi-Oh!)"},
    {"BL", "MTO"},
    {"DK", "Kodansha"},
};

const u8 SEE_NEW_LICENSEE_CODE_FLAG = 0x33;
static cart_t cart_ctx;

/**
 * @brief Load a cartridge from a path
 * @param p_cart_path Path to the cartridge file
 * @return returns a cart_t struct containing a representation of the cartridge
 */
cart_t load_cart(char *p_cart_path) {
  strncpy(cart_ctx.filename, p_cart_path, sizeof(cart_ctx.filename) - 1);

  FILE *rom_file = fopen(cart_ctx.filename, "rb");

  if (rom_file == NULL) {
    printf("Error opening file: %s\n", cart_ctx.filename);
    exit(1);
  }

  fseek(rom_file, 0, SEEK_END);

  cart_ctx.rom_size_bytes = ftell(rom_file);

  rewind(rom_file);

  cart_ctx.p_rom = malloc(cart_ctx.rom_size_bytes);
  fread(cart_ctx.p_rom, cart_ctx.rom_size_bytes, 1, rom_file);
  fclose(rom_file);

  cart_ctx.metadata =
      (cart_metadata_t *)(cart_ctx.p_rom + 0x100); // Metadata starts at 0x100

  if (cart_ctx.metadata->old_licensee_code == SEE_NEW_LICENSEE_CODE_FLAG) {
    /* Pad the title string when using the new license code as that shrinks the
     * title string to 11 chars instead. */
    cart_ctx.metadata->title[11] = '\0';
    cart_ctx.metadata->title[12] = '\0';
    cart_ctx.metadata->title[13] = '\0';
    cart_ctx.metadata->title[14] = '\0';
  };

  /* Terminate the title string in case it isn't for some reason */
  cart_ctx.metadata->title[15] = '\0';

  /* These 16 bits values are big endian in the ROM. Intel CPUs are little
   * endian, which reverses the bytes when loading them into a struct directly.
   * So we put them back in the original order. */
  cart_ctx.metadata->new_licensee_code =
      (cart_ctx.p_rom[0x144] << 8 | cart_ctx.p_rom[0x145]);
  cart_ctx.metadata->global_checksum =
      (cart_ctx.p_rom[0x14e] << 8 | cart_ctx.p_rom[0x14f]);

  return cart_ctx;
};

/**
 * @brief Format cart metadata into a string
 * @param buf Buffer to store the formatted metadata
 * @param buflen Length of the buffer
 * @param metadata Cartridge metadata
 */
void format_cart_metadata(char *p_buf, size_t buflen,
                          cart_metadata_t metadata) {
  const char *p_licensee_name =
      get_licensee_name(metadata.old_licensee_code, metadata.new_licensee_code);
  char p_rom_size_human[32];
  get_human_rom_size(p_rom_size_human, sizeof(p_rom_size_human),
                     metadata.rom_size_code);

  snprintf(p_buf, buflen,
           "Cart title:\t%s (v%d)\nLicensee:\t%s (0x%04X)\nCart type:\t%s "
           "(0x%02X)\nROM size:\t%s (0x%02X)\n",
           metadata.title, metadata.version, p_licensee_name,
           metadata.new_licensee_code, ROM_TYPES_NAMES[metadata.cart_type],
           metadata.cart_type, p_rom_size_human, metadata.rom_size_code);
};

/**
 * @brief Outputs cartridge metadata
 */
void print_cart_metadata() {
  char metadata_buf[1024];
  format_cart_metadata(metadata_buf, sizeof(metadata_buf), *cart_ctx.metadata);

  printf("%s\n", metadata_buf);
};

/**
 * @brief Converts the new licensee code to a full name
 * @param code New licensee code string
 * @return Returns the full licensee name
 */
const char *lookup_new_licensee_name(char *p_code) {
  for (int i = 0; i < sizeof(NEW_LICENSEE_NAME) / sizeof(new_licensee_name_t);
       i++) {
    if (strcmp(p_code, NEW_LICENSEE_NAME[i].code) == 0) {
      return NEW_LICENSEE_NAME[i].name;
    }
  }

  /* TODO: Test this case */
  return "Unknown Licensee";
};

/**
 * @brief Converts the licensee code (old and new) to the full licensee name
 * @param old_lic_code Old licensee code
 * @param new_lic_code New licensee code
 * @return the full licensee name
 */
const char *get_licensee_name(u8 old_lic_code, u16 new_lic_code) {
  if (old_lic_code == SEE_NEW_LICENSEE_CODE_FLAG) {
    /* Turning 16 bit value into a null terminated string as the new license
     * code is a string of 2 bytes */
    char ascii_code[3] = {new_lic_code >> 8, new_lic_code & 0xFF, '\0'};

    return lookup_new_licensee_name(ascii_code);
  }

  return OLD_LICENSEE_NAME[old_lic_code];
};

/**
 * @brief Calculates the human readable ROM size from the ROM size code
 * @param p_buf Buffer to store the human readable ROM size
 * @param buflen Length of the buffer
 * @param rom_size_code The ROM size code from the cart metadata
 */
void get_human_rom_size(char *p_buf, size_t buflen, u8 rom_size_code) {
  /* Not considering the 0x52, 0x53, 0x54 size codes as Pan Docs says there are
   * no known cartridges this size and these codes are likely inaccurate. */
  if (rom_size_code <= 0x08) {
    u16 rom_size_kib = 32 * (1 << rom_size_code);

    if (rom_size_code <= 0x04) {
      snprintf(p_buf, buflen, "%dKiB", rom_size_kib);
    } else {
      snprintf(p_buf, buflen, "%dMiB", rom_size_kib / 1024);
    }
  }
}
