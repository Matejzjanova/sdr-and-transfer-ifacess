//
// Created by misha on 30.06.2025.
//
#include "SDREmulator.h"
#include "TransferEmulator.h"
#include "ITransferParams.h"
void TransferEmulator::setHandler(ITransferControl::Handler handler) {
    this->params_.handler = handler;
}

void TransferEmulator::setPacketCount(std::size_t packetCount) {
    params_.bufferSize = packetCount;
}

size_t TransferEmulator::getPacketSize() const {
    return params_.bufferSize;
}

void TransferEmulator::start() {
    if(params_.type == TransferParams::Type::loop) {
        sdrEmulator->start_rx(bridge);

    }
    else {
    }
}

void TransferEmulator::bridge(emulator_transfer transfer, void *params) {
   static_cast<TransferParams*>(params)->handler(transfer.buff, transfer.valid_buff_length);
}

void TransferEmulator::stop() {
sdrEmulator->stop();
}
void TransferEmulator::initialize() {
}

void TransferEmulator::finalize() {

}

void TransferEmulator::startCounter() {

}
void TransferEmulator::setType(TransferParams::Type t) {
    params_.type = t;
}

void TransferEmulator::setSampleRate(uint32_t sr) {
    config_.sampleRate = sr;
    sdrEmulator->set_sr(sr);
}

void TransferEmulator::setFrequency(uint64_t freq) {
    throw std::logic_error("invalid emulator param") ;
}
void TransferEmulator::setVGA(uint32_t gain) {
    throw std::logic_error("invalid emulator param") ;
}
void TransferEmulator::setLNA(uint32_t gain) {
    throw std::logic_error("invalid emulator param") ;
}
void TransferEmulator::setAtt(uint32_t gain) {
    throw std::logic_error("invalid emulator param") ;
}
