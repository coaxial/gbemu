#include <check.h>
#include <emu.h>
#include <stdlib.h>

#include "cart.h"

START_TEST(test_cart_metadata) {
  cart_t cart = load_cart("../roms/tests/blargg/cpu_instrs.gb");

  ck_assert_str_eq(cart.metadata->title, "CPU_INSTRS");
  ck_assert_uint_eq(cart.metadata->new_licensee_code, 0x00);
  ck_assert_uint_eq(cart.metadata->sgb_flag, 0x00);
  ck_assert_uint_eq(cart.metadata->cart_type, 0x01);
  ck_assert_uint_eq(cart.metadata->rom_size, 0x01);
  ck_assert_uint_eq(cart.metadata->ram_size, 0x00);
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

Suite *cart_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("cart");

  tc_core = tcase_create("core");
  /* tcase_add_test(tc_core, test_format_cart_metadata); */
  tcase_add_test(tc_core, test_cart_metadata);
  tcase_add_test(tc_core, test_get_licensee_name);
  tcase_add_test(tc_core, test_metadata_title_padding);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = cart_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
