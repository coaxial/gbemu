#include "bus.h"
#include "cart.h"

/**
 * Gameboy Memory Map
 * 0x0000 - 0x3FFF: 16 KiB ROM Bank 00                 Read Only
 * 0x4000 - 0x7FFF: 16 KiB ROM Bank 01-7F, switchable Read Only
 * 0x8000 - 0x9FFF: Video RAM (VRAM)                  Read/Write
 * 0xA000 - 0xBFFF: External RAM                      Read/Write
 * 0xC000 - 0xCFFF: Work RAM Bank 0 (WRAM)            Read/Write
 * 0xD000 - 0xDFFF: Work RAM Bank 1 (WRAM)            Read/Write
 * 0xE000 - 0xFDFF: Echo of 8000-9FFF                 Read Only
 * 0xFE00 - 0xFE9F: Sprite Attribute Table (OAM)      Read/Write
 * 0xFEA0 - 0xFEFF: Not Usable
 * 0xFF00 - 0xFF7F: I/O Registers                     Hardware I/O
 * 0xFF80 - 0xFFFE: High RAM (HRAM)                   Read/Write
 * 0xFFFF - 0xFFFF: Interrupt Enable Register
 */

/**
 * @brief Reads a byte from the bus
 * @param addr Address to read from
 * @return The byte at the address or 0xFF if the address is invalid
 */
u8 bus_read(u16 addr) {
  /* FIXME: Not handling bank switching for now */
  if (addr < 0x8000) {
    return cart_read(addr);
  }

  return 0xFF;
};

/**
 * @brief Writes a byte to the bus
 * @param addr Address to write to
 * @param value Value to write
 * @return true if the write was successful
 */
bool bus_write(u16 addr, u8 value) {
  /* TODO: Implement writes */
  return false;
};
