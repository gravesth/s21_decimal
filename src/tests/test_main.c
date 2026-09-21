#include "test_main.h"

int main(void) {
    int failed_count = 0;
    Suite *s = suite_comparison();
    SRunner *sr = srunner_create(s);
    srunner_add_suite(sr, suite_convertator());

    srunner_run_all(sr, CK_NORMAL);
    failed_count = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed_count == 0) ? 0 : 1;
}
