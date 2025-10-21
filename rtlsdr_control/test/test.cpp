// Created by misha on 05.10.2025.
//
#include "rtlsdr_control.h"
#include "transfer_interface/transfer.h"
#include "gtest/gtest.h"
#include <chrono>
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

TEST_F(TestRtlSdr, TestSetParams) {
  TransferParams param1(1, TransferParams::Type::loop, 1024, 1024);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
}

ISDRStreamTransfer::Handler handlerFile = [](void *buf, size_t len) -> void {
  std::ofstream out("./test.bin", std::ios::binary | std::ios::app);
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
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(250000));
  ASSERT_THROW(rtlsdr1.stop(), std::runtime_error);
}
TEST_F(TestRtlSdr, TestReceive) {
  TransferParams param1(1, TransferParams::Type::loop, 512000, 512000);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(250000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.start());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  ASSERT_NO_THROW(rtlsdr1.stop());
};

TEST_F(TestRtlSdr, TestSingle) {
  TransferParams param1(1, TransferParams::Type::single, 512000, 512000);
  rtlsdr1.setPacketCount(2);
  ASSERT_NO_THROW(rtlsdr1.setParam(param1));
  ASSERT_NO_THROW(rtlsdr1.setHandler(handlerVerbose));
  ASSERT_NO_THROW(rtlsdr1.setSampleRate(260000));
  ASSERT_NO_THROW(rtlsdr1.initialize());
  ASSERT_NO_THROW(rtlsdr1.start());
}
