# Tesseract.cmake — Arch Linux version (pkg-config)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TESS REQUIRED tesseract)

add_library(Tesseract::Tesseract INTERFACE IMPORTED)

target_include_directories(Tesseract::Tesseract INTERFACE
        ${TESS_INCLUDE_DIRS}
)

target_link_libraries(Tesseract::Tesseract INTERFACE
        ${TESS_LIBRARIES}
)
