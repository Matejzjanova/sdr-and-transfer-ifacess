// Created by misha on 05.10.2025.
//
#include "rtlsdr_control.h"
#include "transfer_interface/transfer.h"
#include "gtest/gtest.h"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <rtl-sdr.h>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <thread>

class TestRtlSdr : public ::testing::Test {
public:
  RtlsdrControl rtlsdr1{0};
};

TEST(RtlSdrTest, RtlSdrCanBeCreateAndDestroyed) {
  ASSERT_NO_THROW(RtlsdrControl rtlsdr1(0));
};

TEST_F(TestRtlSdr, TestSetFrequency) {
  const uint64_t frequencyHz = 1750000;
  ASSERT_NO_THROW(rtlsdr1.setFrequency(frequencyHz));
  ASSERT_NO_THROW(std::cout << "Current frequencyHz: " +
                                   std::to_string(rtlsdr1.getFrequency())
                            << std::endl);
}

TEST_F(TestRtlSdr, TestSetLevel) {
  const uint32_t level = 10;
  ASSERT_NO_THROW(rtlsdr1.setLevel(level));
  ASSERT_NO_THROW(std::cout << "Current tuner gain, teenth of dB: " +
                                   std::to_string(rtlsdr1.getLevel())
                            << std::endl);
}

TEST_F(TestRtlSdr, TestSetLevelBig) {
  const uint32_t level = 100;
  ASSERT_NO_THROW(rtlsdr1.setLevel(level));
  ASSERT_NO_THROW(std::cout << "Current tuner gain, teenth of dB: " +
                                   std::to_string(rtlsdr1.getLevel())
                            << std::endl);
}

TEST_F(TestRtlSdr, TestSetParams) {
  TransferParams param1(1, TransferParams::Type::loop, 1024, 1024);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
}

ISDRStreamTransfer::Handler handlerFile = [](void *buf, size_t len) -> void {
  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
  std::string name = ctime(&t_c) + std::string("_test.bin");

  std::ofstream out(name, std::ios::binary | std::ios::app);
  out.write(static_cast<const char *>(buf), len);
  out.close();
};

ISDRStreamTransfer::Handler handlerVerbose = [](void *buf, size_t len) -> void {
  static std::uint64_t counter;
  counter++;
  std::cout << "Handler couner: " << counter << std::endl;
};

TEST_F(TestRtlSdr, TestStopWithoutStart) {
  TransferParams param1(1, TransferParams::Type::loop, 512000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_THROW(rtlsdr1.stop(), std::runtime_error);
}
TEST_F(TestRtlSdr, TestReceive) {
  TransferParams param1(1, TransferParams::Type::loop, 1024000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(250000));
  ASSERT_NO_THROW(rtlsdr1.start());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_NO_THROW(rtlsdr1.stop());
};

TEST_F(TestRtlSdr, TestSingle) {
  TransferParams param1(1, TransferParams::Type::single, 512000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.start());
}

TEST_F(TestRtlSdr, TestCounter) {
  TransferParams param1(1, TransferParams::Type::single, 512000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerFile));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.startCounter());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_NO_THROW(rtlsdr1.stop());
  ASSERT_NO_THROW(rtlsdr1.finalize());
}

TEST_F(TestRtlSdr, TestFinalize) {
  TransferParams param1(1, TransferParams::Type::loop, 512000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerFile));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.startCounter());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_NO_THROW(rtlsdr1.finalize());
}

TEST_F(TestRtlSdr, TestDestructor) {
  TransferParams param1(1, TransferParams::Type::loop, 512000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerFile));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.startCounter());
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

TEST_F(TestRtlSdr, TestPackageLessBuffer) {
  TransferParams param1(1, TransferParams::Type::loop, 512000, 256000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.startCounter());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_NO_THROW(rtlsdr1.finalize());
}

TEST_F(TestRtlSdr, TestPackageNotMultipleBuffer) {
  TransferParams param1(1, TransferParams::Type::loop, 512000, 153600);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.start());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_NO_THROW(rtlsdr1.finalize());
}

TEST_F(TestRtlSdr, TestTimeReceive) {
  TransferParams param1(1, TransferParams::Type::single, 512000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(256000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  auto start = std::chrono::steady_clock::now();
  ASSERT_NO_THROW(rtlsdr1.start());
  ASSERT_NO_THROW(rtlsdr1.stop());
  auto stop = std::chrono::steady_clock::now();
  ASSERT_EQ(std::chrono::duration_cast<std::chrono::seconds>(stop - start),
            std::chrono::seconds(1));

  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_NO_THROW(rtlsdr1.finalize());
}
