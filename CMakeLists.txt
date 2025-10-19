cmake_minimum_required(VERSION 3.10)
project(LibGraphics VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(OpenCV)


add_library(${PROJECT_NAME} SHARED
        src/library.cpp
        src/opencv_info.cpp
        src/helpers/scan.cpp
        src/helpers/scan.hpp
        src/color/Information.cpp
        include/LibGraphics/color/Information.hpp
        src/color/Scanner.cpp
        src/color/Scanner.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE LIBGRAPHICS_EXPORTS)

set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
)

include(Testing)