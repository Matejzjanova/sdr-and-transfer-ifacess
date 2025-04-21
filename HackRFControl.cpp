//
// Created by misha on 27.03.2025.
//
using namespace std;

#include <iostream>
#include <future>
#include <csignal>
#include "IRFControl.h"
#include "fstream"
#include "wav.h"
#include "cmath"
#include "complex"
class HackRFControl: IRFControl {
public:
    HackRFControl() {

        errorHandler(hackrf_init());
        errorHandler(hackrf_open(&device));
        hackrf_set_sample_rate(device, 10e6);
        hackrf_set_amp_enable(device, 1);
//        manager = receive_callback;
    }

    static void errorHandler(int code) {
        switch(code) {
            case HACKRF_SUCCESS:
                return;
            default:
                throw std::runtime_error("HACKRF ERROR:" + string(hackrf_error_name( (hackrf_error) code)));
        }
    }

    void setFrequency(uint64_t freq) override {
        errorHandler(hackrf_set_freq(device, freq));
        this->freq = freq;
    }

//    void setFrequencyLOIF(uint64_t lo, uint64_t to, rf_path_filter path) {
//        errorHandler(hackrf_set_freq_explicit(device, from, to, path));
//        freq = (from - to)/2;
//    }

    void setLNA(uint32_t gain) override {
        gain = gain <= 62 ? gain : 62;
        errorHandler(hackrf_set_lna_gain(device, (gain)/2*2));
    }

    void setVGA(uint32_t gain) override {
        gain = gain <= 47 ? gain : 47;
        errorHandler(hackrf_set_vga_gain(device, gain > 40 ? 40 : (gain+4 /8 * 8)));
        errorHandler(hackrf_set_txvga_gain(device, gain));
    }
    void setAtt() override {
        cout << "HackRF has no att!" << endl;
    }

    void setSampleRate(uint32_t sr) override {
        hackrf_set_sample_rate(device, sr);
    }

    void receive(uint32_t time) override {
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//      need a transfer callback fun
//      hackrf has fixed buffer ~260000
//      every chunk we need to call an irq fun that start in new thread and handle the chunk data??
//      mb this can do hackrf_transfer??
//we need another thread??
//        cout << "prepare to receive" << endl;
        errorHandler(hackrf_start_rx(device, receive_callback, nullptr));
        sleep(time);
//        cout << "We're here!!" << endl;
        hackrf_stop_rx(device);
    }
    void receiveFmWav(uint32_t time) {
        errorHandler(hackrf_start_rx(device, wav_callback, nullptr));
        sleep(time);
        hackrf_stop_rx(device);
    }

    static int wav_callback(hackrf_transfer* transfer) {
        auto bytes_to_write = transfer->valid_length;
        cout << "Opening file for writing wav..." << endl;
        ofstream out("test.wav", ios::binary | ios::out);
        writeWavHeader(out, 44100, INT32_MAX);
        if (!out.is_open()) {
            cerr << "Failed to open file!" << endl;
            throw std::exception();
        }
        cout << "Starting to receive samples..." << endl;
        uint16_t last_phase = 0;
        for(size_t i = 0; i < transfer->valid_length; i+=2) {

            auto sample = complex<uint8_t>(transfer->buffer[i], transfer->buffer[i+1]);
            uint16_t current_phase = arg(sample);
            uint16_t diff = (current_phase - last_phase)/2*M_PI;
            uint16_t audio = diff * 44100;
            out.write(reinterpret_cast<const char *>(&audio), sizeof(audio));
        }
        return 0;
    }


    void send(void *buf, size_t size) override {
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//        errorHandler(hackrf_start_tx(device, send_callback, nullptr));
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
    }

    void stop() const {
        hackrf_stop_rx(device);
    }

    static int receive_callback(hackrf_transfer* transfer) {
        cout << "Opening file for writing..." << endl;
        ofstream out("test.bin", ios::binary | ios::out);

       if (!out.is_open()) {
            cerr << "Failed to open file" << endl;
            throw std::exception();
        }

        cout << "Starting to receive samples..." << endl;

        size_t bytes_to_write = transfer->valid_length;

        out.write(reinterpret_cast<const char*>(transfer->buffer), bytes_to_write);

        cout << "Written " << bytes_to_write << " bytes to file." << endl;
        out.close();
        return 0;
    }


    ~HackRFControl(){
        hackrf_close(device);
    };

    void* allocateBuf(size_t size) override {
        buffer = new uint32_t[buffer_size];
        buffer_size = size;
        return buffer;
    }

    void setIRQManager(void (*foo)(void *), void *buff) override {
        manager = foo;
    }

    void setIRQsize(size_t size) override {
        irq_size = size;
    }

    hackrf_device* device;
    void (*manager)(void *) = nullptr;
    size_t irq_size;
    uint32_t * buffer;
    size_t buffer_size;
    uint32_t freq;
};