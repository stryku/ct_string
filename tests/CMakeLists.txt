enable_testing()


include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(ct_string_tests ct_string_tests.cpp)

target_link_libraries(ct_string_tests
    ct_string
    gtest
    gtest_main
)

gtest_discover_tests(ct_string_tests)
