#include <fmt/core.h>

#include <zmq.hpp>

int main()
{
  // ZMQ 上下文（整个进程通常只需要一个）
  zmq::context_t context{1};

  // Subscriber Socket
  zmq::socket_t subscriber{context, zmq::socket_type::sub};

  subscriber.connect("ipc:///tmp/robot_pub.ipc");

  // 订阅 weather Topic
  subscriber.set(zmq::sockopt::subscribe, "weather");

  // 订阅所有 Topic
  // subscriber.set(zmq::sockopt::subscribe, "");

  fmt::print("Subscriber started, subscribed to [weather]\n");

  try
  {
    while (true)
    {
      zmq::message_t msg;

      if (subscriber.recv(msg, zmq::recv_flags::none))
      {
        fmt::print("[SUB] {}\n", msg.to_string());
      }
    }
  }
  catch (const zmq::error_t &e)
  {
    fmt::print(stderr, "ZMQ Error: {}\n", e.what());
    return EXIT_FAILURE;
  }
  catch (const std::exception &e)
  {
    fmt::print(stderr, "Error: {}\n", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}