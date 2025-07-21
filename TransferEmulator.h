//
// Created by misha on 30.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
#define RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
#include "ISDRStreamTransfer.h"
#include "SDREmulator.h" //эмулирует какой-то источник данных, работающий по схеме с буффером
#include "string"
#include "IRFControl.h"
#include "RFConfig.h"

class TransferEmulator : ISDRStreamTransfer, IRFControl { //реализация для эмулятора SDRTransferEmulator
public:
    TransferEmulator(const TransferParams& params, const RFConfig& config) : ISDRStreamTransfer(params), params_(params), config_(config) {
    }
   void setHandler(ITransferControl::Handler handler) override;
   void setPacketCount(std::size_t packetCount) override;
   std::size_t getPacketSize() const override;
   void finalize() override;
   void initialize() override;
   void startCounter() override;
   void setType(TransferParams::Type t) override;


   void setSampleRate(uint32_t sr) override;
   void setAtt(uint32_t gain) override;
   void setLNA(uint32_t gain) override;
   void setFrequency(uint64_t freq) override;
   void setVGA(uint32_t gain) override;

   void start() override;
   void stop() override;
   void static bridge(emulator_transfer, void *params);

private:
    TransferParams params_;
    RFConfig config_;
    SDREmulator* sdrEmulator;

};


#endif //RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
