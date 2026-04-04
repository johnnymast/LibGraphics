# Handle vcpkg builds
if(CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg.cmake")
    find_package(OpenCV CONFIG REQUIRED)
    set(OpenCV_INCLUDE_DIRS
            ${OpenCV_INCLUDE_DIRS}
            ${OpenCV_INCLUDE_DIR}
            ${OpenCV_INCLUDE_DIRS_CONFIG}
    )
    set(OpenCV_LINK_LIBRARIES
            opencv_core
            opencv_imgproc
            opencv_imgcodecs
            opencv_highgui
            opencv_videoio
    )
    message(STATUS "Using OpenCV from vcpkg")
    return()
endif()

if(WIN32)
    set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
    find_package(OpenCV REQUIRED)

    set(OpenCV_INCLUDE_DIRS
            ${OpenCV_INCLUDE_DIRS}
            ${OpenCV_INCLUDE_DIR}
    )
    set(OpenCV_LINK_LIBRARIES
            ${OpenCV_LIBS}
    )

elseif(UNIX)
    set(CUDAToolkit_ROOT "/opt/cuda")
    find_package(OpenCV REQUIRED)

    set(OpenCV_INCLUDE_DIRS
            ${OpenCV_INCLUDE_DIRS}
            ${OpenCV_INCLUDE_DIRS_OPENCV4}
    )
    set(OpenCV_LINK_LIBRARIES
            ${OpenCV_LIBRARIES}
    )
endif()


