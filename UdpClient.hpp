#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include "UdpPeer.hpp"

class UdpClient : public UdpPeer {
 public:
  UdpClient(boost::asio::io_context &ioContext, Observer &observer);
};

#endif
