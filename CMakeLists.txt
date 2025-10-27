cmake_minimum_required(VERSION 3.10)
project(LibGraphics VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LIBGRAPHICS_ENABLE_TESTS "Build LibGraphics test suite" ON)



list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(${PROJECT_NAME} SHARED
        include/modules/stb_image.h
        include/modules/stb_image_write.h
        include/LibGraphics/exceptions/LowConfidenceException.hpp

        include/LibGraphics/ocr/OcrTextReader.hpp
        include/LibGraphics/ocr/OcrResult.hpp

        include/LibGraphics/match/TemplateMatcher.hpp
        include/LibGraphics/match/Region.hpp
        include/LibGraphics/match/MatchResult.hpp
        include/LibGraphics/match/MatchOptions.hpp

        include/LibGraphics/color/Information.hpp
        include/LibGraphics/color/BackgroundScanner.hpp

        include/LibGraphics/utils/Converter.hpp

        include/LibGraphics/Image.hpp
        include/LibGraphics.hpp


        src/opencv_info.cpp

        src/ocr/OcrTextReader.cpp

        src/match/TemplateMatcher.cpp
        src/match/Region.cpp
        src/match/MatchResult.cpp

        src/color/Information.cpp
        src/color/BackgroundScanner.cpp

        src/utils/Converter.cpp

        src/LibGraphics.cpp
        src/Image.cpp
)


include(Version)
include(OpenCV)
include(Tesseract)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_BINARY_DIR}/generated
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${OpenCV_LINK_LIBRARIES}
        PRIVATE
        Tesseract::Tesseract
        leptonica
)


target_compile_definitions(${PROJECT_NAME} PRIVATE LIBGRAPHICS_EXPORTS)

set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
)

if(LIBGRAPHICS_ENABLE_TESTS)
    include(Testing)
endif()
