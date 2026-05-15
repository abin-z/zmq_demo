#include <iostream>
#include <string>
#include <zmq.hpp>

int main()
{
  zmq::context_t context{1};
  zmq::socket_t socket{context, zmq::socket_type::req};
  socket.connect("tcp://127.0.0.1:5555");

  const std::string msg = "hello";
  socket.send(zmq::buffer(msg), zmq::send_flags::none);

  zmq::message_t reply;
  if (!socket.recv(reply, zmq::recv_flags::none))
  {
    return 1;
  }

  const std::string reply_text(static_cast<char *>(reply.data()), reply.size());
  std::cout << "回复: " << reply_text << "\n";
  return 0;
}
