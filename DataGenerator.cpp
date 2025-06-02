//
// Created by misha on 02.06.2025.
//

#include "DataGenerator.h"

void DataGenerator::start(uint8_t *buff, uint64_t size, uint64_t sr) {
     std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> disturb(0, 255);

    const auto delay = std::chrono::milliseconds(1000/sr);
    for(size_t i = 0; i != size; i+=2) {
        buff[i] = disturb(gen);
        buff[i + 1] = disturb(gen);
    }
}