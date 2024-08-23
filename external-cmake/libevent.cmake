find_package(Git REQUIRED)
include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_BINARY_DIR}/third-party")

FetchContent_Declare(
    extern_libevent
    # GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
    GIT_REPOSITORY https://gitee.com/mirrors/libevent.git
    GIT_TAG        master
)

# 不构建可执行文件
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_EXECUTABLES OFF CACHE BOOL "Build executables in the library" FORCE)

FetchContent_MakeAvailable(extern_libevent)
