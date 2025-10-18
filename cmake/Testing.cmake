include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_color
        tests/helpers/test_color.cpp
)

target_link_libraries(test_color gtest_main)
target_include_directories(test_color PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src)


add_test(NAME ColorTest COMMAND test_color)

