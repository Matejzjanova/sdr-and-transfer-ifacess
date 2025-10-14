project(rtlsdr_control)
find_package(GTest REQUIRED)
find_package(rtlsdr REQUIRED)
find_package(Threads REQUIRED)
enable_testing()

add_executable(rtlsdr_test test/test.cpp)

target_sources(rtlsdr_test PUBLIC src/rtlsdr_control/rtlsdr_control.cpp
        include/rtlsdr_control/rtlsdr_control.h)

target_link_libraries(rtlsdr_test PRIVATE rtlsdr Threads::Threads  pthread
        sdr_rf_interface transfer_interface GTest::gtest GTest::gmock GTest::gtest_main )

target_include_directories(rtlsdr_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/rtlsdr_control ) #${CMAKE_CURRENT_SOURCE_DIR}/../extern/rtlsdr/include)

add_test(NAME rtlsdr_test COMMAND rtlsdr_test)
