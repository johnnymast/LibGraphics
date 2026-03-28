# ... existing code ...
include(FetchContent)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# Leptonica options
#set(BUILD_PROG OFF CACHE BOOL "" FORCE)
#set(SW_BUILD OFF CACHE BOOL "" FORCE)

## If using vcpkg (typical on Windows), prefer system packages over FetchContent
#if (WIN32 OR CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg.cmake")
#    find_package(Tesseract CONFIG QUIET)
#    find_package(Leptonica CONFIG QUIET)
#
#    if (TARGET Tesseract::libtesseract OR TARGET Tesseract::Tesseract)
#        # Normalize alias name
#        if (NOT TARGET Tesseract::Tesseract AND TARGET Tesseract::libtesseract)
#            add_library(Tesseract::Tesseract ALIAS Tesseract::libtesseract)
#        endif()
#        message(STATUS "Using Tesseract/Leptonica from package manager/toolchain")
#        # Define runtime data path (adjust if needed)
#        add_compile_definitions(TESSDATA_PREFIX=\"/usr/share/tessdata/\")
#        return()
#    endif()
#
#    message(STATUS "Package manager not providing Tesseract/Leptonica; falling back to FetchContent")
#endif()

message(STATUS "Fetching Leptonica...")

FetchContent_Declare(
        leptonica
        GIT_REPOSITORY https://github.com/DanBloomberg/leptonica.git
        GIT_TAG        1.85.0 # Or another specific tag/commit
)

#if (WIN32)
#    if (EXISTS "${leptonica_BINARY_DIR}")
#        set(Leptonica_DIR "${leptonica_BINARY_DIR}" CACHE PATH "Leptonica binary dir" FORCE)
#    endif()
#endif()

FetchContent_MakeAvailable(leptonica)