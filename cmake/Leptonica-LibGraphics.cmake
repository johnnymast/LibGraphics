if (WIN32)
    # Windows (vcpkg): use LeptonicaConfig.cmake
    find_package(Leptonica CONFIG REQUIRED)

    add_library(Leptonica::Leptonica INTERFACE IMPORTED)

    target_link_libraries(Leptonica::Leptonica INTERFACE
            leptonica
    )

else()
    # Linux/macOS: use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LEPT REQUIRED lept)

    add_library(Leptonica::Leptonica INTERFACE IMPORTED)

    target_include_directories(Leptonica::Leptonica INTERFACE
            ${LEPT_INCLUDE_DIRS}
    )

    target_link_libraries(Leptonica::Leptonica INTERFACE
            ${LEPT_LIBRARIES}
    )
endif()
