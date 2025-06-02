//
// Created by misha on 02.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_ITRANSFEREMULATOR_H
#define RFI_BASE_ON_LIBHACKRF_ITRANSFEREMULATOR_H
#include "ITransferControl.h"

class ITransferEmulator : ITransferControl{
    void * allocateBuf(size_t size) override;
    void setIRQManager(std::function<void (void *, size_t)> manager) override;
    void setIRQsize(size_t size) override;
};


#endif //RFI_BASE_ON_LIBHACKRF_ITRANSFEREMULATOR_H
