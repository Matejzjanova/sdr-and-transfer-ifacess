//
// Created by misha on 21.04.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_WAH_H
#define RFI_BASE_ON_LIBHACKRF_WAH_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>


void writeWavHeader(std::ofstream& file, uint32_t sampleRate, uint32_t dataSize);
#endif //RFI_BASE_ON_LIBHACKRF_WAH_H
