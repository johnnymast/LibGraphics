# ... existing code ...
include(FetchContent)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "Fetching Leptonica...")

FetchContent_Declare(
        leptonica
        GIT_REPOSITORY https://github.com/DanBloomberg/leptonica.git
        GIT_TAG        1.85.0 # Or another specific tag/commit
)

FetchContent_MakeAvailable(leptonica)