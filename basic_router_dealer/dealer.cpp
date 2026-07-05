#include <fmt/format.h>

#include <string>
#include <thread>
#include <vector>
#include <zmq.hpp>

int main()
{
  zmq::context_t context(1);
  zmq::socket_t dealer(context, zmq::socket_type::dealer);

  const std::string client_id = "dealer-1";
  dealer.set(zmq::sockopt::routing_id, client_id);

  const std::string endpoint = "ipc:///tmp/router_dealer.ipc";
  dealer.connect(endpoint);
  fmt::print("[dealer {}] connect {}\n", client_id, endpoint);

  std::vector<std::string> messages = {"hello", "world", "zmq", "router", "dealer"};

  for (int i = 0; i < static_cast<int>(messages.size()); ++i)
  {
    std::string request = fmt::format("{}-{}", i + 1, messages[i]);
    dealer.send(zmq::buffer(request), zmq::send_flags::none);
    fmt::print("[dealer {}] send: {}\n", client_id, request);

    zmq::message_t reply;
    auto ok = dealer.recv(reply, zmq::recv_flags::none);
    if (!ok)
    {
      fmt::print(stderr, "[dealer {}] recv failed\n", client_id);
      continue;
    }

    std::string reply_str(static_cast<char *>(reply.data()), reply.size());
    fmt::print("[dealer {}] recv: {}\n", client_id, reply_str);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  return EXIT_SUCCESS;
}
