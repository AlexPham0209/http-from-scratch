#include "unity.h"
#include "unity_internals.h"
#include "request.h"
#include <complex.h>
#include <string.h>

void setUp() {}

void tearDown() {}

void test_get_line(void) {
    char str[1][100] = {"TEST\r\nTEST2\r\nTEST3\r\n\r\nBODY"};
    char* ptr = str[0];
    
    // Consuming every new line in the string
    char* a = get_line(&ptr);
    char* b = get_line(&ptr);
    char* c = get_line(&ptr);
    char* d = get_line(&ptr);
    char* e = get_line(&ptr);
    
    // Testing each line (expected, actual)
    TEST_ASSERT_EQUAL_STRING("TEST", a);
    TEST_ASSERT_EQUAL_STRING("TEST2", b);
    TEST_ASSERT_EQUAL_STRING("TEST3", c);
    TEST_ASSERT_EQUAL_STRING("", d);
    TEST_ASSERT_EQUAL_STRING("BODY", e);
}

void test_get_line_2(void) {
    char str[1][100] = {"GET / HTTP/1.1\r\n TEST2"};
    char* ptr = str[0];
    
    char* request_line = get_line(&ptr);
    char* next = get_line(&ptr);
    
    char* method = strsep(&request_line, " ");
    char* request_target = strsep(&request_line, " ");
    char* version = strsep(&request_line, " ");
    char* additional = strsep(&request_line, " ");

    // Testing each token and line
    TEST_ASSERT_EQUAL_STRING("GET", method);
    TEST_ASSERT_EQUAL_STRING("/", request_target);
    TEST_ASSERT_EQUAL_STRING("HTTP/1.1", version);

    // Safely assert null using TEST_ASSERT_NULL instead of strcmp-based assertions
    TEST_ASSERT_NULL(additional);
    TEST_ASSERT_EQUAL_STRING(" TEST2", next);
}

void test_request_success(void) {
    char request[] = "GET / HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 
    Request* req = parse_request(request);

    // Asserting request is not null safely
    TEST_ASSERT_NOT_NULL(req);

    // Request line (expected, actual)
    TEST_ASSERT_EQUAL(GET, req->method);
    TEST_ASSERT_EQUAL_STRING("/", req->request_target);
    TEST_ASSERT_EQUAL_STRING("HTTP/1.1", req->version);

    // Headers
    Headers* headers = req->headers;
    TEST_ASSERT_EQUAL(2, headers->size);
    TEST_ASSERT_EQUAL_STRING("Host", headers->data[0].key);
    TEST_ASSERT_EQUAL_STRING("example.com", headers->data[0].value);
    TEST_ASSERT_EQUAL_STRING("Content-Type", headers->data[1].key);
    TEST_ASSERT_EQUAL_STRING("application/json", headers->data[1].value);

    // Body 
    TEST_ASSERT_EQUAL_STRING("{\"status\": \"ok\"}", req->body);
    request_free(req);
}

void test_request_only_lf(void) {
    char request[] = "GET / HTTP/1.1\nHost: example.com\nContent-Type: application/json\n\n{\"status\": \"ok\"}"; 
    Request* req = parse_request(request);

    // Asserting request is not null safely
    TEST_ASSERT_NOT_NULL(req);

    // Request line (expected, actual)
    TEST_ASSERT_EQUAL(GET, req->method);
    TEST_ASSERT_EQUAL_STRING("/", req->request_target);
    TEST_ASSERT_EQUAL_STRING("HTTP/1.1", req->version);

    // Headers
    Headers* headers = req->headers;
    TEST_ASSERT_EQUAL(2, headers->size);
    TEST_ASSERT_EQUAL_STRING("Host", headers->data[0].key);
    TEST_ASSERT_EQUAL_STRING("example.com", headers->data[0].value);
    TEST_ASSERT_EQUAL_STRING("Content-Type", headers->data[1].key);
    TEST_ASSERT_EQUAL_STRING("application/json", headers->data[1].value);

    // Body 
    TEST_ASSERT_EQUAL_STRING("{\"status\": \"ok\"}", req->body);
    request_free(req);
}

void test_request_missing_request_line(void) {
    char request[] = "Host: example.com\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 
    Request* req = parse_request(request);

    // Asserting request is null
    TEST_ASSERT_NULL(req);
}

void test_request_malformed_request_line(void) {
    char request_1[] = "GET\r\nHost: example.com\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 
    char request_2[] = "GET /\r\nHost: example.com\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 
    char request_3[] = "/\r\nHost: example.com\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 
    char request_4[] = "HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 
    char request_5[] = "HTTP/1.1 / HTTP/1.1 Random nonsense\r\nHost: example.com\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 

    Request* req_1 = parse_request(request_1);
    Request* req_2 = parse_request(request_2);
    Request* req_3 = parse_request(request_3);
    Request* req_4 = parse_request(request_4);
    Request* req_5 = parse_request(request_5);

    // Asserting all return null
    TEST_ASSERT_NULL(req_1);
    TEST_ASSERT_NULL(req_2);
    TEST_ASSERT_NULL(req_3);
    TEST_ASSERT_NULL(req_4);
    TEST_ASSERT_NULL(req_5);
}

void test_request_malformed_header(void) {
    char request_1[] = "GET\r\n\r\n\r\n\r\n{\"status\": \"ok\"}"; 
    char request_2[] = "GET /\r\nHost\r\nContent-Type: application/json\r\n\r\n{\"status\": \"ok\"}"; 

    Request* req_1 = parse_request(request_1);
    Request* req_2 = parse_request(request_2);

    // Asserting requests are null
    TEST_ASSERT_NULL(req_1);
    TEST_ASSERT_NULL(req_2);
}

void test_request_missing_empty_line(void) {
    char request[] = "GET / HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/json\r\n{\"status\": \"ok\"}"; 
    Request* req = parse_request(request);

    // Asserting request is null
    TEST_ASSERT_NULL(req);
}

void test_request_empty_body(void) {
    char request[] = "GET / HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/json\r\n\r\n"; 
    Request* req = parse_request(request);

    // Asserting request is not null
    TEST_ASSERT_NOT_NULL(req);

    // Request line
    TEST_ASSERT_EQUAL(GET, req->method);
    TEST_ASSERT_EQUAL_STRING("/", req->request_target);
    TEST_ASSERT_EQUAL_STRING("HTTP/1.1", req->version);

    // Headers
    Headers* headers = req->headers;
    TEST_ASSERT_EQUAL(2, headers->size);
    TEST_ASSERT_EQUAL_STRING("Host", headers->data[0].key);
    TEST_ASSERT_EQUAL_STRING("example.com", headers->data[0].value);
    TEST_ASSERT_EQUAL_STRING("Content-Type", headers->data[1].key);
    TEST_ASSERT_EQUAL_STRING("application/json", headers->data[1].value);

    // Body 
    TEST_ASSERT_EQUAL_STRING("", req->body);
    request_free(req);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_line);
    RUN_TEST(test_get_line_2);
    RUN_TEST(test_request_success);
    RUN_TEST(test_request_only_lf);
    RUN_TEST(test_request_missing_request_line);
    RUN_TEST(test_request_malformed_request_line);
    RUN_TEST(test_request_malformed_header);
    RUN_TEST(test_request_missing_empty_line);
    RUN_TEST(test_request_empty_body);
    return UNITY_END();
}