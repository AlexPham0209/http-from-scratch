cmake_minimum_required(VERSION 3.28)
project(http_from_scratch C)

# Ensure a consistent C standard is used across compilers
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Create clang configuration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setting directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Creating a static library
add_library(
    http_library
    STATIC
    src/server.c
    include/server.h 
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