#ifndef UDP_PEER_HPP
#define UDP_PEER_HPP

#include <boost/asio.hpp>
#include <deque>
#include <utility>

class UdpPeer {
 public:
  struct Observer {
    virtual void onReceivedFrom(const char *data, size_t size,
                                const boost::asio::ip::udp::endpoint &endpoint);
  };
  bool openSocket(const boost::asio::ip::udp &protocol);
  void startReceiving();
  void sendTo(const char *data, size_t size,
              const boost::asio::ip::udp::endpoint &endpoint);
  void closeSocket();

 protected:
  UdpPeer(boost::asio::io_context &ioContext, Observer &observer);
  boost::asio::ip::udp::socket m_socket;

 private:
  void doSend();
  void doReceive();

  boost::asio::io_context &m_ioContext;
  boost::asio::ip::udp::endpoint m_remoteEndpoint;
  boost::asio::streambuf m_receiveBuffer;
  boost::asio::streambuf m_sendBuffer;
  std::deque<std::pair<boost::asio::ip::udp::endpoint, size_t>> m_sendDataInfo;
  std::mutex m_sendMutex;
  Observer &m_observer;
  bool m_isReceiving;
  bool m_isSending;
};

#endif
