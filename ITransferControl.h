//
// Created by misha on 12.05.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H
#define RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H

#include "stdint.h"
#include "hackrf/host/libhackrf/src/hackrf.h"
#include "functional"
class ITransferControl {
    virtual void* allocateBuf(size_t size) = 0;
    virtual void setIRQsize(size_t size) = 0;
    virtual void setIRQManager(std::function<void(void*, size_t)> manager) = 0;

    void* buff;
    std::function<void(void*, size_t)>  manager;
    size_t irqSize;
};
#endif //RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H
