message(STATUS "Linking OpenCV")

if (WIN32)
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
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_imgcodecs
        opencv_videoio
        opencv_calib3d
)