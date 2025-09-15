//
// Created by misha on 02.06.2025.
//

#include "SDREmulator.h"
#include <future>
#include "chrono"
#include "thread"

void SDREmulator::start_in_other_thread(emulator_callback callback) {
    std::random_device rd;
    std::mt19937 gen64(rd());
    std::uniform_int_distribution<uint8_t> disturb(0, 255);
    emulator_transfer transfer = {
            native_buffer,
            native_buffer_length,
            0
    };
    for(int i = 0; this->thread_flag; i++) {
        status = EmulStat::receiving;
        if (i == native_buffer_length) {
            i = 0;
        };
        std::lock_guard<std::mutex> buff_lock(buff_mutex);
        native_buffer[i] = disturb(gen64);
        transfer.valid_buff_length += 1;
        std::this_thread::sleep_for(std::chrono::seconds(1/sample_rate));
        if(i == chunk_size) {
        auto nothing= std::async(std::launch::async,
                       [this, callback, transfer]() {
                               callback(transfer, nullptr);
                       });

        }

    }
    status = EmulStat::flushing;
    callback(transfer, nullptr);
}
EmulCodes sdr_start_rx(SDREmulator* sdr, emulator_callback callback) {

    auto nothing = std::async(std::launch::async, &SDREmulator::start_in_other_thread, sdr, callback);
    nothing;
    return EmulCodes::EMULATOR_SUCCESS;
}
EmulCodes sdr_stop(SDREmulator* sdr) {
    if(sdr == nullptr) {
        return EmulCodes::EMULATOR_ERROR;
    }
    if(sdr->status == EmulStat::receiving ) {
        sdr->thread_flag = false;
    }
    return EmulCodes::EMULATOR_SUCCESS;
}

void sdr_get_info(SDREmulator* sdr) {
    std::cout << "This is SDR emulator with id " << sdr->sdr_id << std::endl;
}

EmulCodes sdr_set_sr(SDREmulator* sdr, uint64_t sr) {
    sdr->sample_rate = sr;
    return EmulCodes::EMULATOR_SUCCESS;
}

EmulCodes sdr_init() {
   return EmulCodes::EMULATOR_SUCCESS;
}

EmulCodes sdr_close(SDREmulator* sdr) {
        delete sdr;
        return EmulCodes::EMULATOR_SUCCESS;
}
