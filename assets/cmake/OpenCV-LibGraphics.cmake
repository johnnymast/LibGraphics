
message(STATUS "  🔨 Linking OpenCV")

find_package(OpenCV REQUIRED)

target_include_directories(LibGraphics
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
)

target_link_directories(LibGraphics
        PUBLIC
        ${OpenCV_LIBRARY_DIRS}
)