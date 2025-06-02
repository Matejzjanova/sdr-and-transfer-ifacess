//
// Created by misha on 02.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_DATAGENERATOR_H
#define RFI_BASE_ON_LIBHACKRF_DATAGENERATOR_H
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include "stdint.h"

class DataGenerator {
    void start(uint8_t * buff, uint64_t size, uint64_t sr);

};


#endif //RFI_BASE_ON_LIBHACKRF_DATAGENERATOR_H
