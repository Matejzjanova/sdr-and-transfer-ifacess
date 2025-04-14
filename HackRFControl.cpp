//
// Created by misha on 27.03.2025.
//
using namespace std;

#include <iostream>
#include <future>
#include "IRFControl.h"
#include "libhackrf/src/hackrf.h"
class HackRFControl: IRFControl {
    HackRFControl() {
        errorHandler(hackrf_init());
        errorHandler(hackrf_open(&device));
    }

    static void errorHandler(int code) {
        switch(code) {
            case HACKRF_SUCCESS:
                return;
            default:
                throw std::runtime_error("HACKRF ERROR:" + string(hackrf_error_name( (hackrf_error) code)));
        }
    }

    void setFrequency( uint64_t freq) override {
        errorHandler(hackrf_set_freq(device, freq));
    }

    uint64_t setFrequency(uint64_t from, uint64_t to, rf_path_filter path) {
        errorHandler(hackrf_set_freq_explicit(device, from, to, path));
    }

    void setLNA(uint32_t gain) override {
        errorHandler(hackrf_set_lna_gain(device, gain));
    }

    uint32_t recieve(void *buf, size_t size) override {
        hackrf_transfer rx;
        std::future<void*> buff = async(hackrf_start_rx(device, manager, rx.rx_ctx));
    }

    uint32_t send(void *buf, size_t size) override {

    }
    hackrf_device* device;

    void *manager = nullptr;

    ~HackRFControl(){
        hackrf_close(device);
    };

};