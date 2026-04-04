message(STATUS "Linking OpenCV")
if(CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg.cmake")
    find_package(OpenCV CONFIG REQUIRED)
endif()

if(WIN32)
    set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
    find_package(OpenCV REQUIRED)
endif()

if(UNIX)
    set(CUDAToolkit_ROOT "/opt/cuda")
    find_package(OpenCV REQUIRED)
endif()

target_include_directories(LibGraphics
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(LibGraphics
        PUBLIC
        ${OpenCV_LIBRARIES}
)
