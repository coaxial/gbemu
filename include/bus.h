#pragma once

#include "common.h"

u8 bus_read(u16 addr);
void bus_write(u16 addr, u8 value);

u16 bus_read_word(u16 addr);
void bus_write_word(u16 addr, u16 value);
