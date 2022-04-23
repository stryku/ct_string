cmake_minimum_required(VERSION 3.13)

project(ct_string)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(include)

option(CT_STRING_ENABLE_TESTS "Enable library tests" ON)


if(CT_STRING_ENABLE_TESTS)
    add_subdirectory(tests)
endif()
