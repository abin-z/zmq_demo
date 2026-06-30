project(pushpull_basic LANGUAGES CXX)

add_executable(pusher pusher.cpp)
add_executable(puller puller.cpp)

target_link_libraries(pusher PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)

# 显式链接需要的库
target_link_libraries(puller PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)