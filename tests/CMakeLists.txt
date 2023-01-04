add_executable(udp-server-client-tests
  Test.cpp)

target_link_libraries(udp-server-client-tests PRIVATE udp-server-client)

target_compile_features(udp-server-client-tests PRIVATE cxx_std_17)

target_include_directories(udp-server-client-tests PUBLIC ..)

find_package(GTest 1.11.0 REQUIRED)
if (GTest_FOUND)
  target_link_libraries(udp-server-client-tests PRIVATE GTest::gtest GTest::gtest_main)
endif()
