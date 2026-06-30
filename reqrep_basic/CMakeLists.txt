project(reqrep_basic LANGUAGES CXX)

add_executable(helloword helloword.cpp)
add_executable(reqrep_server reqrep_server.cpp)
add_executable(reqrep_client reqrep_client.cpp)

target_link_libraries(reqrep_client PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)

# 显式链接需要的库
target_link_libraries(reqrep_server PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)