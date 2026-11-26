cmake_minimum_required(VERSION 3.28)
project(http_from_scratch C)

# Ensure a consistent C standard is used across compilers
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create clang configuration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setting directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

# Fetching the Unity unit testing framework
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/throwtheswitch/unity
    GIT_TAG v2.7.0
)
FetchContent_MakeAvailable(unity)

# Enable testing
enable_testing()

# Creating a static library
add_library(
    http_library
    STATIC
    src/server.c
    include/server.h
    src/request.c
    include/request.h
    src/response.c
    include/response.h
    src/header.c
    include/header.h
    include/serve.h
    src/serve.c
)

# So, we can have headers from different directories added to each other with a relative path to the project directory.
target_include_directories(
    http_library
    PUBLIC 
    ${INCLUDE_DIR}
)

# Creating server executable using main.c as the entry point
add_executable(
    http_server 
    ${SRC_DIR}/main.c
)


# Linking the library to the http_server executable target (http_server automatically gets access to ${INCLUDE_DIR} now)
target_link_libraries(
    http_server
    PRIVATE
    http_library
)

# Setting up unit tests 
set(TEST_FILES
    tests/test_request.c
    tests/test_response.c
)

foreach(test_src ${TEST_FILES})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE http_library unity::framework)

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
