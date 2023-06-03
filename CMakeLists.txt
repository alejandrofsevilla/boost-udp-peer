cmake_minimum_required(VERSION 3.22.0)

project(boost-udp-server-client LANGUAGES CXX)

add_subdirectory(tests)

add_library(boost-udp-server-client SHARED
  UdpClient.hpp
  UdpClient.cpp
  UdpPeer.hpp
  UdpPeer.cpp
  UdpServer.hpp
  UdpServer.cpp
  )

target_compile_features(boost-udp-server-client PRIVATE cxx_std_17)

target_compile_options(boost-udp-server-client PRIVATE -Wall -Wextra -Wpedantic -Werror -O0)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
  target_include_directories(boost-udp-server-client PUBLIC ${Boost_INCLUDE_DIRS})
endif()

