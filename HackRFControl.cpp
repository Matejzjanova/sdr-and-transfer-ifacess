//
// Created by misha on 27.03.2025.
//
using namespace std;

#include <iostream>
#include "IRFControl.h"
#include "libhackrf/src/hackrf.h"
class HackRFControl: IRFControl {
    HackRFControl() {
        if (hackrf_init()) {
            if(hackrf_open(&device)) {
                //information about opened hackrf version etc
            };
        };
    }

    virtual uint32_t setFrequency( uint64_t freq) {

        uint32_t result = hackrf_set_freq(device, freq);
        return HACKRF_ERROR_OTHER;
    }

    virtual uint64_t setFrequency(uint64_t from, uint64_t to, rf_path_filter path) {
        if (hackrf_set_freq_explicit(device, from, to, path)) {
            return 0;
        };
        return HACKRF_ERROR_OTHER;
    }

    virtual uint32_t setAtt() {

    }

    virtual uint32_t setLNA(uint32_t gain)  {
        if(hackrf_set_lna_gain(device, gain)) {
            return 0;
        };
        return HACKRF_ERROR_OTHER;
    }

    hackrf_device* device{};
};