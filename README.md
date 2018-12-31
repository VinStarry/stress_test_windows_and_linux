# stress_test_windows_and_linux
Simple stress testing tool on Windows and Linux. Windows和Linux平台下的简单压力测试工具。

轻量级的，比较好用，改写起来也很简单。



## Windows

在Visual Studio 2017上进行的开发。  

已经进行了Release，可以直接运行 **Release/client_test.exe** 

运行参数的配置文件可以通过设置 **Release/server.config** 

server.config的格式如下  

``` 
server_ip = 192.168.9.205

server_port = 7000

thread_number = 200

tasks_per_thread = 100
```

**server_ip** : 服务器IP地址  

**server_port** : 服务器端口  

**thread_number** : 同时访问的线程数量

**tasks_per_thread** : 每个线程执行的任务数量  



### 注意事项

1. 由于是并发测试，建立连接并发送信息后，客户端没有主动关闭连接。
2. Windows 默认设置TCP连接是从49xxx开始到65535的，也就是说客户端在未修改参数的情况下最多同时发起16000多个连接，如果需要修改参数，请修改Windows系统配置中的**tcp port range**。
3. 正常情况下1秒钟可以完成40000~50000个连接的创建。
4. 如发生连接错误会报错，可以参照 http://winsock-error.danielclarke.com 查看错误原因。



## Linux

在Clion上进行的开发，CMake版本为3.12(可以降低)

可执行文件是 **multi_client_linux/cmake-build-debug/multi_client_linux**

配置文件是 **multi_client_linux/cmake-build-debug/network.config**

配置文件的格式同Windows下的格式。



### 注意事项

1. 由于是并发测试，建立连接并发送信息后，客户端没有主动关闭连接。
2. 在测试机上 Linux 的连接建立速度会明显慢于 Windows。 （也有可能是机器原因）

## 后续工作
由于这是一个比较简单基础的压力测试程序，后续的意义不大。  
可能会增加 **raw socket** 的选项。  
会增加**命令参数操控**的模式。

