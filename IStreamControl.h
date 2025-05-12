//
// Created by misha on 12.05.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_ISTREAMCONTROL_H
#define RFI_BASE_ON_LIBHACKRF_ISTREAMCONTROL_H
class IStreamControl {
    void virtual start(int time) = 0;
    void virtual stop() = 0;
};
#endif //RFI_BASE_ON_LIBHACKRF_ISTREAMCONTROL_H
