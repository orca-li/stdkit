#include <stdkit.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char test_message[] = "Hello, iterator!\r\n";

void* cbputchar(void* const itr, ...)
{
    putchar(*itrread((itrobj_t)itr));
    return NULL;
}

int hello_iterator(void)
{
    itrobj_t itr = itrnew(0);
    itrinit(itr, KIT_SIZEOF(*test_message), (int8_t*)test_message);
    itr->step = cbputchar;
    itrrun(itr, strlen(test_message));
    itrfree(itr);

    return 0;
}

static void test_hello_iterator(void** state)
{
    KIT_UNUSED(state);

    int result = hello_iterator();
    assert_int_equal(result, 0);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hello_iterator),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}