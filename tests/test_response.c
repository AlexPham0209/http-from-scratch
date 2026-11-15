#include "unity.h"
#include "unity_internals.h"
#include "response.h"
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp() {}

void tearDown() {}

void test_response_to_string() {
    Response* response = malloc(sizeof(Response));

    // Status line
    char* version = "HTTP/1.1"; 
    int status_code = 200;
    char* reason_phrase = "OK";
    
    // Response headers
    Headers* headers = malloc(sizeof(Headers));
    headers_init(headers, 3);
    headers_add(headers, (Header){"Server", "Apache"});
    headers_add(headers, (Header){"Content-Type", "application/json"});
    headers_add(headers, (Header){"Last-Modified", "Sun, 15 Nov 2026 9:45:00 EST"}); 

    // Body 
    char* body = "{message: \"This is the response\"}";
    
    // Converting response struct into string
    response_init(
        response,
        version,
        status_code,
        reason_phrase,
        headers,
        body
    );
    char* actual = response_to_string(response);
    char* expected = 
        "HTTP/1.1 200 OK\r\n"
        "Server: Apache\r\n"
        "Content-Type: application/json\r\n"
        "Last-Modified: Sun, 15 Nov 2026 9:45:00 EST\r\n"
        "\r\n"
        "{message: \"This is the response\"}";

    TEST_ASSERT_EQUAL(strlen(expected), strlen(actual));
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    
    free(actual);
    response_free(response);
}

void test_response_to_string_no_headers() {
    Response* response = malloc(sizeof(Response));

    // Status line
    char* version = "HTTP/1.1"; 
    int status_code = 200;
    char* reason_phrase = "OK";
    
    // Body 
    char* body = "{message: \"This is the response\"}";
    
    // Converting response struct into string
    response_init(
        response,
        version,
        status_code,
        reason_phrase,
        NULL,
        body
    );
    char* actual = response_to_string(response);
    char* expected = 
        "HTTP/1.1 200 OK\r\n"
        "\r\n"
        "{message: \"This is the response\"}";

    TEST_ASSERT_EQUAL(strlen(expected), strlen(actual));
    TEST_ASSERT_EQUAL_STRING(expected, actual);
    
    free(actual);
    response_free(response);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_response_to_string);
    RUN_TEST(test_response_to_string_no_headers);
    return UNITY_END();
}