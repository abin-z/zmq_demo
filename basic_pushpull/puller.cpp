#include <cstdio>
#include <iostream>
#include <string>

#include <fmt/format.h>
#include <zmq.hpp>

int main()
{
  zmq::context_t context(1);
  zmq::socket_t puller(context, zmq::socket_type::pull);

  // 连接到 IPC 地址（注意是 connect，不是 bind）
  puller.connect("ipc:///tmp/push_pull.ipc");

  fmt::print("PULL 端已启动，连接到 ipc:///tmp/push_pull.ipc\n");
  fmt::print("等待接收消息...\n");

  while (true)
  {
    zmq::message_t message;
    if (!puller.recv(message, zmq::recv_flags::none))
    {
      fmt::print(stderr, "接收消息失败\n");
      break;
    }

    std::string msg(static_cast<char *>(message.data()), message.size());
    fmt::print("收到: {} (大小: {} 字节)\n", msg, message.size());
  }

  return 0;
}