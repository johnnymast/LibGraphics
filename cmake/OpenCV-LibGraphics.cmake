
message(STATUS "  🔨 Linking OpenCV")

if (WIN32)
find_package(OpenCV REQUIRED)

target_include_directories(LibGraphics
        PUBLIC
        ${OpenCV_INCLUDE_DIRS}
)

target_link_directories(LibGraphics
        PUBLIC
        ${OpenCV_LIBRARY_DIRS}
)
endif()

if (UNIX)
    set(CUDAToolkit_ROOT "/opt/cuda")
    find_package(OpenCV REQUIRED)
endif ()

if(OpenCV_CUDA_VERSION)
    add_definitions(-DOPENCV_WITH_CUDA)
endif()