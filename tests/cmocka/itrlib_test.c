#include <stdkit.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define STR_TEST_MESSAGE "Hello, iterator!\r\n"

void *cbfinish_endline(void *const itr, ...)
{
    KIT_UNUSED(itr);

    printf("\r\n");
    return KIT_NULL;
}

void *cbstep_putchar(void *const itr, ...)
{
    putchar(*itrread(KIT_TO(itrobj_t, itr)));
    return KIT_NULL;
}

void *cbstep_dump(void *const itr, ...)
{
    printf("%02hhx", *itrread((itrobj_t)itr));
    return KIT_NULL;
}

void *cbstep_write(void *const itr, ...)
{
    static int counter = 0;
    itrwrite(KIT_TO(itrobj_t, itr), &counter, sizeof(int8_t));
    counter++;
    return KIT_NULL;
}

void *cbreset_set_cbstep_write(void *const itr, ...)
{
    KIT_TO(itrobj_t, itr)->step = cbstep_write;
    return KIT_NULL;
}

void *cbreset_set_cbstep_dump(void *const itr, ...)
{
    KIT_TO(itrobj_t, itr)->step = cbstep_dump;
    return KIT_NULL;
}

int hello_iterator(char *test_message)
{
    itrobj_t itr = KIT_NULL;
    itr = itrnew(ITR_F_NONE);
    if (!itr)
        KIT_ASSERT(KIT_FALSE);
    itrinit(itr, KIT_SIZEOF(int8_t), KIT_TO(itrptr_t, test_message));
    itr->step = cbstep_putchar;
    itrrun(itr, strlen(test_message));
    itrfree(&itr);

    return 0;
}

void dbg_dump(itrptr_t addr, size_t range)
{
    itrobj_t itr = KIT_NULL;

    itr = itrnew(ITR_F_NONE);
    itrinit(itr, sizeof(char), addr);

    itr->reset = cbreset_set_cbstep_dump;
    itr->step = cbstep_dump;
    itrrst(itr);
    itr->finish = cbfinish_endline;
    itrrun(itr, range);

    itrfree(&itr);
}

int8_t test_array[18];

int enumeration_iterator(void)
{
    itrobj_t itr = KIT_NULL;
    itr = itrnew(ITR_F_NONE);
    if (!itr)
        KIT_ASSERT(KIT_FALSE);
    itrinit(itr, KIT_SIZEOFELM(test_array), KIT_TO(itrptr_t, test_array));
    itr->reset = cbreset_set_cbstep_write;
    itrrst(itr);
    KIT_FOR(KIT_ARRAY_SIZE(test_array))
        itrpp(itr);

    dbg_dump(test_array, KIT_ARRAY_SIZE(test_array));

    KIT_FOR_EACH(test_array)
        assert_int_equal(KIT_ELEMENT(test_array), i);

    itrfree(&itr);

    return 0;
}

int index_iterator(void)
{
    itrobj_t itr = KIT_NULL;

    itr = itrnew(ITR_F_NONE);
    if (!itr)
        KIT_ASSERT(KIT_FALSE);
    itrinit(itr, KIT_SIZEOFELM(test_array), KIT_TO(itrptr_t, test_array));

    KIT_FOR_EACH(test_array)
    {
        printf("%02zx", i);
        itridx(itr, i);
        assert_int_equal(KIT_ELEMENT(test_array), *itrread(itr));
    }
    cbfinish_endline(NULL);

    intmax_t reverse_index = 0;
    KIT_FOR_EACH(test_array)
    {
        reverse_index = KIT_LAST_INDEX(test_array) - i;
        printf("%02zx", reverse_index);
        itridx(itr, reverse_index);
        assert_int_equal(test_array[reverse_index], *itrread(itr));
    }
    cbfinish_endline(NULL);

    itrfree(&itr);

    return 0;
}

itrobj_t itr_template = KIT_NULL;

int copy_iterator(void)
{
    itrobj_t itr = KIT_NULL;

    itr = itrnew(ITR_F_NONE);
    if (!itr)
        KIT_ASSERT(KIT_FALSE);
    itrinit(itr, KIT_SIZEOFELM(test_array), KIT_TO(itrptr_t, test_array));
    itr->step = cbstep_dump;
    itr->finish = cbfinish_endline;
    itrrun(itr, KIT_SIZEOF(test_array));
    itrrst(itr);

    itrcpy(&itr_template, itr);
    itrfree(&itr);

    return 0;
}

int check_duplicate_iterator(void)
{
    itrobj_t itr = KIT_NULL;

    itrcpy(&itr, itr_template);
    itrrun(itr, KIT_SIZEOF(test_array));
    itrfree(&itr);

    itrcpy(&itr, itr_template);
    itrrun(itr, KIT_SIZEOF(test_array));
    itrfree(&itr);

    return 0;
}

#define ASSERT_VALUE_V8 8
#define ASSERT_VALUE_V16U 16
#define ASSERT_VALUE_V32 32
#define ASSERT_VALUE_V64U 64
#define ASSERT_VALUE_ARRAY16 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
#define ASSERT_VALUE_MESSAGE "test_struct_message"

#define STRUCT_DATA_ALIGN 1

struct TEST_STRUCT {
    int8_t v8;
    uint16_t v16u;
    int32_t v32;
    uint64_t v64u;
    int8_t test_array_16[16];
    char test_message[32];
} test_struct = {
    .v8 = ASSERT_VALUE_V8,
    .v16u = ASSERT_VALUE_V16U,
    .v32 = ASSERT_VALUE_V32,
    .v64u = ASSERT_VALUE_V64U,
    .test_array_16 = ASSERT_VALUE_ARRAY16,
    .test_message = ASSERT_VALUE_MESSAGE
};

int data_conversion(void)
{
    itrobj_t itr = itrnew(ITR_F_NONE);
    itrinit(itr, sizeof(int8_t), KIT_TO(itrptr_t, &test_struct));

    assert_int_equal(ASSERT_VALUE_V8, KIT_TO(int8_t, *itrread(itr)));
    itradd(itr, sizeof(uint8_t) + STRUCT_DATA_ALIGN);
    assert_ptr_equal(itrread(itr), &test_struct.v16u);
    assert_int_equal(ASSERT_VALUE_V16U, KIT_TO(uint16_t, *itrread(itr)));
    itradd(itr, sizeof(uint16_t));
    assert_ptr_equal(itrread(itr), &test_struct.v32);
    assert_int_equal(ASSERT_VALUE_V32, KIT_TO(int32_t, *itrread(itr)));
    itradd(itr, sizeof(int32_t));
    assert_ptr_equal(itrread(itr), &test_struct.v64u);
    assert_int_equal(ASSERT_VALUE_V64U, KIT_TO(uint64_t, *itrread(itr)));
    itradd(itr, sizeof(uint64_t));

    assert_ptr_equal(itrread(itr), &test_struct.test_array_16);
    KIT_FOR_EACH(test_struct.test_array_16)
    {
        assert_int_equal(KIT_TO(int8_t, *itrread(itr)), i);
        itrpp(itr);
    }

    assert_ptr_equal(itrread(itr), &test_struct.test_message);
    int result = itrcmp(itr, ASSERT_VALUE_MESSAGE, KIT_ARRAY_SIZE(ASSERT_VALUE_MESSAGE));
    assert_int_equal(result, 0);
    test_struct.test_message[5] ^= 0xFF;
    result = itrcmp(itr, ASSERT_VALUE_MESSAGE, KIT_ARRAY_SIZE(ASSERT_VALUE_MESSAGE));
    assert_int_not_equal(result, 0);

    itrfree(&itr);

    return 0;
}

void test_hello_iterator(void **state)
{
    KIT_UNUSED(state);

    char *test_message = NULL;
    test_message = (char *)malloc(strlen(STR_TEST_MESSAGE) + KIT_NULLTERM);
    strcpy(test_message, STR_TEST_MESSAGE);
    if (!test_message)
        KIT_ASSERT(KIT_FALSE);

    int result = hello_iterator(test_message);
    assert_int_equal(result, 0);

    KIT_FREE(test_message);
}

void test_enumeration_iterator(void **state)
{
    KIT_UNUSED(state);

    int result = enumeration_iterator();
    assert_int_equal(result, 0);
}

void test_index_iterator(void **state)
{
    KIT_UNUSED(state);

    int result = index_iterator();
    assert_int_equal(result, 0);
}

void test_copy_iterator(void **state)
{
    KIT_UNUSED(state);

    int result = copy_iterator();
    assert_int_equal(result, 0);
    result = check_duplicate_iterator();
    assert_int_equal(result, 0);
}

void test_data_conversion(void **state)
{
    KIT_UNUSED(state);

    int result = data_conversion();
    assert_int_equal(result, 0);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hello_iterator),
        cmocka_unit_test(test_enumeration_iterator),
        cmocka_unit_test(test_index_iterator),
        cmocka_unit_test(test_copy_iterator),
        cmocka_unit_test(test_data_conversion),
    };

    itrfree(&itr_template);

    return cmocka_run_group_tests(tests, NULL, NULL);
}