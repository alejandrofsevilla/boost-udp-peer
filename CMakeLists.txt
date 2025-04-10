cmake_minimum_required(VERSION 3.22.0)

project(boost-udp-peer LANGUAGES CXX)

add_subdirectory(tests)

add_library(${PROJECT_NAME} SHARED
  UdpPeer.hpp
  UdpPeer.cpp
  )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -O0)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()

