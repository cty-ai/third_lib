find_package(Git REQUIRED)
include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}/third-party")

FetchContent_Declare(
    extern_curl
    # GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
    GIT_REPOSITORY https://gitee.com/gwlee/curl.git
    GIT_TAG        master
)

# 不构建可执行文件
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(extern_curl)