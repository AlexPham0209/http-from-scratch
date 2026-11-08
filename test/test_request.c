#include "unity.h"
#include "unity_internals.h"
#include "request.h"
void setUp() {

}

void tearDown() {

}
void test_consume() {
    char str[1][100] = {"TEST\r\nTEST2\r\nTEST3\r\n\r\nBODY"};
    char* ptr = str[0];
    
    // Consuming every new line in the string
    char* a = consume(&ptr);
    char* b = consume(&ptr);
    char* c = consume(&ptr);
    char* d = consume(&ptr);
    char* e = consume(&ptr);
    
    // Testing each line
    TEST_ASSERT_EQUAL_STRING(a, "TEST");
    TEST_ASSERT_EQUAL_STRING(b, "TEST2");
    TEST_ASSERT_EQUAL_STRING(c, "TEST3");
    TEST_ASSERT_EQUAL_STRING(d, "");
    TEST_ASSERT_EQUAL_STRING(e, "BODY");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_consume);
    return UNITY_END();
}