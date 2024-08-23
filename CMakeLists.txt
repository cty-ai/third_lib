
cmake_minimum_required(VERSION 3.11)
project(third_lib)
set(BINARY_NAME test)
set(CMAKE_CXX_STANDARD 11)

# add_library(third_lib SHARED src/third_lib.cpp)
# target_include_directories(third_lib PUBLIC include)

include_directories(${CMAKE_SOURCE_DIR})

include(FetchContent)
set(BUILD_SHARED_LIBS ON)

message("clone auto-log")
include(external-cmake/auto-log.cmake)
include_directories(${FETCHCONTENT_BASE_DIR}/extern_autolog-src)

message("clone plog")
include(external-cmake/plog.cmake)
include_directories(${FETCHCONTENT_BASE_DIR}/extern_plog-src/include)

message("clone curl")
include(external-cmake/curl.cmake)
include_directories(${FETCHCONTENT_BASE_DIR}/extern_curl-src/include)
#link_directories(${FETCHCONTENT_BASE_DIR}/extern_curl-build/lib)

message("clone libevent")
include(external-cmake/libevent.cmake)
include_directories(${FETCHCONTENT_BASE_DIR}/extern_libevent-src/include)
#link_directories(${FETCHCONTENT_BASE_DIR}/extern_libevent-build/lib)

message("clone cpp-httplib")
include(external-cmake/cpp-httplib.cmake)
include_directories(${FETCHCONTENT_BASE_DIR}/extern_cpp-httplib-src)

set(EXTERNAL_LIB "-ldl -lrt -lgomp -lz -lm -lpthread -lssl -lcrypto -lcurl -levent -levent_openssl")
set(DEPS ${DEPS} ${EXTERNAL_LIB})

AUX_SOURCE_DIRECTORY(./ SRCS)
add_executable(${BINARY_NAME} ${SRCS})

target_link_libraries(${BINARY_NAME} ${DEPS})
