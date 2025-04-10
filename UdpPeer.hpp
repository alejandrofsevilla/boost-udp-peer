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

  UdpPeer(boost::asio::io_context &ioContext, Observer &observer);

  bool openSocket(const boost::asio::ip::udp &protocol);
  bool bind(uint16_t port);
  void startReceiving();
  void sendTo(const char *data, size_t size,
              const boost::asio::ip::udp::endpoint &endpoint);
  void closeSocket();

protected:
private:
  boost::asio::ip::udp::socket m_socket;
  boost::asio::ip::udp::endpoint m_remoteEndpoint;
  boost::asio::streambuf m_receiveBuffer;
  boost::asio::streambuf m_sendBuffer;
  std::deque<std::pair<boost::asio::ip::udp::endpoint, size_t>> m_sendDataInfo;
  std::mutex m_sendMutex;
  Observer &m_observer;
  bool m_isReceiving;
  bool m_isSending;

  void doSend();
  void doReceive();
};

#endif
