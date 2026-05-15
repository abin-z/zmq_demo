cmake_minimum_required(VERSION 3.16)
project(reqrep_basic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 若已在 reqrep_basic/.deps 下安装过 libzmq（例如首次无系统包时从源码安装），优先使用该前缀
set(_reqrep_local_prefix "${CMAKE_CURRENT_SOURCE_DIR}/.deps/prefix")
if(EXISTS "${_reqrep_local_prefix}/lib/libzmq.so" OR EXISTS "${_reqrep_local_prefix}/lib/libzmq.a")
  list(PREPEND CMAKE_PREFIX_PATH "${_reqrep_local_prefix}")
endif()

find_library(ZMQ_LIBRARY NAMES zmq REQUIRED)
find_path(ZMQ_INCLUDE_DIR zmq.h REQUIRED)
find_path(
  CPPZMQ_INCLUDE_DIR
  "zmq.hpp"
  HINTS
    "${CMAKE_CURRENT_SOURCE_DIR}/.deps/cppzmq"
    "${_reqrep_local_prefix}/include"
  REQUIRED)

add_executable(reqrep_server server.cpp)
target_include_directories(reqrep_server PRIVATE ${ZMQ_INCLUDE_DIR} ${CPPZMQ_INCLUDE_DIR})
target_link_libraries(reqrep_server PRIVATE ${ZMQ_LIBRARY})

add_executable(reqrep_client client.cpp)
target_include_directories(reqrep_client PRIVATE ${ZMQ_INCLUDE_DIR} ${CPPZMQ_INCLUDE_DIR})
target_link_libraries(reqrep_client PRIVATE ${ZMQ_LIBRARY})
