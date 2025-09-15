//
// Created by misha on 30.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_ISDRSTREAMTRANSFER_H
#define RFI_BASE_ON_LIBHACKRF_ISDRSTREAMTRANSFER_H
#include "ITransferControl.h"
class ISDRStreamTransfer : public ITransferControl {
public:
    explicit ISDRStreamTransfer(const TransferParams& params) : ITransferControl(params) {}
    virtual void start() = 0;
    virtual void startCounter() = 0;
    virtual void stop() = 0;
};
#endif //RFI_BASE_ON_LIBHACKRF_ISDRSTREAMTRANSFER_H