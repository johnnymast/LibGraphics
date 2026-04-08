
message(STATUS "Linking OpenCV")

find_package(OpenCV REQUIRED)

target_include_directories(LibGraphics
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
)

target_link_directories(LibGraphics
        PUBLIC
        ${OpenCV_LIBRARY_DIRS}
)

message(STATUS "=== LibCore OpenCV Debug ===")
message(STATUS "OpenCV_DIR: ${OpenCV_DIR}")
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBRARIES: ${OpenCV_LIBRARIES}")
message(STATUS "============================")
