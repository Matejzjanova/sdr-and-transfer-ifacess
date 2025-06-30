//
// Created by misha on 30.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
#define RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
#include "ISDRStreamTransfer.h"
#include "SDREmulator.h" //эмулирует какой-то источник данных, работающий по схеме с буффером

class TransferEmulator : ISDRStreamTransfer {
   void setHandler(ITransferControl::Handler handler) override;
   void setPacketCount(std::size_t packetCount) override;
   std::size_t getPacketSize() const override;
   void finalize() override;
   void initalize() override;
   void setType(TransferParams::Type t) override;

   void start() override;
   void stop() override;
   void startCounter() override;

private:
    void transferBridge(emulator_transfer);
    TransferParams params;
};


#endif //RFI_BASE_ON_LIBHACKRF_TRANSFEREMULATOR_H
