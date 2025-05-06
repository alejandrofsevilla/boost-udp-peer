# [![LinuxBuildWorkflow](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuild.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuild.yml?event=push) [![TestsWorkflow](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuildAndTest.yml/badge.svg)](https://github.com/alejandrofsevilla/boost-udp-server-client/actions/workflows/LinuxBuildAndTest.yml?event=push)
# Boost UDP Peer
Asynchronous [Boost.Asio](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html) UDP Sender/Receiver Peer example.
## Requirements
- C++ 17 compiler
- CMake 3.22.0
- Boost 1.74.0
- GoogleTest 1.11.0
## Example
```cpp

boost::asio::io_context context;
std::thread thread([&context]() { context.run(); });

struct : UdpPeer::Observer {
  void onReceivedFrom(const char *data, size_t size,
                      const boost::asio::ip::udp::endpoint &endpoint) {
    std::cout << "data received from endpoint address "
              << endpoint.address().to_string() << ": ";
    std::cout.write(data, size);
    std::cout << std::endl;
  };
} observer;

auto protocol{boost::asio::ip::udp::v4()};
uint16_t receiverPort{1234};
boost::asio::ip::udp::endpoint receiverEndpoint{protocol, receiverPort};

UdpPeer receiver{context, observer};
receiver.openSocket();
receiver.bind(receiverPort);
receiver.startReceiving();

UdpPeer sender{context, observer};
sender.openSocket(boost::asio::ip::udp::v4());
std::string msg("example");
sender.sendTo(msg.data(), msg.size(), receiverEndpoint);

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
   git clone https://github.com/alejandrofsevilla/boost-udp-peer.git
   cd boost-udp-peer
   ```
- Build.
   ```terminal
   cmake -S . -B build
   cmake --build build
   ```
- Run tests.
   ```terminal
   ./build/tests/boost-udp-peer-tests 
   ```
