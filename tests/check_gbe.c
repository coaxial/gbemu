#include <check.h>
#include <emu.h>
#include <stdlib.h>

#include "cart.h"

START_TEST(test_format_cart_metadata) {
  cart_metadata_t metadata;

  metadata.cart_type = 0x77;
  char buf[1024];
  format_cart_metadata(buf, sizeof(buf), metadata);
  ck_assert_str_eq(buf, "Cart type: 0x77");
};

Suite *cart_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("cart");

  tc_core = tcase_create("core");
  tcase_add_test(tc_core, test_format_cart_metadata);
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
