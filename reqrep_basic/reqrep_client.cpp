#include <iostream>
#include <string>
#include <zmq.hpp>

int main()
{
  zmq::context_t context{1};
  zmq::socket_t socket{context, zmq::socket_type::req};

  // 连接 IPC 地址（必须和服务端一致）
  socket.connect("ipc:///tmp/zmq_demo.ipc");

  std::cout << "REQ 已连接 ipc:///tmp/zmq_demo.ipc\n";
  std::cout << "输入消息 (输入 'q' 退出):\n";

  std::string msg;
  while (std::getline(std::cin, msg))
  {
    if (msg == "q") break;

    // 发送消息
    socket.send(zmq::buffer(msg), zmq::send_flags::none);
    std::cout << "发送: " << msg << "\n";

    // 接收回复
    zmq::message_t reply;
    if (socket.recv(reply, zmq::recv_flags::none))
    {
      const std::string reply_text(static_cast<char *>(reply.data()), reply.size());
      std::cout << "收到回复: " << reply_text << "\n";
    }
    else
    {
      std::cerr << "接收回复失败\n";
      break;
    }
  }

  return 0;
}