find_package(Git REQUIRED)
include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}/third-party")

FetchContent_Declare(
    extern_plog
    # GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
    GIT_REPOSITORY https://gitee.com/alvisisme/plog.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(extern_plog)