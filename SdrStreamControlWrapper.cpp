//
// Created by misha on 08.09.2025.
//

#include "transfer_interface/transfer.h"
#include "cmath"
SdrStreamControlWrapper::SdrStreamControlWrapper(std::function<ISDRStreamTransfer&(uint32_t)>&& fun, uint32_t mask) {
   getter = fun;
   this->mask = mask;
}

void SdrStreamControlWrapper::startByMask(uint32_t mask) {
    /* transform size_t into mask? */
    int j = 0;
    uint32_t maxInt = pow(2, 32);
    for(uint32_t i = 0x0; i != maxInt; i*=2, j++) {
        if(mask && i) getter(j).start();
    }
}
void SdrStreamControlWrapper::stopByMask(uint32_t mask) {
    /* transform size_t into mask? */
    int j = 0;
    uint32_t maxInt = pow(2, 32);
    for(uint32_t i = 0x0; i != maxInt; i*=2, j++) {
        if(mask && i) getter(j).stop();
    }
}
void SdrStreamControlWrapper::resetByMask(uint32_t mask) {
    /* transform size_t into mask? */
    int j = 0;
    uint32_t maxInt = pow(2, 32);
    for(uint32_t i = 0x0; i != maxInt; i*=2, j++) {
        if(mask && i) getter(j).finalize();
    }
}
