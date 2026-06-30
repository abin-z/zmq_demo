#include <unistd.h>

#include <iostream>
#include <string>

#include <fmt/format.h>
#include <zmq.hpp>

int main()
{
  zmq::context_t context(1);
  zmq::socket_t pusher(context, zmq::socket_type::push);

  // 删除旧的 IPC 文件
  unlink("/tmp/push_pull.ipc");

  // 绑定 IPC 地址
  pusher.bind("ipc:///tmp/push_pull.ipc");

  fmt::print("PUSH 端已启动，绑定到 ipc:///tmp/push_pull.ipc\n");
  fmt::print("输入消息 (输入 'q' 退出):\n");

  std::string msg;
  while (std::getline(std::cin, msg))
  {
    if (msg == "q")
    {
      break;
    }

    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    pusher.send(message, zmq::send_flags::none);

    fmt::print("发送: {}\n", msg);
  }

  return 0;
}