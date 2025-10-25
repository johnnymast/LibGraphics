if(WIN32)
    set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OpenCV REQUIRED opencv4)
endif()

