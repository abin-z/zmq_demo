project(publish_subscribe LANGUAGES CXX)

add_executable(publisher publisher.cpp)
add_executable(subscriber subscriber.cpp)

target_link_libraries(publisher PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)

# 显式链接需要的库
target_link_libraries(subscriber PRIVATE
    fmt::fmt
    cppzmq   # 这个会自动链接 libzmq
)