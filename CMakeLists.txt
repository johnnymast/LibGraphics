cmake_minimum_required(VERSION 3.10)
project(LibGraphics VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LIBGRAPHICS_ENABLE_TESTS "Build LibGraphics test suite" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---------------------------------------------------------
# Library sources
# ---------------------------------------------------------
add_library(${PROJECT_NAME} SHARED
        include/modules/stb_image.h
        include/modules/stb_image_write.h
        include/LibGraphics/exceptions/LowConfidenceException.hpp

        include/LibGraphics/ocr/OcrTextReader.hpp
        include/LibGraphics/ocr/OcrResult.hpp

        include/LibGraphics/match/TemplateMatcher.hpp
        include/LibGraphics/match/MatchResult.hpp
        include/LibGraphics/match/MatchOptions.hpp

        include/LibGraphics/type/Region.hpp
        include/LibGraphics/type/Rect.hpp

        include/LibGraphics/color/Information.hpp
        include/LibGraphics/color/BackgroundScanner.hpp

        include/LibGraphics/utils/Converter.hpp

        include/LibGraphics/Image.hpp
        include/LibGraphics.hpp

        src/opencv_info.cpp
        src/ocr/OcrTextReader.cpp
        src/match/TemplateMatcher.cpp
        src/match/MatchResult.cpp
        src/type/Region.cpp
        src/color/Information.cpp
        src/color/BackgroundScanner.cpp
        src/utils/Converter.cpp
        src/LibGraphics.cpp
        src/Image.cpp
)

# ---------------------------------------------------------
# Dependencies
# ---------------------------------------------------------
include(Version)
include(OpenCV)
include(Leptonica)
include(Tesseract)

# ---------------------------------------------------------
# Include directories
# ---------------------------------------------------------
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


# ---------------------------------------------------------
# Linking
# ---------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${OpenCV_LINK_LIBRARIES}
        Tesseract::Tesseract
        Leptonica::Leptonica
)



target_compile_definitions(${PROJECT_NAME} PRIVATE LIBGRAPHICS_EXPORTS)

set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
)

# ---------------------------------------------------------
# Tests
# ---------------------------------------------------------
if(LIBGRAPHICS_ENABLE_TESTS)
    include(Testing)
endif()

# ============================================
# Install rules for LibGraphics
# ============================================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS LibGraphics
        EXPORT LibGraphicsTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/LibGraphics
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT LibGraphicsTargets
        FILE LibGraphicsTargets.cmake
        NAMESPACE LibGraphics::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibGraphics
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/LibGraphicsConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/LibGraphicsConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/LibGraphicsConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibGraphics
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/LibGraphicsConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/LibGraphicsConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LibGraphics
)
