//
// Created by misha on 21.07.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_SDRSTREAMCONTROLWRAPPER_H
#define RFI_BASE_ON_LIBHACKRF_SDRSTREAMCONTROLWRAPPER_H
#include "functional"
#include "ISDRStreamTransfer.h"
class SdrStreamControlWrapper {
    SdrStreamControlWrapper(std::function<ISDRStreamTransfer&(uint32_t)>&&, uint32_t id) {

    }
};


#endif //RFI_BASE_ON_LIBHACKRF_SDRSTREAMCONTROLWRAPPER_H
