//
// Created by misha on 05.10.2025.
//
#include "rtlsdr_control.h"
#include "transfer_interface/transfer.h"
#include <cstddef>
#include <cstdint>
#include <rtl-sdr.h>
#include <stdexcept>
#include <string>

RtlsdrControl::RtlsdrControl(size_t dev_index)
    : ISDRStreamTransfer(TransferParams()) {
  rtlsdr_dev_t *dev = nullptr;
  int device_count = rtlsdr_get_device_count();

  int res = rtlsdr_open(&device, dev_index);
  if (res < 0) {
    throw std::runtime_error(std::string(
        "cant't open rtlsdr, rtlsdr_open return " + std::to_string(res)));
  }
}

RtlsdrControl::~RtlsdrControl() { rtlsdr_close(device); }
void RtlsdrControl::initialize() {}

void RtlsdrControl::finalize() {}

// transfer

void RtlsdrControl::start() {}

void RtlsdrControl::startCounter() {}

void RtlsdrControl::stop() {}

// set transfer params

void RtlsdrControl::setPacketCount(size_t packetCount) {}

std::size_t RtlsdrControl::getPacketSize() const {}

void RtlsdrControl::setPacketSize(size_t size) {}

void RtlsdrControl::setType(TransferParams::Type t) {}

void RtlsdrControl::setHandler(Handler hdl) {}
// set params

void RtlsdrControl::setFrequency(uint64_t fr) {
  if (fr < 100000 || fr > 1750000) {
    throw std::logic_error("freqency is not in the range 100kHz 1.75MHz");
  }
  check(rtlsdr_set_center_freq(device, fr));
}

void RtlsdrControl::setLevel(uint32_t levelDb) {}

void RtlsdrControl::setLna(uint32_t levelDb) {}

void RtlsdrControl::setPower(uint32_t power) {}

void RtlsdrControl::setAttenuator(uint32_t attDb) {}

uint64_t RtlsdrControl::getFrequency() const {}

uint32_t RtlsdrControl::getLevel() const {}

uint32_t RtlsdrControl::getLna() const {}

uint32_t RtlsdrControl::getPower() const {}

uint32_t RtlsdrControl::getAttenuator() const {}
// to context
//
void *RtlsdrControl::toContext() noexcept {};

void RtlsdrControl::check(int retVal) {
  if (retVal) {
    throw std::runtime_error("rtlsdr fun return: " + std::to_string(retVal));
  }
}
