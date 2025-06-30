//
// Created by misha on 02.06.2025.
//

#include "SDREmulator.h"
#include <future>
#include "chrono"
#include "thread"


void SDREmulator::start_rx(emulator_callback callback) {
    std::random_device rd;
    std::mt19937 gen64(rd());
    std::uniform_int_distribution<uint8_t> disturb(0, 255);
    emulator_transfer transfer = {
            native_buffer,
            native_buffer_length
    };
    for(int i = 0; ; i++) {
        i == native_buffer_length ? 0 : i;
        if(i == chunk_size) {
            std::future<void> f = std::async(std::launch::async, callback, transfer);
        }
        native_buffer[i] = disturb(gen64);
        std::this_thread::sleep_for(std::chrono::seconds(1/sample_rate));
    }

}