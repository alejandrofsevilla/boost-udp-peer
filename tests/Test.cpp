#include <gtest/gtest.h>

#include <UdpClient.hpp>
#include <UdpServer.hpp>
#include <random>
#include <thread>

namespace {
inline std::string generateRandomString(size_t size) {
  std::random_device rd;
  std::mt19937 eng{rd()};
  std::uniform_int_distribution<char> dist;
  std::string str(size, {});
  std::generate(str.begin(), str.end(), std::bind(dist, eng));
  return str;
}
}  // namespace

TEST(UdpTest, UdpClientOpensSocket) {
  boost::asio::io_context context;
  UdpClient::Observer observer;
  UdpClient client{context, observer};
  EXPECT_EQ(client.openSocket(boost::asio::ip::udp::v4()), true);
}

TEST(UdpTest, UdpServerBinds) {
  boost::asio::io_context context;
  UdpServer::Observer observer;
  UdpServer server{context, observer};
  EXPECT_EQ(server.openSocket(boost::asio::ip::udp::v4()), true);
  EXPECT_EQ(server.bind(1234), true);
}

TEST(UdpTest, UdpClientSends) {
  constexpr uint16_t port{1234};
  constexpr size_t messageSize{1000};
  const auto protocol{boost::asio::ip::udp::v4()};
  boost::asio::io_context context;
  struct : UdpServer::Observer {
    std::string msg{};
    void onReceivedFrom(const char *data, size_t size,
                        const boost::asio::ip::udp::endpoint &) {
      msg.append(data, size);
    };
  } serverObserver;
  UdpServer server{context, serverObserver};
  server.openSocket(protocol);
  server.bind(port);
  UdpClient::Observer clientObserver;
  UdpClient client{context, clientObserver};
  client.openSocket(protocol);
  server.startReceiving();
  std::thread thread([&context]() { context.run(); });
  auto msg{generateRandomString(messageSize)};
  client.sendTo(msg.data(), msg.size(), {protocol, port});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(serverObserver.msg, msg);
  context.stop();
  thread.join();
}

TEST(UdpTest, UdpServerSends) {
  constexpr uint16_t port{1234};
  constexpr size_t messageSize{1000};
  const auto protocol{boost::asio::ip::udp::v4()};
  boost::asio::io_context context;
  struct : UdpServer::Observer {
    boost::asio::ip::udp::endpoint endpoint;
    void onReceivedFrom(const char *, size_t,
                        const boost::asio::ip::udp::endpoint &e) {
      endpoint = e;
    };
  } serverObserver;
  UdpServer server{context, serverObserver};
  server.openSocket(protocol);
  server.bind(port);
  server.startReceiving();
  struct : UdpClient::Observer {
    std::string msg{};
    void onReceivedFrom(const char *data, size_t size,
                        const boost::asio::ip::udp::endpoint &) {
      msg.append(data, size);
    };
  } clientObserver;
  UdpClient client{context, clientObserver};
  client.openSocket(protocol);
  client.startReceiving();
  std::thread thread([&context]() { context.run(); });
  client.sendTo({}, 0, {protocol, port});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto msg{generateRandomString(messageSize)};
  server.sendTo(msg.data(), msg.size(), serverObserver.endpoint);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(clientObserver.msg, msg);
  context.stop();
  thread.join();
}
