# ZeroMQ的基础案例

### ZeroMQ 是一个：

高性能消息通信库
类似 socket，但比 socket 高层
不需要 broker（不像 Kafka / RabbitMQ）
支持：
TCP
IPC
inproc（进程内）
multicast

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

**通信模式（Messaging Pattern）**

而不是：

**send / recv 细节**

ZeroMQ 内置了多种通信模型，其中最核心的是：

REQ / REP（请求响应）

PUB / SUB（发布订阅）

PUSH / PULL（任务分发）

ROUTER / DEALER（高级异步路由）


