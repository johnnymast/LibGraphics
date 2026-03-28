include(FetchContent)

set(TESSERACT_VERSION "5.5.2" CACHE STRING "Tesseract version to use")

add_compile_definitions(TESSDATA_PREFIX=\"/usr/share/tessdata/\")

message(STATUS "Fetching Tesseract...")

FetchContent_Declare(
        tesseract
        GIT_REPOSITORY https://github.com/tesseract-ocr/tesseract.git
        GIT_TAG ${TESSERACT_VERSION}
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(tesseract)

set_target_properties(libtesseract PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(Tesseract::Tesseract ALIAS libtesseract)


message(STATUS "Tesseract OCR setup complete")