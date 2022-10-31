#include "smart-calc/logic.h"

#include <check.h>

START_TEST(valid_in) {
  char test[255] = "2 + 3 + 5";
  double check = 2 + 3 + 5;
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "( 2 + 3 ) + 5");
  check = (2 + 3) + 5;
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "-45 - -45");
  check = -45 - -45;
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "2 * 3");
  check = 2 * 3;
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "3 / 1");
  check = 3 / 1;
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "2 ^ 2 ^ 3");
  check = pow(2, pow(2, 3));
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "sin ( 2 )");
  check = sin(2);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "cos ( 2 )");
  check = cos(2);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "tan ( 2 )");
  check = tan(2);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "asin ( 1 )");
  check = asin(1);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "acos ( 1 )");
  check = acos(1);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "atan ( 2 )");
  check = atan(2);
  ck_assert_double_eq_tol(calc(test, ""), check, 1e-6);

  memset(test, '\0', 255);
  strcpy(test, "sqrt ( 4 )");
  check = sqrt(4);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "log ( 4 )");
  check = log(4);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "ln ( 4 )");
  check = log10(4);
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "mod ( 4 , 2 )");
  check = 4 % 2;
  ck_assert_double_eq(calc(test, ""), check);

  memset(test, '\0', 255);
  strcpy(test, "X");
  check = 25;
  ck_assert_double_eq(calc(test, "25"), check);
}
END_TEST

START_TEST(non_valid_in) {
  char test[255] = "( ( 5 )";
  ck_assert_int_eq(isnan(calc(test, "")), 1);

  memset(test, '\0', 255);
  strcpy(test, "sin()");
  ck_assert_int_eq(isnan(calc(test, "")), 1);

  memset(test, '\0', 255);
  strcpy(test, "5 + 5 )");
  ck_assert_int_eq(isnan(calc(test, "")), 1);

  memset(test, '\0', 255);
  strcpy(test, "5 5 + 5");
  ck_assert_int_eq(isnan(calc(test, "")), 1);

  memset(test, '\0', 255);
  strcpy(test, "5 + +");
  ck_assert_int_eq(isnan(calc(test, "")), 1);

  memset(test, '\0', 255);
  strcpy(test, "sin ( 5 + 5");
  ck_assert_int_eq(isnan(calc(test, "")), 1);

  memset(test, '\0', 255);
  strcpy(test, " ");
  ck_assert_int_eq(isnan(calc(test, "")), 1);

  memset(test, '\0', 255);
  strcpy(test, "dfsdgsdgsdgsdg");
  ck_assert_int_eq(isnan(calc(test, "")), 1);
}
END_TEST

int main(void) {
  Suite *suite = suite_create("s21_Debuger");
  SRunner *srunner = srunner_create(suite);
  
  TCase *valid_in_f = tcase_create("valid_in");
  suite_add_tcase(suite, valid_in_f);
  tcase_add_test(valid_in_f, valid_in);
  
  TCase *non_valid_in_f = tcase_create("non_valid_in");
  suite_add_tcase(suite, non_valid_in_f);
  tcase_add_test(non_valid_in_f, non_valid_in);

  srunner_run_all(srunner, CK_VERBOSE);
  int number_failed = srunner_ntests_failed(srunner);
  srunner_free(srunner);

  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
