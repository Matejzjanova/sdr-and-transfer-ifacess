//
// Created by misha on 30.06.2025.
//
#include "SDREmulator.h"
#include "TransferEmulator.h"
void TransferEmulator::setHandler(ITransferControl::Handler handler) {
    this->params.handler = handler;
}

void TransferEmulator::setPacketCount(std::size_t packetCount) {
    params.bufferSize = packetCount;
}

size_t TransferEmulator::getPacketSize() const {
    return params.bufferSize;
}

void TransferEmulator::start() {
    start_rx(transferBridge(emulator_transfer));
}