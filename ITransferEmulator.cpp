//
// Created by misha on 02.06.2025.
//

#include "ITransferEmulator.h"

void *ITransferControl::allocateBuf(size_t size) {
    buff = new void*[size];
    return buff;
}

void ITransferControl::setIRQManager(std::function<void(void *, size_t)> manager) {
     this->manager = manager;
}

void ITransferControl::setIRQsize(size_t size) {
    irqSize = size;
}