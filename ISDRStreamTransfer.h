//
// Created by misha on 30.06.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_ISDRSTREAMTRANSFER_H
#define RFI_BASE_ON_LIBHACKRF_ISDRSTREAMTRANSFER_H
#include "ITransferControl.h"
#endif //RFI_BASE_ON_LIBHACKRF_ISDRSTREAMTRANSFER_H
class ISDRStreamTransfer : public ITransferControl {
    virtual void start();
    virtual void startCounter();
    virtual void stop();
};