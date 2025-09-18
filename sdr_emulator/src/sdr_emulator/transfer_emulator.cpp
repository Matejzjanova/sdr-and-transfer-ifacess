//
// Created by misha on 30.06.2025.
//
#include "sdr_emulator.h"

#include <iostream>
#include "thread"

SdrEmulator::SdrEmulator(size_t id, const TransferParams &params) : ISDRStreamTransfer(params) {
    sdr_id_ = id;
    params_ = params;
    state_ = State::waiting;
    is_rx.clear();
}

void SdrEmulator::initialize() {
        //smth init code
    native_buffer_ = new uint8_t[params_.bufferSize];
}

void SdrEmulator::setHandler(ITransferControl::Handler handler) {
    handler_ = handler;
}

void SdrEmulator::setType(TransferParams::Type t) {
    params_.type = t;
}

void SdrEmulator::setPacketCount(std::size_t packetCount) {

}

std::size_t SdrEmulator::getPacketSize() const {
    return params_.chunkSize;
}

void SdrEmulator::start() {
    if(params_.bufferSize == 0) {
        throw std::runtime_error("Buffer size is zero ):");
    }
    if(native_buffer_ == nullptr) {
        throw std::runtime_error("Initialize() emulator first!");
    }
    if(state_ == State::receiving) {
        throw std::runtime_error("Emulator actually emulating");
    }
    if(!sample_rate_) {
        throw std::runtime_error("Set correct sample rate");
    }
    auto start_rx_in_other_thread = [&] () -> size_t {
        is_rx.test_and_set();
        size_t i = 0, total_bytes_rx = 0;
        while (is_rx.test()) {
            native_buffer_[i] = random();
            i++;
            total_bytes_rx++;
            usleep(10e6 / sample_rate_);
            if(!(i % params_.chunkSize)) {
                handler_(&native_buffer_[i - params_.chunkSize], params_.chunkSize);
            }
            if(i == params_.bufferSize) {
                i = 0;
            }
        }
        return total_bytes_rx;
    };
    tx_thread = new std::thread(start_rx_in_other_thread);
}

void SdrEmulator::startCounter() {
//???
}

void SdrEmulator::stop() {
    is_rx.clear();
}

void SdrEmulator::finalize() {

}

 SdrEmulator::State SdrEmulator::getState() {
    return state_;
}

void * SdrEmulator::toContext() noexcept {

}

void SdrEmulator::setSampleRate(uint64_t samples) {
    sample_rate_ = samples;
}


