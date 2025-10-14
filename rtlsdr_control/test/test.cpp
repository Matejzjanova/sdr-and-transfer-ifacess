// Created by misha on 05.10.2025.
//
#include "rtlsdr_control.h"
#include "gtest/gtest.h"
#include <cstdint>
#include <sys/types.h>

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
}
