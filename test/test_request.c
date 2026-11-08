#include "unity.h"
#include "unity_internals.h"

void setUp() {

}

void tearDown() {

}

void test_request(void) {
    TEST_ASSERT_EQUAL(10, 10);
}

void test_request_2(void) {
    TEST_ASSERT_EQUAL(10, 10);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_request);
    RUN_TEST(test_request_2);
    return UNITY_END();
}