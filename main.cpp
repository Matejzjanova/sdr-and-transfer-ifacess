//
// Created by misha on 21.07.2025.
//
#include "TransferEmulator.h"
int main() {
    TransferParams params(1, TransferParams::Type::loop, 100);
    TransferEmulator SDR1(params);
}