project(sdr_emulator)

find_package(GTest REQUIRED)

enable_testing()
add_executable(emulator_test test/test.cpp)
target_sources(emulator_test PUBLIC src/sdr_emulator/sdr_emulator.cpp include/sdr_emulator/sdr_emulator.h)
target_link_libraries(emulator_test PRIVATE pthread sdr_rf_interface transfer_interface GTest::gtest GTest::gmock GTest::gtest_main)
target_include_directories(emulator_test PUBLIC include/sdr_emulator/)
add_test(NAME emulator_test COMMAND emulator_test)

