//
// Created by misha on 28.04.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_IRFCONTROL_H
#define RFI_BASE_ON_LIBHACKRF_IRFCONTROL_H

#include "stdint.h"
#include "hackrf/host/libhackrf/src/hackrf.h"

class IRFControl {
    virtual void setFrequency(uint64_t freq) = 0;
    virtual void setLNA(uint32_t gain) = 0;
    virtual void setAtt(uint32_t gain) = 0;
    virtual void setVGA(uint32_t gain) = 0;
    virtual void setSampleRate(uint32_t sr) = 0;
};


#endif //RFI_BASE_ON_LIBHACKRF_IRFCONTROL_H
