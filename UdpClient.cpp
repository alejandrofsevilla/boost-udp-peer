#include "UdpClient.hpp"

UdpClient::UdpClient(boost::asio::io_context &ioContext, Observer &observer)
    : UdpPeer{ioContext, observer} {}
