include(FetchContent)

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
        GIT_TAG 1.83.1
        GIT_SHALLOW TRUE
)

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

# Make available
message(STATUS "Fetching Leptonica...")
FetchContent_GetProperties(leptonica)

if (NOT leptonica_POPULATED)
    FetchContent_Populate(leptonica)

    # Patch CMakeLists.txt to fix CMake version requirement
    file(READ "${leptonica_SOURCE_DIR}/CMakeLists.txt" LEPTONICA_CMAKE)
    string(REPLACE "cmake_minimum_required(VERSION 3.1.3)" "cmake_minimum_required(VERSION 3.5)" LEPTONICA_CMAKE "${LEPTONICA_CMAKE}")
    file(WRITE "${leptonica_SOURCE_DIR}/CMakeLists.txt" "${LEPTONICA_CMAKE}")

    add_subdirectory(${leptonica_SOURCE_DIR} ${leptonica_BINARY_DIR})

    set_target_properties(leptonica PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif ()

message(STATUS "Fetching Tesseract...")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_MakeAvailable(tesseract)
set_target_properties(libtesseract PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Create interface library to simplify linking
add_library(Tesseract::Tesseract ALIAS libtesseract)

message(STATUS "Tesseract OCR setup complete")

# Restore flags
set(CMAKE_C_FLAGS "${SAVED_CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${SAVED_CMAKE_CXX_FLAGS}")