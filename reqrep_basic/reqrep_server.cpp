#include <iostream>
#include <string>
#include <zmq.hpp>

int main()
{
  zmq::context_t context{1};
  zmq::socket_t socket{context, zmq::socket_type::rep};
  socket.bind("tcp://*:5555");

  std::cout << "REP 已绑定 tcp://*:5555，等待请求...\n";

  while (true)
  {
    zmq::message_t request;
    if (!socket.recv(request, zmq::recv_flags::none))
    {
      return 1;
    }

    const std::string req_text(static_cast<char *>(request.data()), request.size());
    std::cout << "收到: " << req_text << "\n";

    const std::string reply = "world";
    socket.send(zmq::buffer(reply), zmq::send_flags::none);
  }
}
