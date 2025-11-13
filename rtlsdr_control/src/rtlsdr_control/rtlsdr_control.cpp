//
// Created by misha on 05.10.2025.
//
#include "rtlsdr_control.h"
#include "transfer_interface/transfer.h"

#include <rtl-sdr.h>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>

RtlsdrControl::RtlsdrControl(size_t dev_index)
    : ISDRStreamTransfer(TransferParams()) {

  state_ = RtlsdrState::waiting;
  rtlsdr_dev_t *dev = nullptr;
  int device_count = rtlsdr_get_device_count();

  check(rtlsdr_open(&device_, dev_index));

  numGains_ = rtlsdr_get_tuner_gains(device_, nullptr);
  gains_ = new uint64_t[numGains_];
}

RtlsdrControl::~RtlsdrControl() {
  if (state_ == RtlsdrState::receiveng) {
    finalize();
  }
  if (buf_) {
    finalize();
  }
  delete gains_;
  rtlsdr_close(device_);
}

void RtlsdrControl::initialize() {
  if (param_.bufferSize <= 0) {
    throw std::logic_error("buffer must be > 0");
  }
  buf_ = new uint8_t[param_.bufferSize];
  std::cout << "buf alloc sz: " << std::to_string(param_.bufferSize)
            << std::endl;
  readSz_ = new int;
}

void RtlsdrControl::finalize() {
  if (state_ == RtlsdrState::receiveng) {
    stop();
  };
  delete[] buf_;
  buf_ = nullptr;
  delete readSz_;
  readSz_ = nullptr;
  delete rxThread;
}

// transfer

void RtlsdrControl::recieveSingle(uint8_t *currentPos, size_t available) {

  int totalRead = 0;

  if (available < param_.packageSize) {
    std::cout << "not enough space in buf" << std::endl;
    rtlsdr_reset_buffer(device_);
    rtlsdr_read_sync(device_, currentPos, available, readSz_);
    if (*readSz_ <= 0) {

      throw std::runtime_error("smth wrong with sdr, num samples read is" +
                               std::to_string(*readSz_));
    }
    hdl_(currentPos, *readSz_);
    totalRead = *readSz_;
    *readSz_ = 0;

    rtlsdr_read_sync(device_, buf_, param_.packageSize - available, readSz_);
    if (*readSz_ <= 0) {
      throw std::runtime_error("smth wrong with sdr, num samples read is" +
                               std::to_string(*readSz_));
    }
    hdl_(currentPos, *readSz_);
    *readSz_ += totalRead;

  } else {

    std::cout << "enough space in buf" << std::endl;
    rtlsdr_reset_buffer(device_);

    rtlsdr_read_sync(device_, currentPos, param_.packageSize, readSz_);

    if (*readSz_ <= 0) {
      throw std::runtime_error("smth wrong with sdr, num samples read is " +
                               std::to_string(*readSz_));
    }

    hdl_(currentPos, *readSz_);
  }
}

void RtlsdrControl::start() {
  if (!hdl_) {
    throw std::runtime_error("bad handler");
  }
  if (!buf_) {
    throw std::runtime_error("you must init rtlsdr before receive");
  }
  if (param_.type == TransferParams::Type::loop) {

    auto startInOtherThread = [this]() -> void {
      size_t posCounter = 0;
      isReceive.test_and_set();
      while (isReceive.test()) {
        recieveSingle(buf_ + posCounter, param_.bufferSize - posCounter);
        posCounter += param_.packageSize;
        posCounter = posCounter % param_.bufferSize;
        std::cout << "bytes read: " << std::to_string(*readSz_) << std::endl;
      }
    };
    state_ = RtlsdrState::receiveng;
    std::cout << "starting with buffer sz " << std::to_string(param_.bufferSize)
              << " and chunk sz " << std::to_string(param_.packageSize)
              << std::endl;
    rxThread = new std::thread(startInOtherThread);

  } else {
    auto startInOtherThreadSingle = [this]() -> void {
      size_t posCounter = 0;
      isReceive.test_and_set();
      recieveSingle(buf_ + posCounter, param_.bufferSize - posCounter);
      posCounter += param_.packageSize;
      posCounter %= param_.bufferSize;
      std::cout << "bytes read: " << std::to_string(*readSz_) << std::endl;
    };
    state_ = RtlsdrState::receiveng;
    std::cout << "starting single with buffer sz "
              << std::to_string(param_.bufferSize) << " and chunk sz "
              << std::to_string(param_.packageSize) << std::endl;
    rxThread = new std::thread(startInOtherThreadSingle);
    isReceive.clear();
  }
}

void RtlsdrControl::startCounter() {
  rtlsdr_set_testmode(device_, 1);
  state_ = RtlsdrState::counter;
  start();
}

void RtlsdrControl::stop() {
  if (state_ == RtlsdrState::receiveng) {
    if (state_ == RtlsdrState::counter) {
      rtlsdr_set_testmode(device_, 0);
    }
    std::cout << "stopping receive" << std::endl;
    isReceive.clear();
    if (rxThread) {
      rxThread->join();
    }
    state_ = RtlsdrState::waiting;

  } else {
    throw std::runtime_error("nothing to stop");
  }
}

// set transfer params

std::size_t RtlsdrControl::getPacketSize() const { return param_.packageSize; }

void RtlsdrControl::setPacketSize(size_t size) {
  param_.packageSize = size;
  std::cout << "set chunk size: " << std::to_string(param_.bufferSize);
}
void RtlsdrControl::setType(TransferParams::Type t) { param_.type = t; }

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
