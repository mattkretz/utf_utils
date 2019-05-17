cmake_minimum_required (VERSION 3.5)
project (utf_utils)

include_directories(src)

set(Sources
    src/utf_utils.cpp
    src/utf_utils.h

    test/av_utf8.c
    test/av_utf8.h
    test/boost_utf8.hpp
    test/boost_utf8_config.hpp
    test/hoehrmann.cpp
    test/hoehrmann.h
    test/llvm_convert_utf.c
    test/llvm_convert_utf.h
    test/test_basics.cpp
    test/test_conversions_16.cpp
    test/test_conversions_32.cpp
    test/test_main.cpp
    test/test_main.h
)

include_directories(test)
add_executable(utf_utils_test ${Sources})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -std=c++17")
set(CMAKE_VERBOSE_MAKEFILE 1)
