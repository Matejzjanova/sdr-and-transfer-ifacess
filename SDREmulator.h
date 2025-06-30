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
struct emulator_transfer {
    uint8_t* buff;
    size_t buff_length;
};
typedef void (*emulator_callback)(emulator_transfer transfer);
class SDREmulator {
public:
    SDREmulator(uint64_t packet_size, uint64_t sample_rate) : chunk_size(packet_size), sample_rate(sample_rate) {
         native_buffer = new uint8_t[native_buffer_length];
    };
    ~SDREmulator() {
        delete [] native_buffer;
    }
    /**
     * @brief сделан по аналогии c hackf_start_rx
     * @param emulator_callback функция для обработки буффера, принимает на вход указатель emulator_transfer
     */
    void start_rx(emulator_callback callback);
    void stop();
private:
    uint8_t* native_buffer;
    const size_t native_buffer_length = 10e6;
    size_t chunk_size;
    uint64_t sample_rate;

};


#endif //RFI_BASE_ON_LIBHACKRF_DATAGENERATOR_H
