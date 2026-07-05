#include <fmt/format.h>

#include <string>
#include <zmq.hpp>

int main()
{
  zmq::context_t context(1);
  zmq::socket_t router(context, zmq::socket_type::router);

  const std::string endpoint = "ipc:///tmp/router_dealer.ipc";
  router.bind(endpoint);
  fmt::print("[router] bind {}\n", endpoint);

  while (true)
  {
    zmq::message_t identity;
    zmq::message_t request;

    // ROUTER 从 DEALER 接收两帧消息：身份帧 + 内容帧
    auto ok = router.recv(identity, zmq::recv_flags::none);
    if (!ok) continue;

    ok = router.recv(request, zmq::recv_flags::none);
    if (!ok) continue;

    std::string client_id(identity.to_string());
    std::string msg(request.to_string());

    fmt::print("[router] recv from {}: {}\n", client_id, msg);

    std::string reply = fmt::format("reply to {}: {}", client_id, msg);

    router.send(identity, zmq::send_flags::sndmore);
    router.send(zmq::buffer(reply), zmq::send_flags::none);

    fmt::print("[router] send to {}: {}\n", client_id, reply);
  }

  return EXIT_SUCCESS;
}
