//
// Created by misha on 12.05.2025.
//

#include "HackRFControl.h"

HackRFControl::HackRFControl() {
    errorHandler(hackrf_init());
    errorHandler(hackrf_open(&device));
    hackrf_set_sample_rate(device, 10e6);
    hackrf_set_amp_enable(device, 1);
    manager = nullptr;
}
void HackRFControl::setSampleRate(uint32_t sr) {
    hackrf_set_sample_rate(device, sr);
}

 void HackRFControl::errorHandler(int code) {
    switch(code) {
        case HACKRF_SUCCESS:
            return;
        default:
            throw std::runtime_error("HACKRF ERROR:" + std::string(hackrf_error_name( (hackrf_error) code)));
    }
}
void HackRFControl::setFrequency(uint64_t freq) {
    errorHandler(hackrf_set_freq(device, freq));
    this->freq = freq;
}

//    void setFrequencyLOIF(uint64_t lo, uint64_t to, rf_path_filter path) {
//        errorHandler(hackrf_set_freq_explicit(device, from, to, path));
//        freq = (from - to)/2;
//    }

void HackRFControl::setLNA(uint32_t gain) {
    gain = gain <= 62 ? gain : 62;
    errorHandler(hackrf_set_lna_gain(device, (gain)/2*2));
}

void HackRFControl::setVGA(uint32_t gain) {
    gain = gain <= 47 ? gain : 47;
    errorHandler(hackrf_set_vga_gain(device, gain > 40 ? 40 : (gain+4 /8 * 8)));
    errorHandler(hackrf_set_txvga_gain(device, gain));
}
void HackRFControl::setAtt(uint32_t gain) {
    std::cout << "HackRF has no att!" << std::endl;
}

 HackRFControl::~HackRFControl(){
    hackrf_close(device);
};

void* HackRFControl::allocateBuf(size_t size) {
    buffer = new uint8_t[size];
    buffer_size = size;
    return buffer;
}

void HackRFControl::setIRQManager(std::function<void(void*, size_t)> foo) {
    manager = foo;
}

void HackRFControl::setIRQsize(size_t size) {
    irq_size = size;
}
int HackRFControl::callback(hackrf_transfer* transfer) {
    if(transfer->buffer_length < irq_size) {
        std::cout << "Too large chunk size" << std::endl;
        irq_size = transfer->buffer_length;
    }
    for (size_t i = 0; i != irq_size;  i+=2) {
        transfer->buffer[i] = buffer[i];
        transfer->buffer[i+1] = buffer[i+1];
    }

}

void HackRFControl::start(int time) {
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    errorHandler(hackrf_start_rx(device, this->callback, nullptr));
    sleep(time);
//        cout << "We're here!!" << endl;
    hackrf_stop_rx(device);
}