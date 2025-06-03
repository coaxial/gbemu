#include <check.h>
#include <emu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cart.h"

/* START_TEST(test_format_cart_metadata) { */
/*   cart_metadata_t metadata; */
/*   metadata.cart_type = 0x06; */
/*   strncpy(metadata.title, "Test ROM", sizeof(metadata.title)); */
/*   metadata.version = 0x00; */
/*   char buf[1024]; */
/**/
/*   format_cart_metadata(buf, sizeof(buf), metadata); */
/**/
/*   ck_assert_str_eq(buf, */
/*                    "Cart title: Test ROM (v0)\nCart type: MBC2+BATTERY
 * (0x06)"); */
/* }; */

START_TEST(test_cart_metadata) {
  cart_t cart = load_cart("../roms/cpu_instrs.gb");

  ck_assert_str_eq(cart.rom_metadata->title, "CPU_INSTRS");
  ck_assert_uint_eq(cart.rom_metadata->new_licensee_code, 0x00);
  ck_assert_uint_eq(cart.rom_metadata->sgb_flag, 0x00);
  ck_assert_uint_eq(cart.rom_metadata->cart_type, 0x01);
  ck_assert_uint_eq(cart.rom_metadata->rom_size, 0x01);
  ck_assert_uint_eq(cart.rom_metadata->ram_size, 0x00);
  ck_assert_uint_eq(cart.rom_metadata->destination_code, 0x00);
  ck_assert_uint_eq(cart.rom_metadata->old_licensee_code, 0x00);
  ck_assert_uint_eq(cart.rom_metadata->version, 0x00);
  ck_assert_uint_eq(cart.rom_metadata->checksum, 0x3b);
  ck_assert_uint_eq(cart.rom_metadata->global_checksum, 0xf530);
}
END_TEST

Suite *cart_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("cart");

  tc_core = tcase_create("core");
  /* tcase_add_test(tc_core, test_format_cart_metadata); */
  tcase_add_test(tc_core, test_cart_metadata);
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
