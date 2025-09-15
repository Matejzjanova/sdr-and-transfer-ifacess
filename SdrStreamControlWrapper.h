//
// Created by misha on 21.07.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_SDRSTREAMCONTROLWRAPPER_H
#define RFI_BASE_ON_LIBHACKRF_SDRSTREAMCONTROLWRAPPER_H
#include "functional"
#include "ISDRStreamTransfer.h"
#include "memory"
#include "vector"
enum class SDRState {
    STATE_IDLE = 0x01,
    STATE_RUNNING = 0x02,
    STATE_ERROR = 0x03
};
class SdrStreamControlWrapper {
    SdrStreamControlWrapper(std::function<ISDRStreamTransfer&(uint32_t)>&&, uint32_t mask);
    ~SdrStreamControlWrapper();
    void startByMask(uint32_t mask);
    void stopByMask(uint32_t mask);
    void startCntrByMask(uint32_t mask);
    void resetByMask(uint32_t mask);
    void unresetByMask(uint32_t mask);
    uint32_t getState(size_t) const;

    std::function<ISDRStreamTransfer&(size_t)> getter;
    uint32_t max_dev;
    uint32_t mask = 0;
};


#endif //RFI_BASE_ON_LIBHACKRF_SDRSTREAMCONTROLWRAPPER_H
