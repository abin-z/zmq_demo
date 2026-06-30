#include <iostream>
#include <string>
#include <zmq.hpp>

int main()
{
  zmq::context_t context(1);
  zmq::socket_t puller(context, zmq::socket_type::pull);

  // 连接到 IPC 地址（注意是 connect，不是 bind）
  puller.connect("ipc:///tmp/push_pull.ipc");

  std::cout << "PULL 端已启动，连接到 ipc:///tmp/push_pull.ipc" << std::endl;
  std::cout << "等待接收消息..." << std::endl;

  while (true)
  {
    zmq::message_t message;
    if (!puller.recv(message, zmq::recv_flags::none))
    {
      std::cerr << "接收消息失败" << std::endl;
      break;
    }

    std::string msg(static_cast<char *>(message.data()), message.size());
    std::cout << "收到: " << msg << " (大小: " << message.size() << " 字节)" << std::endl;
  }

  return 0;
}