#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <boost/asio.hpp>

#include "UdpPeer.hpp"

class UdpServer : public UdpPeer {
 public:
  UdpServer(boost::asio::io_context &ioContext, UdpPeer::Observer &observer);
  bool bind(uint16_t port);

 private:
  boost::asio::ip::udp::endpoint m_endpoint;
};

#endif
