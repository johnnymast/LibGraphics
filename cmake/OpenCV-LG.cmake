message(STATUS "Linking OpenCV")

if (WIN32)
    set(OpenCV_ROOT "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
    find_package(OpenCV REQUIRED)
endif ()

if (UNIX)
    set(CUDAToolkit_ROOT "/opt/cuda")
    find_package(OpenCV REQUIRED)
endif ()

target_include_directories(LibGraphics
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(LibGraphics
        PUBLIC
        ${OpenCV_LIBRARIES}
)
