# Detecteer platform en configureer OpenCV
if(WIN32)
    # Windows: pad naar OpenCV build directory
    set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
elseif(UNIX)
    # Linux: probeer pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OpenCV REQUIRED opencv4)
endif()

# Vind OpenCV
find_package(OpenCV REQUIRED)
