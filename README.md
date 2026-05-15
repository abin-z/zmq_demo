# ZeroMQ的基础案例

### ZeroMQ 是一个：

高性能消息通信库, 类似 socket，但比 socket 高层
不需要 broker（不像 Kafka / RabbitMQ）支持：

- TCP
- IPC
- inproc（进程内）
- multicast

你可以理解成：

“Socket + 消息队列 + 网络模式封装”

它不是传统 MQ 服务，而是：

一个嵌入式通信库
链接进你的 C++ 程序即可


### 它解决什么问题

传统 socket 最大的问题：
```
recv/send 太底层
连接管理麻烦
重连麻烦
线程模型复杂
消息边界不存在
异步困难
```
ZeroMQ 帮你做了：
```
自动重连
消息边界
异步队列
负载均衡
发布订阅
请求响应
线程间通信
```


### ZeroMQ 四大核心通信模型

ZeroMQ 是一个高性能异步消息通信库。

与传统 Socket 编程不同，ZeroMQ 更强调：

**通信模式（Messaging Pattern）**`

而不是：

**send / recv 细节**

ZeroMQ 内置了多种通信模型，其中最核心的是：

**REQ / REP（请求响应）**

**PUB / SUB（发布订阅）**

**PUSH / PULL（任务分发）**

**ROUTER / DEALER（高级异步路由）**

---

### 四种模型对比

| 模型          | 特点         | 是否同步 | 适合场景   |
| ------------- | ------------ | -------- | ---------- |
| REQ/REP       | 请求响应     | 同步     | RPC        |
| PUB/SUB       | 消息广播     | 异步     | 状态推送   |
| PUSH/PULL     | 任务分发     | 异步     | 并行处理   |
| ROUTER/DEALER | 高级异步路由 | 异步     | 高并发系统 |



# 一、REQ / REP（请求-响应模型）

这是最基础、最容易理解的模式。

### 1. 通信结构

```txt
Client (REQ)  ----request---->  Server (REP)

Client (REQ)  <---response---- Server (REP)
```

REQ：

- Request
- 请求端

REP：

- Reply
- 响应端

### 2. 工作方式

REQ socket：

```
send
recv
send
recv
```

必须严格交替。

REP socket：

```
recv
send
recv
send
```

也必须严格交替。

这是它的重要特点。

### 3. 示例

客户端：

```cpp
#include <zmq.hpp>
#include <iostream>

int main()
{
    zmq::context_t context(1);

    zmq::socket_t socket(context, zmq::socket_type::req);

    socket.connect("tcp://localhost:5555");

    socket.send(
        zmq::buffer("hello"),
        zmq::send_flags::none);

    zmq::message_t reply;

    socket.recv(reply);

    std::cout << reply.to_string()
              << std::endl;
}
```

服务端：

```cpp
#include <zmq.hpp>
#include <iostream>

int main()
{
    zmq::context_t context(1);

    zmq::socket_t socket(context, zmq::socket_type::rep);

    socket.bind("tcp://*:5555");

    while (true)
    {
        zmq::message_t request;

        socket.recv(request);

        std::cout << "recv: "
                  << request.to_string()
                  << std::endl;

        socket.send(
            zmq::buffer("world"),
            zmq::send_flags::none);
    }
}
```

------

### 4. 本质思想

REQ/REP 本质上是：

```
同步 RPC 模型
```

非常像：

- HTTP 请求
- RPC 调用
- REST API

------

### 5. 优点

**简单最容易上手。**

**自动管理连接**

不用：

- accept
- reconnect

**有消息边界**

不像 TCP 会粘包。

### 6. 缺点

**强同步**

REQ：

```
必须 send 后 recv
```

不能连续发送。

否则：

```
Operation cannot be accomplished in current state
```

------

**不适合高并发异步**

因为它是：

```
阻塞式请求响应思维
```

### 7. 适用场景

适合：

- RPC
- 配置查询
- 控制指令
- SDK 请求接口

例如：

```
机器人控制器 -> 请求状态
服务器 -> 返回结果
```

------

# 二、PUB / SUB（发布订阅模型）

这是 ZeroMQ 最经典的模式之一。

### 1. 通信结构

```
           ---> Subscriber1
Publisher
           ---> Subscriber2
           ---> Subscriber3
```

PUB：

- 发布者

SUB：

- 订阅者

### 2. 本质思想

本质：

```
广播模型
```

类似：

- ROS topic
- 消息总线
- 新闻广播

### 3. 示例

发布端：

```cpp
zmq::socket_t pub(ctx, zmq::socket_type::pub);

pub.bind("tcp://*:5556");

while (true)
{
    pub.send(zmq::buffer("temperature 25"));
}
```

订阅端：

```cpp
zmq::socket_t sub(ctx, zmq::socket_type::sub);

sub.connect("tcp://localhost:5556");

sub.set(zmq::sockopt::subscribe, "temperature");
```

------

### 4. Topic 过滤机制

这是重点。

订阅：

```
subscribe = "temperature"
```

意味着：

```
只接收以 temperature 开头的消息
```

因此：

```
temperature 25
temperature 30
```

会收到。

而：

```
humidity 50
```

不会收到。

------

### 5. 特点

------

**一对多广播**

天然支持：

```
1 -> N
```

**发布者不知道订阅者是谁**

这是：

```
解耦
```

的核心。

**高吞吐**

因为：

- 无需等待回复
- 无需同步

### 6. 缺点

**订阅者可能丢消息**

PUB/SUB 默认：

```
不保证可靠送达
```

因为它更偏：

```
实时流
```

而不是：

```
可靠事务
```

------

## Late Joiner Problem

后加入的订阅者：

```
收不到历史消息
```

因为它不是 Kafka。

### 7. 适用场景

非常适合：

**机器人状态广播**

```
激光雷达数据
IMU数据
位置数据
```

**日志系统**

```
服务端 -> 多个日志客户端
```

**行情系统**

```
股票价格广播
```



# 三、PUSH / PULL（任务分发模型）

PUSH / PULL 本质上是：

```
生产者 - 消费者模型
```

适用于：

- 任务队列
- 并行计算
- 图像处理
- AI 推理
- 视频处理流水线

------

### 1. 通信结构

```
Producer (PUSH)
      |
      +----> Worker1 (PULL)
      +----> Worker2 (PULL)
      +----> Worker3 (PULL)
```

------

### 2. 工作原理

PUSH Socket：

```
负责分发任务
```

PULL Socket：

```
负责接收任务
```

ZeroMQ 内部会自动进行：

```
负载均衡（Round Robin）
```

例如：

```
task1 -> worker1
task2 -> worker2
task3 -> worker3
```

------

### 3. C++ 示例

**生产者**

```cpp
#include <zmq.hpp>

int main()
{
    zmq::context_t context(1);

    zmq::socket_t push(context, zmq::socket_type::push);

    push.bind("tcp://*:5557");

    while (true)
    {
        push.send(
            zmq::buffer("task"),
            zmq::send_flags::none);
    }
}
```

**消费者**

```cpp
#include <zmq.hpp>
#include <iostream>

int main()
{
    zmq::context_t context(1);

    zmq::socket_t pull(context, zmq::socket_type::pull);

    pull.connect("tcp://localhost:5557");

    while (true)
    {
        zmq::message_t msg;

        pull.recv(msg);

        std::cout << msg.to_string()
                  << std::endl;
    }
}
```

------

### 4. 优点

**自动负载均衡**

无需自己实现：

- 调度器
- 任务队列
- Worker 管理

**易于扩容**

新增 Worker：

```
直接 connect 即可
```

------

**非常适合并行任务**

例如：

- 图像识别
- 视频转码
- 数据处理

### 5. 缺点

**单向通信**

PUSH/PULL：

```
不支持返回结果
```

因此不适合 RPC。

**Worker 崩溃可能导致任务丢失**

默认情况下：

```
不保证任务可靠性
```

# 四、ROUTER / DEALER（高级异步模型）

ROUTER / DEALER 是 ZeroMQ 最核心、最强大的通信模式。

它解决了：

```
REQ/REP 同步阻塞
```

的问题。

适用于：

- 高并发服务器
- 异步 RPC
- 聊天系统
- 网关系统
- 分布式系统

### 1. Dealer 是什么

Dealer 可以理解为：

```
异步版 REQ
```

与 REQ 不同：

Dealer 可以：

```cpp
send();
send();
send();
```

连续发送消息，而无需等待回复。

### 2. Router 是什么

Router 可以理解为：

```
带路由能力的异步服务端
```

它能够识别：

```
每一个客户端
```

因为每个连接都具有：

```
identity
```

### 3. 通信结构

```
Dealer1 \
Dealer2  ---> Router
Dealer3 /
```

Router 可以知道：

```
消息来自谁
```

并且可以：

```
定向回复
```

### 4. Router 收到的数据结构

Router 收到的消息：

```
[identity][message]
```

其中：

```
identity
```

表示客户端 ID。

因此：

```
Router 可以实现消息路由
```

### 5. 最大特点

**完全异步**

无需：

```
send -> recv
```

严格同步。

------

**支持高并发**

非常适合：

- Reactor 模型
- 高性能服务器

------

### 支持代理架构

例如：

```
Frontend Router
        ↓
Backend Dealer
        ↓
Worker Pool
```

这已经非常接近：

```
现代微服务网关
```

的设计思想。

### 6. 典型场景

高性能 RPC

聊天服务器

消息网关

分布式任务系统
