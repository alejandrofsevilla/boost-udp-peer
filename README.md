# [![LinuxBuildWorkflow](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuild.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuild.yml?event=push) [![TestsWorkflow](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuildAndTest.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuildAndTest.yml?event=push)
# Boost UDP Server/Client
Asynchronous [Boost.Asio](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html) UDP Server and Client example.
## Requirements
- C++ 17
- CMake 3.22.0
- Boost 1.74.0
- GoogleTest 1.11.0
## Usage
### Server
```cpp
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
```
### Client
```cpp
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
```
## Build
- Install dependencies.
  - linux 
   ```terminal
   sudo apt-get install libboost-dev
   sudo apt-get install libgtest-dev
   ```
  - macOs
   ```terminal
   brew install boost
   brew install googletest
   ```
- Clone repository.
   ```terminal
   git clone https://github.com/alejandrofsevilla/boost-udp-server-client.git
   cd boost-udp-server-client
   ```
- Build.
   ```terminal
   cmake -S . -B build
   cmake --build build
   ```
- Run tests.
   ```terminal
   ./build/tests/boost-udp-server-client-tests 
   ```
## Implementation
```mermaid
classDiagram
    class C_0015175123961717550173["UdpPeer"]
    class C_0015175123961717550173 {
        +closeSocket() : void
        -doReceive() : void
        -doSend() : void
        +openSocket(const boost::asio::ip::udp & protocol) : bool
        +sendTo(const char * data, size_t size, const boost::asio::ip::udp::endpoint & endpoint) : void
        +startReceiving() : void
        -m_isReceiving : bool
        -m_isSending : bool
        -m_receiveBuffer : boost::asio::streambuf
        -m_remoteEndpoint : boost::asio::ip::udp::endpoint
        -m_sendBuffer : boost::asio::streambuf
        -m_sendDataInfo : std::deque&lt;std::pair&lt;boost::asio::ip::udp::endpoint,size_t&gt;&gt;
        -m_sendMutex : std::mutex
        #m_socket : boost::asio::ip::udp::socket
    }
    class C_0017118870456923537943["UdpPeer::Observer"]
    class C_0017118870456923537943 {
        +onReceivedFrom(const char * data, size_t size, const boost::asio::ip::udp::endpoint & endpoint) : void
    }
    class C_0006076180002494955007["UdpServer"]
    class C_0006076180002494955007 {
        +bind(uint16_t port) : bool
        -m_endpoint : boost::asio::ip::udp::endpoint
    }
    class C_0015224344871708291566["UdpClient"]
    class C_0015224344871708291566 {
    }
    C_0015175123961717550173 --> C_0017118870456923537943 : -m_observer
    C_0015175123961717550173 ()-- C_0017118870456923537943 : 
    C_0006076180002494955007 ..> C_0017118870456923537943 : 
    C_0015175123961717550173 <|-- C_0006076180002494955007 : 
    C_0015224344871708291566 ..> C_0017118870456923537943 : 
    C_0015175123961717550173 <|-- C_0015224344871708291566 : 

%% Generated with clang-uml, version 0.6.0
%% LLVM version Ubuntu clang version 15.0.7

```
