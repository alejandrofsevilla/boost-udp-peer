#include "UdpServer.hpp"

#include <iostream>

UdpServer::UdpServer(boost::asio::io_context &ioContext,
                     UdpPeer::Observer &observer)
    : UdpPeer{ioContext, observer} {}

bool UdpServer::bind(uint16_t port) {
  boost::system::error_code error;
  m_socket.bind({m_socket.local_endpoint(error).protocol(), port}, error);
  if (error) {
    std::cerr << "UDP UdpServer Socket Bind error: " << error.message()
              << std::endl;
    return false;
  }
  return true;
}
