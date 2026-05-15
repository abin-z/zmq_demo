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

------

### 3. 示例

客户端：

```cpp
zmq::socket_t socket(ctx, zmq::socket_type::req);

socket.connect("tcp://127.0.0.1:5555");

socket.send(zmq::buffer("hello"));

socket.recv(reply);
```

服务端：

```cpp
zmq::socket_t socket(ctx, zmq::socket_type::rep);

socket.bind("tcp://*:5555");

socket.recv(req);

socket.send(zmq::buffer("world"));
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
