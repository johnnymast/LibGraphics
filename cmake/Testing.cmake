include(FetchContent)

# Catch2 v3
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(graphics_testsuite
        tests/color/Color.test.cpp
        tests/color/BackgroundScanner.test.cpp
        tests/color/BackgroundScanner.wrappers.test.cpp
        tests/match/Region.test.cpp
        tests/match/MatchResult.test.cpp
        tests/match/TemplateMatcher.test.cpp
        tests/utils/Converter.test.cpp
        tests/ocr/OcrTextReader.test.cpp
        tests/image.test.cpp
)

# Include paths for tests
target_include_directories(graphics_testsuite
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(graphics_testsuite
        PRIVATE
        LibGraphics
        Catch2::Catch2WithMain
)

# Register tests
include(CTest)
include(Catch)
catch_discover_tests(graphics_testsuite)
