#include "test_runner.h"

int main()
{
    SRunner *sr = srunner_create(NULL);
    srunner_add_suite(sr, s21_add_suite);
    srunner_run_all(sr, CK_NORMAL);

    int failed = srunner_ntests_failed(sr);

    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}