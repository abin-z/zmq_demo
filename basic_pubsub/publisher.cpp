#include <fmt/format.h>

#include <chrono>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <zmq.hpp>

using namespace std::chrono_literals;

int main()
{
  // ZMQ 上下文（整个进程通常只需要一个）
  zmq::context_t context{1};

  // Publisher Socket
  zmq::socket_t publisher{context, zmq::socket_type::pub};
  publisher.bind("ipc:///tmp/robot_pub.ipc");

  fmt::print("Publisher started, listening on ipc:///tmp/robot_pub.ipc\n");

  // Topic 列表
  const std::vector<std::string> topics{"weather", "news", "sports"};

  // 随机数生成器
  std::random_device rd;
  std::mt19937 rng{rd()};

  std::uniform_int_distribution<int> message_id{1, 100};
  std::uniform_int_distribution<std::size_t> topic_index{0, topics.size() - 1};

  try
  {
    while (true)
    {
      const auto &topic = topics[topic_index(rng)];

      const std::string message = fmt::format("{} 这是来自发布者的消息 #{}", topic, message_id(rng));

      publisher.send(zmq::buffer(message), zmq::send_flags::none);

      fmt::print("[PUB] {}\n", message);

      std::this_thread::sleep_for(1s);
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