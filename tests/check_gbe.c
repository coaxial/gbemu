#include <check.h>
#include <emu.h>
#include <stdlib.h>

#include "cart.h"
#include "cpu.h"

/**
 * Cart Test Suite
 */
START_TEST(test_cart_metadata) {
  cart_t cart = load_cart("../roms/tests/blargg/cpu_instrs.gb");

  ck_assert_str_eq(cart.metadata->title, "CPU_INSTRS");
  ck_assert_uint_eq(cart.metadata->new_licensee_code, 0x00);
  ck_assert_uint_eq(cart.metadata->sgb_flag, 0x00);
  ck_assert_uint_eq(cart.metadata->cart_type, 0x01);
  ck_assert_uint_eq(cart.metadata->rom_size_code, 0x01);
  ck_assert_uint_eq(cart.metadata->ram_size_code, 0x00);
  ck_assert_uint_eq(cart.metadata->destination_code, 0x00);
  ck_assert_uint_eq(cart.metadata->old_licensee_code, 0x00);
  ck_assert_uint_eq(cart.metadata->version, 0x00);
  ck_assert_uint_eq(cart.metadata->checksum, 0x3b);
  ck_assert_uint_eq(cart.metadata->global_checksum, 0xf530);
}
END_TEST

START_TEST(test_get_licensee_name) {
  u8 OLD_LIC_CODES[] = {0x00, 0x01, 0x69, 0xB9, 0xFF};
  char *OLD_LIC_NAMES[] = {"None", "Nintendo", "EA (Electronic Arts)",
                           "Pony Canyon", "LJN"};
  char *NEW_LIC_CODES[] = {"00", "01", "78", "DK"};
  char *NEW_LIC_NAMES[] = {"None", "Nintendo Research & Development 1", "THQ",
                           "Kodansha"};

  for (int i = 0; i < (sizeof(OLD_LIC_CODES) / sizeof(u8)); i++) {
    char *expected = OLD_LIC_NAMES[i];

    const char *actual = get_licensee_name(OLD_LIC_CODES[i], 0xBABE);

    ck_assert_str_eq(actual, expected);
  }

  for (int i = 0; i < (sizeof(NEW_LIC_CODES) / sizeof(char *)); i++) {
    u16 raw_code = ((NEW_LIC_CODES[i][0] << 8) | NEW_LIC_CODES[i][1]);
    char *expected = NEW_LIC_NAMES[i];

    const char *actual = get_licensee_name(0x33, raw_code);

    ck_assert_str_eq(actual, expected);
  }
}
END_TEST

START_TEST(test_metadata_title_padding) {
  cart_t cart = load_cart("../roms/tests/new_lic_code.gb");

  ck_assert_str_eq(cart.metadata->title, "COFFEEBREAK");
}
END_TEST

START_TEST(test_get_rom_size) {
  u8 ROM_SIZE_CODES[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  char *P_EXPECTED_ROM_SIZES[] = {"32KiB",  "64KiB",  "128KiB",
                                  "256KiB", "512KiB", "1MiB",
                                  "2MiB",   "4MiB",   "8MiB"};

  for (size_t i = 0; i < (sizeof(ROM_SIZE_CODES) / sizeof(ROM_SIZE_CODES[0]));
       i++) {
    char *p_expected = P_EXPECTED_ROM_SIZES[i];

    char p_actual[32];
    get_human_rom_size(p_actual, sizeof(p_actual), ROM_SIZE_CODES[i]);

    ck_assert_str_eq(p_actual, p_expected);
  }
}
END_TEST

START_TEST(test_get_ram_size) {
  u8 RAM_SIZE_CODES[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  int EXPECTED_RAM_SIZES[] = {0, -1, 8, 32, 128, 64};

  for (size_t i = 0; i < (sizeof(RAM_SIZE_CODES) / sizeof(RAM_SIZE_CODES[0]));
       i++) {
    int expected = EXPECTED_RAM_SIZES[i];

    int actual = get_ram_size_kib(RAM_SIZE_CODES[i]);

    ck_assert_int_eq(actual, expected);
  }
}
END_TEST

Suite *cart_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("cart");

  tc_core = tcase_create("core");
  tcase_add_test(tc_core, test_cart_metadata);
  tcase_add_test(tc_core, test_get_licensee_name);
  tcase_add_test(tc_core, test_metadata_title_padding);
  tcase_add_test(tc_core, test_get_rom_size);
  tcase_add_test(tc_core, test_get_ram_size);
  suite_add_tcase(s, tc_core);

  return s;
}

/**
 * CPU Test Suite
 */
START_TEST(test_set_get_reg_pair) {
  registers_t regs = {
      .a = 0xBA,
      .b = 0xBE,
      .c = 0xBA,
      .d = 0xBE,
      .e = 0xBA,
      .f = 0xBE,
      .h = 0xBA,
      .l = 0xBE,
  };
  reg_pair_t reg_pairs[] = {
      REG_PAIR_AF,
      REG_PAIR_BC,
      REG_PAIR_DE,
      REG_PAIR_HL,
  };
  u16 expected = 0xCAFE;

  for (int i = 0; i < sizeof(reg_pairs) / sizeof(reg_pair_t); i++) {
    bool success = set_reg_pair(&regs, reg_pairs[i], expected);
    u16 actual = get_reg_pair(&regs, reg_pairs[i]);

    ck_assert(success);
    ck_assert_uint_eq(actual, expected);
  };
}
END_TEST

START_TEST(test_set_reg_pair_invalid) {
  registers_t regs;

  bool expected = false;
  bool actual = set_reg_pair(&regs, 0x05, 0xCAFE);

  ck_assert(expected == actual);
}
END_TEST

START_TEST(test_get_reg_pair_invalid) {
  registers_t regs;

  int invalid_pair = 42;
  bool expected = INVALID_REG_PAIR;
  bool actual = get_reg_pair(&regs, invalid_pair);

  ck_assert(expected == actual);
}
END_TEST

Suite *cpu_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("cpu");

  tc_core = tcase_create("core");
  tcase_add_test(tc_core, test_set_get_reg_pair);
  tcase_add_test(tc_core, test_set_reg_pair_invalid);
  tcase_add_test(tc_core, test_get_reg_pair_invalid);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  Suite *s1;
  SRunner *sr;

  s = cart_suite();
  s1 = cpu_suite();
  sr = srunner_create(s);
  srunner_add_suite(sr, s1);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
