//
// Created by misha on 05.10.2025.
//
#include "rtl-sdr.h"
#include "sdr_rf_interface/sdr_rf.h"
#include "transfer_interface/transfer.h"

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <thread>
#ifndef RFI_BASE_ON_LIBHACKRF_TEST_RTLSDR_H
#define RFI_BASE_ON_LIBHACKRF_TEST_RTLSDR_H
class RtlsdrControl : ISDRStreamTransfer, ISdrDevice, IDeviceRF {

public:
  enum class RtlsdrState {
    waiting,
    recieveng
    // flushing;
  };

  explicit RtlsdrControl(size_t dev_index);
  ~RtlsdrControl();
  void initialize() override;
  void finalize() override;
  // transfer param
  void setParam(TransferParams &params);

  void setHandler(Handler hdl) override;
  Handler getHandler();
  void setType(TransferParams::Type t) override;
  void setPacketSize(size_t size) override;
  std::size_t getPacketSize() const override;
  void setPacketCount(size_t packetCount) override;

  void setSampleRate(uint64_t sr);
  // transfer
  void start() override;
  void startCounter() override;
  void stop() override;

  // sdr param
  void setFrequency(uint64_t frequencyHz) override;
  uint64_t getFrequency() const override;

  void setLevel(uint32_t levelDb) override;
  uint32_t getLevel() const override;

  void setLna(uint32_t lna) override;
  uint32_t getLna() const override;

  void setPower(uint32_t power) override;
  uint32_t getPower() const override;

  void setAttenuator(uint32_t attDb) override;
  uint32_t getAttenuator() const override;
  // to context
  void *toContext() noexcept override;

  void setBandwith();

private:
  void check(int retVal);

  Handler hdl_;

  void static cbWrapper(uint8_t *buf, uint32_t len, void *ctx);

  RtlsdrState state;

  uint64_t frequencyHz_ = 17500000;
  uint64_t levelDb_;
  uint64_t levelItermididateFrequency_;

  TransferParams param_;

  rtlsdr_dev_t *device_;

  uint64_t *gains_;
  size_t numGains_;
  uint32_t ifGain_;

  static const uint32_t kMaxIfGain = 60;

  std::thread *recieveThread;
  std::condition_variable a;
};
#endif // RFI_BASE_ON_LIBHACKRF_TEST_RTLSDR_H
