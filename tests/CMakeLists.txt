cmake_minimum_required(VERSION 3.22.0)

project(boost-udp-peer-tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
  Test.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE boost-udp-peer)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ..)

find_package(GTest 1.11.0 REQUIRED)
if (GTest_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main)
endif()