//
// Created by misha on 02.06.2025.
//

#include "SDREmulator.h"
#include <future>
#include "chrono"
#include "thread"


void SDREmulator::start_in_other_thread(emulator_callback callback) {
    std::mutex buff_mutex;
    std::unique_lock<std::mutex> buff_lock(buff_mutex);
    std::random_device rd;
    std::mt19937 gen64(rd());
    std::uniform_int_distribution<uint8_t> disturb(0, 255);
    emulator_transfer transfer = {
            native_buffer,
            native_buffer_length,
            0
    };
    for(int i = 0; this->thread_flag; i++) {
        status = SDRStatus::receiving;
        if (i == native_buffer_length) {
            i = 0;
        };
        buff_lock.lock();
        native_buffer[i] = disturb(gen64);
        buff_lock.unlock();
        transfer.valid_buff_length += 1;
        std::this_thread::sleep_for(std::chrono::seconds(1/sample_rate));
        if(i == chunk_size) {
            std::async(std::launch::async,
                       [this, callback, transfer]() {
                               callback(transfer, nullptr);
                       });

        }

    }
    status = SDRStatus::flushing;
    callback(transfer, nullptr);
}
void SDREmulator::start_rx(emulator_callback callback) {
    std::async(std::launch::async, [this](emulator_callback callback) {this->start_rx(callback); }, callback);
}
void SDREmulator::stop() {
    if(status == SDRStatus::receiving) {
        thread_flag = false;
    }
}

void SDREmulator::get_info() {
    std::cout << "This is SDR emulator with id " << sdr_id << std::endl;
}

void SDREmulator::set_sr(uint64_t sr) {
    sample_rate = sr;
}