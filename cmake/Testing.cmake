include(FetchContent)
# Catch2 v3
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(test_color
        tests/color/test_color.cpp
        tests/color/test_backgroundscanner.cpp
)

# Link your library (provides helpers::Color) and Catch2's main
target_link_libraries(test_color
        PRIVATE
        LibGraphics
        Catch2::Catch2WithMain
)

# Tests may include headers from src and include
target_include_directories(test_color PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Register tests
include(CTest)
include(Catch)

catch_discover_tests(test_color)
#catch_discover_tests(test_backgroundscanner)