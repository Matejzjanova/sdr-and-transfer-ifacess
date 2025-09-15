//
// Created by misha on 02.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_DATAGENERATOR_H
#define RFI_BASE_ON_LIBHACKRF_DATAGENERATOR_H
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <mutex>
#include "stdint.h"
#include "syscall.h"
#include "iomanip"
#include "atomic"
#include "boost/circular_buffer.hpp"
#include "thread"
enum class EmulCodes {
    EMULATOR_ERROR = -1,
    EMULATOR_SUCCESS = 1
};
enum class EmulStat {
    receiving = 1,
    waitinig = 2,
    flushing = 3
};
struct emulator_transfer { //предполагается, что в SDR-библиотеках есть структура данных, аналогичная TransferParams
    uint8_t* buff;
    size_t buff_length;
    size_t valid_buff_length;
};
typedef void (*emulator_callback)(emulator_transfer transfer, void* ctx);
class SDREmulator {
public:
    SDREmulator(uint64_t packet_size = 10e6, uint64_t sample_rate = 10, size_t id = 0) : chunk_size(packet_size),
                                                                                           sample_rate(sample_rate),
                                                                                           status(EmulStat::waitinig) {
        auto timeOfCreation = std::chrono::system_clock::now();
        std::time_t timeOfCreation_t = std::chrono::system_clock::to_time_t(timeOfCreation);
        char *time = new char[25];
        std::strftime(time, sizeof(time), "%Y-%b-%d %H-%M-%S", std::gmtime(&timeOfCreation_t));
        sdr_id = std::string(time) + "id :" + std::to_string(id);
        delete[] time;

        native_buffer = new uint8_t[native_buffer_length];
    };

    ~SDREmulator() {
       delete[] native_buffer;
    }

    void start_in_other_thread(emulator_callback callback);

    uint8_t* native_buffer;
    const size_t native_buffer_length = 10e6;
    size_t chunk_size;
    uint64_t sample_rate;
    std::string sdr_id;
    std::atomic<bool> thread_flag;
    EmulStat status;
    std::mutex buff_mutex;


    /*


*
     * @brief сделан по аналогии c hackf_start_rx
     * @param emulator_callback функция для обработки буффера, принимает на вход указатель emulator_transfer
     */
};
   EmulCodes sdr_init();
//   EmulCodes sdr_open(SDREmulator* sdr);
   EmulCodes sdr_start_rx(SDREmulator* sdr, emulator_callback callback);
   EmulCodes sdr_stop(SDREmulator* sdr);
   EmulCodes sdr_set_sr(SDREmulator* sdr, uint64_t sr);


    void sdr_get_info(SDREmulator* sdr);

#endif //RFI_BASE_ON_LIBHACKRF_DATAGENERATOR_H
