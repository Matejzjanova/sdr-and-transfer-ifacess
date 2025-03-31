//
// Created by misha on 30.03.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_IRFCONTROL_H
#define RFI_BASE_ON_LIBHACKRF_IRFCONTROL_H
#include "stdint.h"
#include "libhackrf/src/hackrf.h"


class IRFControl {
//    Control methods
    virtual uint32_t setFrequency(uint64_t freq) = 0;
    virtual uint32_t setLNA(uint32_t gain) = 0;
    virtual  uint32_t setAtt() = 0;
//
//    virtual uint32_t findDevice() = 0;
//    virtual uint32_t init() = 0; //constructor

//RX/TX
    virtual uint32_t send(void* buf, size_t size) = 0;
    virtual uint32_t recieve(void* buf, size_t size) = 0;
};


#endif //RFI_BASE_ON_LIBHACKRF_IRFCONTROL_H
