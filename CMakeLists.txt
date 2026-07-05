cmake_minimum_required(VERSION 3.16)

project(ZMQ_DEMO LANGUAGES C CXX)

# 生成 compile_commands.json 文件（用于 clangd、clang-tidy 等工具）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置默认构建类型为Release，如果未指定构建类型
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

# 设置所有库生成位置无关代码 (Position Independent Code) 对应GCC/Clang的-fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
# 静态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/lib)
# 动态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/bin)
# 可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build_output/bin)


# 添加fmt库
include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 12.2.0
)

FetchContent_MakeAvailable(fmt)
# 将 fmt 设置为全局链接库。
# 当前项目仅用于 Demo/学习用途，所有 target 都会使用 fmt，
# 因此直接使用 link_libraries() 简化配置，避免在每个
# target_link_libraries() 中重复添加 fmt::fmt。
#
# 注意：
# 对于大型项目或可复用库项目，推荐在具体 target 上使用
# target_link_libraries() 显式声明依赖关系。
link_libraries(fmt::fmt)

# 添加 cppzmq 库（注意：它会自动依赖 libzmq，需要系统已安装 libzmq3-dev）
FetchContent_Declare(
    cppzmq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    GIT_TAG v4.11.0   # 或者用最新稳定版本
)

# 关闭 cppzmq 自带测试，避免因为 Catch2 等测试依赖导致构建失败
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "Build cppzmq tests" FORCE)
FetchContent_MakeAvailable(cppzmq)

add_subdirectory(basic_reqrep)
add_subdirectory(basic_pushpull)
add_subdirectory(basic_pubsub)
