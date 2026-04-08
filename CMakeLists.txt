cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0074 NEW)

project(LibGraphics VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LIBGRAPHICS_ENABLE_TESTS "Build LibGraphics test suite" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


# Project titles naar LibGraphics

add_library(LibGraphics SHARED
        include/private/LibGraphics/modules/stb_image.hpp
        include/private/LibGraphics/modules/stb_image_write.hpp
        include/public/LibGraphics/exceptions/LowConfidenceException.hpp

        include/public/LibGraphics/ocr/OcrTextReader.hpp
        include/public/LibGraphics/ocr/OcrResult.hpp

        include/public/LibGraphics/match/TemplateMatcher.hpp
        include/public/LibGraphics/match/MatchResult.hpp
        include/public/LibGraphics/match/MatchOptions.hpp

        include/public/LibGraphics/type/Region.hpp
        include/public/LibGraphics/type/Rect.hpp

        include/public/LibGraphics/color/Information.hpp
        include/public/LibGraphics/color/BackgroundScanner.hpp

        include/public/LibGraphics/utils/Converter.hpp

        include/public/LibGraphics/Image.hpp
        include/public/LibGraphics/LibGraphics.hpp

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

if (MSVC)
    target_compile_options(LibGraphics PRIVATE /wd4251)
endif()

include(Version)
include(Leptonica)
include(Tesseract)
include(OpenCV-LG)

target_include_directories(LibGraphics
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/private
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_link_libraries(LibGraphics
        PRIVATE
        Tesseract::Tesseract
        Leptonica::Leptonica
        ${OpenCV_LIBS}
)

target_compile_definitions(LibGraphics PRIVATE LIBGRAPHICS_EXPORTS)

set_target_properties(LibGraphics PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
)

if(LIBGRAPHICS_ENABLE_TESTS)
    include(Testing)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS LibGraphics
        EXPORT LibGraphicsTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/public/LibGraphics
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