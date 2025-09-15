//
// Created by misha on 30.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
#define RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
#include "ISDRStreamTransfer.h"
#include "SDREmulator.h" //эмулирует какой-то источник данных, работающий по схеме с буффером
#include "string"
#include <memory>

class TransferEmulator : ISDRStreamTransfer  { //реализация для эмулятора SDRTransferEmulator
public:
   explicit TransferEmulator(const TransferParams& params) : ISDRStreamTransfer(params), params_(params) {
        EmulCodes resInit = sdr_init();
        if(resInit != EmulCodes::EMULATOR_SUCCESS) {
            throw std::runtime_error("Smth gone wrong while init");
        }

       // EmulCodes resOpen = sdr_open(sdrEmulator.get()); if(resOpen != EmulCodes::EMULATOR_SUCCESS) {
       //     // std::cout << "ERROR: SDR can't open with code " << resOpen << std::endl;
       //     throw std::runtime_error("");
       // }

   }

   void setHandler(ITransferControl::Handler handler) override;
   void setPacketCount(std::size_t packetCount) override;
   std::size_t getPacketSize() const override;
   void finalize() override;
   void initialize() override;
   void startCounter() override;
   void setType(TransferParams::Type t) override;
   EmulStat getState();

   void start() override;
   void stop() override;
   void static bridge(emulator_transfer t, void* ctx);
   std::function<void(void* ptr, std::size_t sz)> handler;

private:
    TransferParams params_;
    std::unique_ptr<SDREmulator> sdrEmulator;

};


#endif //RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
