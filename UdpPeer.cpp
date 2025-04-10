#include "UdpPeer.hpp"

#include <iostream>

namespace {
constexpr size_t f_messageMaxSize = 65535;
} // namespace

UdpPeer::UdpPeer(boost::asio::io_context &ioContext, Observer &observer)
    : m_socket{ioContext}, m_remoteEndpoint{}, m_receiveBuffer{},
      m_sendBuffer{}, m_sendDataInfo{}, m_sendMutex{}, m_observer{observer},
      m_isReceiving{false}, m_isSending{false} {}

void UdpPeer::Observer::onReceivedFrom(const char *, size_t,
                                       const boost::asio::ip::udp::endpoint &) {
}

bool UdpPeer::openSocket(const boost::asio::ip::udp &protocol) {
  boost::system::error_code error;
  m_socket.open(protocol, error);
  if (error) {
    std::cerr << "UDPPeer::openSocket error: " << error.message() << std::endl;
    return false;
  }
  return true;
}

bool UdpPeer::bind(uint16_t port) {
  boost::system::error_code error;
  m_socket.bind({m_socket.local_endpoint(error).protocol(), port}, error);
  if (error) {
    std::cerr << "UDPPeer::bind error: " << error.message() << std::endl;
    return false;
  }
  return true;
}

void UdpPeer::startReceiving() {
  if (m_isReceiving) {
    return;
  }
  doReceive();
}

void UdpPeer::sendTo(const char *data, size_t size,
                     const boost::asio::ip::udp::endpoint &endpoint) {
  std::lock_guard<std::mutex> guard{m_sendMutex};
  std::ostream bufferStream{&m_sendBuffer};
  bufferStream.write(data, size);
  m_sendDataInfo.push_back({endpoint, size});
  if (!m_isSending) {
    doSend();
  }
}

void UdpPeer::closeSocket() {
  boost::system::error_code error;
  m_socket.close(error);
  if (error) {
    std::cerr << "UdpPeer::closeSocket() error: " << error.message()
              << std::endl;
  }
  m_isReceiving = false;
  m_isSending = false;
  return;
}

void UdpPeer::doSend() {
  m_isSending = true;
  auto dataInfo = m_sendDataInfo.front();
  auto endpoint = dataInfo.first;
  auto size = dataInfo.second;
  auto buffer = boost::asio::buffer(m_sendBuffer.data(), size);
  m_socket.async_send_to(
      buffer, endpoint, [this, size](const auto &error, auto) {
        if (error) {
          std::cout << "UdpPeer::doSend() error: " << error.message()
                    << std::endl;
        }
        std::lock_guard<std::mutex> guard{m_sendMutex};
        m_sendBuffer.consume(size);
        m_sendDataInfo.pop_front();
        if (m_sendDataInfo.size() == 0) {
          m_isSending = false;
          return;
        }
        doSend();
      });
}

void UdpPeer::doReceive() {
  m_isReceiving = true;
  auto buffers = m_receiveBuffer.prepare(f_messageMaxSize);
  m_socket.async_receive_from(
      buffers, m_remoteEndpoint,
      [this](const auto &error, auto bytesTransferred) {
        if (error) {
          std::cout << "UdpPeer::doReceive() error: " << error.message()
                    << std::endl;
          return closeSocket();
        }
        m_receiveBuffer.commit(bytesTransferred);
        m_observer.onReceivedFrom(
            static_cast<const char *>(m_receiveBuffer.data().data()),
            bytesTransferred, m_remoteEndpoint);
        m_receiveBuffer.consume(bytesTransferred);
        doReceive();
      });
}
