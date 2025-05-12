//
// Created by misha on 12.05.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_HACKRFCONTROL_H
#define RFI_BASE_ON_LIBHACKRF_HACKRFCONTROL_H


#include "IRFControl.h"
#include "IStreamControl.h"
#include "ITransferControl.h"
#include "hackrf/host/libhackrf/src/hackrf.h"
#include <stdexcept>
#include <iostream>
#include <string>
#include <csignal>
class HackRFControl : IStreamControl, IRFControl, ITransferControl {
public:
    HackRFControl();
    ~HackRFControl();

    void setSampleRate(uint32_t sr) override;
    void setFrequency(uint64_t freq) override;
    void setVGA(uint32_t gain) override;
    void setAtt(uint32_t gain) override;
    void setLNA(uint32_t) override;

    void setIRQsize(size_t size) override;
    void * allocateBuf(size_t size) override;
    void setIRQManager(std::function<void (void *, size_t)>) override;

    void start(int time) override;
    void errorHandler(int code);

    hackrf_device* device;
    std::function<void(void*, size_t)>  manager;
    size_t irq_size;
    uint8_t * buffer;
    size_t buffer_size;
    uint32_t freq;
private:
    int callback(hackrf_transfer* transfer);
};


#endif //RFI_BASE_ON_LIBHACKRF_HACKRFCONTROL_H
