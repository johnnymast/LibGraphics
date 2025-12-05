# ... existing code ...
# Prefer vcpkg if a vcpkg toolchain is in use (works on Windows, Linux, macOS)
if(CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg.cmake")
    find_package(OpenCV CONFIG REQUIRED)
    # Expose common variables used by the rest of the project
    set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS_CONFIG})
    set(OpenCV_LINK_LIBRARIES opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui opencv_videoio)
    message(STATUS "Using OpenCV from vcpkg")
    return()
endif()

if(WIN32)
    # Legacy manual path (optional). Prefer vcpkg above.
    set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
    find_package(OpenCV REQUIRED)
    set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIR})
    set(OpenCV_LINK_LIBRARIES ${OpenCV_LIBS})
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OpenCV REQUIRED opencv4)
    set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS_OPENCV4})
    set(OpenCV_LINK_LIBRARIES ${OpenCV_LINK_LIBRARIES} ${OpenCV_LIBRARIES})
endif()
# ... existing code ...