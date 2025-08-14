#include <gtest/gtest.h>

#include <UdpPeer.hpp>
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
} // namespace

TEST(UdpTest, OpenSocket) {
  boost::asio::io_context context;
  UdpPeer::Observer observer;
  UdpPeer peer{context, observer};
  EXPECT_EQ(peer.openSocket(boost::asio::ip::udp::v4()), true);
}

TEST(UdpTest, Bind) {
  boost::asio::io_context context;
  UdpPeer::Observer observer;
  UdpPeer peer{context, observer};
  EXPECT_EQ(peer.openSocket(boost::asio::ip::udp::v4()), true);
  EXPECT_EQ(peer.bind(1234), true);
}

TEST(UdpTest, SendAndReceive) {
  //constexpr uint16_t senderPort{1234};
  constexpr uint16_t receiverPort{5678};
  constexpr size_t messageSize{1000};
  const auto protocol{boost::asio::ip::udp::v4()};
  boost::asio::io_context context;
  struct : UdpPeer::Observer {
    std::string msg{};
    void onReceivedFrom(const char *data, size_t size,
                        const boost::asio::ip::udp::endpoint &) {
      msg.append(data, size);
    };
  } receiverObserver;
  UdpPeer receiver{context, receiverObserver};
  receiver.openSocket(protocol);
  receiver.bind(receiverPort);
  receiver.startReceiving();
  UdpPeer::Observer senderObserver;
  UdpPeer sender{context, senderObserver};
  sender.openSocket(protocol);
  //sender.bind(senderPort);
  std::thread thread([&context]() { context.run(); });
  auto msg{generateRandomString(messageSize)};
  sender.sendTo(msg.data(), msg.size(), {protocol, receiverPort});
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(receiverObserver.msg, msg);
  context.stop();
  thread.join();
}
