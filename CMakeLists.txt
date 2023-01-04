cmake_minimum_required(VERSION 3.22.0)

project(udp-server-client LANGUAGES CXX)

add_subdirectory(tests)

add_library(udp-server-client SHARED
  UdpClient.hpp
  UdpClient.cpp
  UdpPeer.hpp
  UdpPeer.cpp
  UdpServer.hpp
  UdpServer.cpp
  )

target_compile_features(udp-server-client PRIVATE cxx_std_17)

target_compile_options(udp-server-client PRIVATE -Wall -Wextra -Wpedantic -Werror -O0)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
  target_include_directories(udp-server-client PUBLIC ${Boost_INCLUDE_DIRS})
endif()

