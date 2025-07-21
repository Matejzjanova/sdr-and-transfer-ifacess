//
// Created by misha on 21.07.2025.
//
#include "stdint.h"
#ifndef RFI_BASE_ON_LIBHACKRF_RFCONFIG_H
#define RFI_BASE_ON_LIBHACKRF_RFCONFIG_H
struct RFConfig {
    RFConfig() = default;
    RFConfig(uint64_t sr, uint64_t fr, uint64_t g) : sampleRate(sr), frequency(fr), gain(g) {}
    uint64_t sampleRate;
    uint64_t frequency;
    uint64_t gain;
};
#endif //RFI_BASE_ON_LIBHACKRF_RFCONFIG_H
