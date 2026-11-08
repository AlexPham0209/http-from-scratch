#include "unity.h"
#include "unity_internals.h"
#include "request.h"
#include <string.h>

void setUp() {}

void tearDown() {}

void test_get_line() {
    char str[1][100] = {"TEST\r\nTEST2\r\nTEST3\r\n\r\nBODY"};
    char* ptr = str[0];
    
    // Consuming every new line in the string
    char* a = get_line(&ptr);
    char* b = get_line(&ptr);
    char* c = get_line(&ptr);
    char* d = get_line(&ptr);
    char* e = get_line(&ptr);
    
    // Testing each line
    TEST_ASSERT_EQUAL_STRING(a, "TEST");
    TEST_ASSERT_EQUAL_STRING(b, "TEST2");
    TEST_ASSERT_EQUAL_STRING(c, "TEST3");
    TEST_ASSERT_EQUAL_STRING(d, "");
    TEST_ASSERT_EQUAL_STRING(e, "BODY");
}

void test_get_line_2(void) {
    char str[1][100] = {"GET / HTTP/1.1r\n TEST2"};
    char* ptr = str[0];
    
    char* request_line = get_line(&ptr);
    char* next = get_line(&ptr);
    
    char* method = strsep(&request_line, " ");
    char* request_target = strsep(&request_line, " ");
    char* version = strsep(&request_line, " ");
    char* additional = strsep(&request_line, " ");

    // Testing each line
    TEST_ASSERT_EQUAL_STRING(method, "GET");
    TEST_ASSERT_EQUAL_STRING(request_target, "/");
    TEST_ASSERT_EQUAL_STRING(version, "HTTP/1.1");

    // Ensures that strsep doesn't search past the null terminator set by get_line
    TEST_ASSERT_EQUAL_STRING(additional, NULL);
    TEST_ASSERT_EQUAL_STRING(next, " TEST2");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_line);
    RUN_TEST(test_get_line_2);
    return UNITY_END();
}