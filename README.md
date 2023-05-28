# UDP Server/Client
Asynchronous UDP Server and Client using [Boost.Asio](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html).
## Requirements
- C++ 17
- CMake 3.22.0
- Boost 1.74.0
- GoogleTest 1.11.0
## Example
### Server
```cpp
#include <UdpServer.hpp>
#include <thread>
#include <iostream>
\\...
struct : UdpServer::Observer {
  void onReceivedFrom(const char *data, size_t size,
                      const boost::asio::ip::udp::endpoint &endpoint) {
    std::cout << "data received from endpoint address "
              << endpoint.address().to_string() << ": ";
    std::cout.write(data, size);
    std::cout << std::endl;
  };
} observer;

boost::asio::io_context context;
std::thread thread([&context]() { context.run(); });

UdpServer server{context, observer};

server.openSocket(boost::asio::ip::udp::v4());
server.bind(1234);
server.startReceiving();
\\...
```
### Client
```cpp
#include <UdpClient.hpp>
#include <thread>
#include <iostream>
\\...
struct : UdpClient::Observer {
  void onReceivedFrom(const char *data, size_t size,
                      const boost::asio::ip::udp::endpoint &endpoint) {
    std::cout << "data received from endpoint address "
              << endpoint.address().to_string() << ": ";
    std::cout.write(data, size);
    std::cout << std::endl;
  };
} observer;

boost::asio::io_context context;
std::thread thread([&context]() { context.run(); });

UdpClient client{context, observer};

client.openSocket(boost::asio::ip::udp::v4());
client.startReceiving();
\\...
```
