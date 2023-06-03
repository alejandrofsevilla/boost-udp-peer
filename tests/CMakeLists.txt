add_executable(boost-udp-server-client-tests
  Test.cpp)

target_link_libraries(boost-udp-server-client-tests PRIVATE boost-udp-server-client)

target_compile_features(boost-udp-server-client-tests PRIVATE cxx_std_17)

target_include_directories(boost-udp-server-client-tests PUBLIC ..)

find_package(GTest 1.11.0 REQUIRED)
if (GTest_FOUND)
  target_link_libraries(boost-udp-server-client-tests PRIVATE GTest::gtest GTest::gtest_main)
endif()
