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

  check(rtlsdr_open(&device_, dev_index));

  numGains_ = rtlsdr_get_tuner_gains(device_, nullptr);
  gains_ = new uint64_t[numGains_];
}

RtlsdrControl::~RtlsdrControl() {
  delete gains_;
  rtlsdr_close(device_);
}
void RtlsdrControl::initialize() {}

void RtlsdrControl::finalize() {}

// transfer

void RtlsdrControl::start() {

  if (param_.type == TransferParams::Type::loop) {
    rtlsdr_reset_buffer(device_);
    if (!hdl_) {
      throw std::runtime_error("bad handler");
    }
    rtlsdr_read_async(
        device_,
        [](uint8_t *buf, uint32_t len, void *ctx) -> void {
          RtlsdrControl *self = static_cast<RtlsdrControl *>(ctx);
          Handler hdl = self->getHandler();
          hdl(buf, len);
        },
        this, static_cast<uint32_t>(param_.bufferSize / param_.packageSize),
        param_.packageSize);
  } else {
    rtlsdr_read_async(
        device_,
        [](uint8_t *buf, uint32_t len, void *ctx) -> void {
          static_cast<RtlsdrControl *>(ctx)->getHandler()(buf, len);
        },
        this, param_.packageCount, param_.packageSize *param_.packageCount);
    int packageRecieve = 0;
  }
}

void RtlsdrControl::startCounter() {}

void RtlsdrControl::stop() { check(rtlsdr_cancel_async(device_)); }

// set transfer params

void RtlsdrControl::setPacketCount(size_t packetCount) {}

std::size_t RtlsdrControl::getPacketSize() const {}

void RtlsdrControl::setPacketSize(size_t size) {}

void RtlsdrControl::setType(TransferParams::Type t) {}
void RtlsdrControl::setHandler(Handler hdl) { hdl_ = hdl; }
// set params

void RtlsdrControl::setFrequency(uint64_t fr) {
  if (fr < 100000 || fr > 1750000) {
    throw std::logic_error("freqency is not in the range 100kHz 1.75MHz");
  }
  check(rtlsdr_set_center_freq(device_, fr));
}

void RtlsdrControl::setLevel(uint32_t levelDb) {

  uint32_t maxGain = gains_[numGains_];

  if (levelDb > gains_[numGains_]) {
    check(rtlsdr_set_tuner_gain(device_, maxGain));
    check(
        // TODO: впихнуть сюда готовые таблицы со стадиями усиления
        rtlsdr_set_tuner_if_gain(device_, 6, levelDb * 10 - maxGain));

    ifGain_ = levelDb - maxGain > kMaxIfGain ? kMaxIfGain : levelDb - maxGain;

  } else {
    rtlsdr_set_tuner_gain(device_, levelDb);
  }
}

void RtlsdrControl::setLna(uint32_t levelDb) {}

void RtlsdrControl::setPower(uint32_t power) {}

void RtlsdrControl::setAttenuator(uint32_t attDb) {}

uint64_t RtlsdrControl::getFrequency() const {
  return rtlsdr_get_center_freq(device_);
}

uint32_t RtlsdrControl::getLevel() const {
  return rtlsdr_get_tuner_gain(device_) + ifGain_;
}

uint32_t RtlsdrControl::getLna() const {}

uint32_t RtlsdrControl::getPower() const {}

uint32_t RtlsdrControl::getAttenuator() const {}
// to context
//
void *RtlsdrControl::toContext() noexcept {};

void RtlsdrControl::check(int retVal) {
  if (retVal) {
    throw std::runtime_error("rtlsdr function return: " +
                             std::to_string(retVal));
  }
}

void RtlsdrControl::setParam(TransferParams &params) { param_ = params; }

ISDRStreamTransfer::Handler RtlsdrControl::getHandler() { return hdl_; }

void RtlsdrControl::setSampleRate(uint64_t sr) {
  check(rtlsdr_set_sample_rate(device_, sr));
}
