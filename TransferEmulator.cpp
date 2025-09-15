//
// Created by misha on 30.06.2025.
//
#include "SDREmulator.h"
#include "TransferEmulator.h"
#include "ITransferParams.h"
void TransferEmulator::setHandler(ITransferControl::Handler handler) {
    this->handler = std::move(handler);

}

void TransferEmulator::setPacketCount(std::size_t packetCount) {
    params_.bufferSize = packetCount;
}

size_t TransferEmulator::getPacketSize() const {
    return params_.bufferSize;
}

void TransferEmulator::start() {
    if(params_.type == TransferParams::Type::loop) {
        if(sdrEmulator == nullptr) throw std::runtime_error("Initialize SDR first!")  ;
        if(handler == nullptr) throw std::runtime_error("Initialize handler first!");
        sdr_start_rx(
                sdrEmulator.get(),
                &bridge);
    }
    else {
    }
}

void TransferEmulator::bridge(emulator_transfer transfer, void * hdl) {
    static_cast<TransferEmulator*>(hdl)->handler(static_cast<void*>(transfer.buff), transfer.valid_buff_length);
}

void TransferEmulator::stop() {
    if(sdr_stop(sdrEmulator.get()) != EmulCodes::EMULATOR_SUCCESS) {
        throw std::runtime_error("Initialize SDR first!");
    }
}
void TransferEmulator::initialize() {

   // if (sdr_init() != EmulCodes::EMULATOR_SUCCESS) throw std::runtime_error("Smth gone wrong while init open");

    SDREmulator* iEmulator = new SDREmulator();
    //EmulCodes resOpen = sdr_open(*iEmulator);
    //if (resOpen != EmulCodes::EMULATOR_SUCCESS) throw std::runtime_error("Smth gone wrong while open sdr");
    sdrEmulator = std::unique_ptr<SDREmulator>(iEmulator);

}

void TransferEmulator::finalize() {

}

void TransferEmulator::startCounter() {

}
void TransferEmulator::setType(TransferParams::Type t) {
    params_.type = t;
}


EmulStat TransferEmulator::getState() {
    return sdrEmulator->status;
}
