# Leptonica.cmake — Arch Linux version (pkg-config)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LEPT REQUIRED lept)

add_library(Leptonica::Leptonica INTERFACE IMPORTED)

target_include_directories(Leptonica::Leptonica INTERFACE
        ${LEPT_INCLUDE_DIRS}
)

target_link_libraries(Leptonica::Leptonica INTERFACE
        ${LEPT_LIBRARIES}
)
