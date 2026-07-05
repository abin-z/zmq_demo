project(router_dealer LANGUAGES CXX)

add_executable(router router.cpp)
add_executable(dealer dealer.cpp)

target_link_libraries(router PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)

# 显式链接需要的库
target_link_libraries(dealer PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)