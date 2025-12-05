# ... existing code ...
include(FetchContent)

# If using vcpkg (typical on Windows), prefer system packages over FetchContent
if (WIN32 OR CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg.cmake")
    find_package(Tesseract CONFIG QUIET)
    find_package(Leptonica CONFIG QUIET)

    if (TARGET Tesseract::libtesseract OR TARGET Tesseract::Tesseract)
        # Normalize alias name
        if (NOT TARGET Tesseract::Tesseract AND TARGET Tesseract::libtesseract)
            add_library(Tesseract::Tesseract ALIAS Tesseract::libtesseract)
        endif()
        message(STATUS "Using Tesseract/Leptonica from package manager/toolchain")
        # Define runtime data path (adjust if needed)
        add_compile_definitions(TESSDATA_PREFIX=\"/usr/share/tessdata/\")
        return()
    endif()

    message(STATUS "Package manager not providing Tesseract/Leptonica; falling back to FetchContent")
endif()

# Set Tesseract version
set(TESSERACT_VERSION "5.3.3" CACHE STRING "Tesseract version to use")

# Save current flags
set(SAVED_CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(SAVED_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Define TESSDATA_PREFIX for runtime use
add_compile_definitions(TESSDATA_PREFIX=\"/usr/share/tessdata/\")

# Suppress warnings for Leptonica
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# Tesseract requires Leptonica
FetchContent_Declare(
        leptonica
        GIT_REPOSITORY https://github.com/DanBloomberg/leptonica.git
        GIT_TAG 1.86.0
        GIT_SHALLOW TRUE
)
# ... existing code ...
FetchContent_Declare(
        tesseract
        GIT_REPOSITORY https://github.com/tesseract-ocr/tesseract.git
        GIT_TAG ${TESSERACT_VERSION}
        GIT_SHALLOW TRUE
)

# Leptonica options
set(BUILD_PROG OFF CACHE BOOL "" FORCE)
set(SW_BUILD OFF CACHE BOOL "" FORCE)

# Tesseract options
set(BUILD_TRAINING_TOOLS OFF CACHE BOOL "" FORCE)
set(INSTALL_CONFIGS OFF CACHE BOOL "" FORCE)
set(GRAPHICS_DISABLED ON CACHE BOOL "" FORCE)
set(DISABLED_LEGACY_ENGINE OFF CACHE BOOL "" FORCE)

# Windows-specific tweaks for MinGW builds (kept for non-vcpkg flows)
if (WIN32)
    if (NOT PKG_CONFIG_EXECUTABLE)
        find_program(PKG_CONFIG_EXECUTABLE NAMES x86_64-w64-mingw32-pkg-config pkg-config)
    endif()
    if (PKG_CONFIG_EXECUTABLE)
        message(STATUS "Using pkg-config: ${PKG_CONFIG_EXECUTABLE}")
        set(ENV{PKG_CONFIG} "${PKG_CONFIG_EXECUTABLE}")
    else()
        message(WARNING "pkg-config not found. On Windows with MinGW, install MSYS2 and 'mingw-w64-x86_64-pkg-config' and run CMake from the MinGW shell.")
    endif()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
    set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-lto")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-lto")
endif()

message(STATUS "Fetching Leptonica...")
FetchContent_GetProperties(leptonica)
if (NOT leptonica_POPULATED)
    FetchContent_Populate(leptonica)
    file(READ "${leptonica_SOURCE_DIR}/CMakeLists.txt" LEPTONICA_CMAKE)
    string(REPLACE "cmake_minimum_required(VERSION 3.1.3)" "cmake_minimum_required(VERSION 3.5)" LEPTONICA_CMAKE "${LEPTONICA_CMAKE}")
    file(WRITE "${leptonica_SOURCE_DIR}/CMakeLists.txt" "${LEPTONICA_CMAKE}")
    add_subdirectory(${leptonica_SOURCE_DIR} ${leptonica_BINARY_DIR})
    set_target_properties(leptonica PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif ()

message(STATUS "Fetching Tesseract...")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (WIN32)
    if (EXISTS "${leptonica_BINARY_DIR}")
        set(Leptonica_DIR "${leptonica_BINARY_DIR}" CACHE PATH "Leptonica binary dir" FORCE)
    endif()
endif()
FetchContent_MakeAvailable(tesseract)
set_target_properties(libtesseract PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_library(Tesseract::Tesseract ALIAS libtesseract)
message(STATUS "Tesseract OCR setup complete")

# Restore flags
set(CMAKE_C_FLAGS "${SAVED_CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${SAVED_CMAKE_CXX_FLAGS}")