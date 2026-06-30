#include <unistd.h>  // for unlink()

#include <iostream>
#include <string>
#include <zmq.hpp>

int main()
{
  zmq::context_t context{1};
  zmq::socket_t socket{context, zmq::socket_type::rep};

  // IPC 版本
  const std::string ipc_path = "ipc:///tmp/zmq_demo.ipc";

  // 删除旧的 IPC 文件（避免 "address already in use" 错误）
  unlink("/tmp/zmq_demo.ipc");

  socket.bind(ipc_path);

  std::cout << "REP 已绑定 " << ipc_path << "，等待请求...\n";
  std::cout << "提示: 请确保客户端使用相同的 IPC 地址连接\n";

  while (true)
  {
    zmq::message_t request;
    if (!socket.recv(request, zmq::recv_flags::none))
    {
      std::cerr << "接收消息失败\n";
      return 1;
    }

    const std::string req_text(static_cast<char *>(request.data()), request.size());
    std::cout << "收到: " << req_text << " (大小: " << request.size() << " 字节)\n";

    const std::string reply = "world";
    socket.send(zmq::buffer(reply), zmq::send_flags::none);
    std::cout << "回复: " << reply << " 已发送\n";
  }
}