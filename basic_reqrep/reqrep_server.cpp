#include <unistd.h>  // for unlink()

#include <cstdio>
#include <string>

#include <fmt/format.h>
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

  fmt::print("REP 已绑定 {}，等待请求...\n", ipc_path);
  fmt::print("提示: 请确保客户端使用相同的 IPC 地址连接\n");

  while (true)
  {
    zmq::message_t request;
    if (!socket.recv(request, zmq::recv_flags::none))
    {
      fmt::print(stderr, "接收消息失败\n");
      return 1;
    }

    const std::string req_text(static_cast<char *>(request.data()), request.size());
    fmt::print("收到: {} (大小: {} 字节)\n", req_text, request.size());

    const std::string reply = "world";
    socket.send(zmq::buffer(reply), zmq::send_flags::none);
    fmt::print("回复: {} 已发送\n", reply);
  }
}