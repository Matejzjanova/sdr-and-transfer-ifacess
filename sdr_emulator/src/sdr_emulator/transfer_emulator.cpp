//
// Created by misha on 30.06.2025.
//
#include "sdr_emulator.h"

#include <iostream>
#include <thread>
#include <future>

SdrEmulator::SdrEmulator(size_t id, const TransferParams &params) : ISDRStreamTransfer(params) {
    sdr_id_ = id;
    params_ = params;
    state_ = State::waiting;
    is_rx_.clear();
}

void SdrEmulator::initialize() {
    //smth init code
    native_buffer_ = new uint8_t[params_.bufferSize];
    bytes_read_ = new size_t;
}

void SdrEmulator::setHandler(ITransferControl::Handler handler) {
    handler_ = handler;
}

void SdrEmulator::setType(TransferParams::Type t) {
    params_.type = t;
}

void SdrEmulator::setPacketCount(std::size_t packetCount) {
    params_.packageCount = packetCount;
}

std::size_t SdrEmulator::getPacketSize() const {
    return params_.packageSize;
}

void SdrEmulator::receivePackage(uint8_t *package_begin, size_t valid_length) {

    //если доступная длина буфера меньше размера пакета, то обрабатывать надо в два "этапа":
    //сначала пишем в конец буфера и вызываем обработчки конца
    //затем пишем в начала и, соответственно, вызываем обработчик начала
    if (valid_length < params_.packageSize) {
        for (int i = 0; i != valid_length; ++i) {
            package_begin[i] = random();
            usleep(10e5 / sample_rate_);
        }
        handler_(package_begin, valid_length);
        package_begin = native_buffer_;

        size_t remains = params_.packageSize - valid_length;
        for (int i = 0; i != remains; ++i) {
            package_begin[i] = random();
            usleep(10e5 / sample_rate_);
        }
        handler_(package_begin, remains);
    } else {
        for (int i = 0; i != params_.packageSize; i++) {
            package_begin[i] = random();
            usleep(10e5 / sample_rate_);
        }
        handler_(package_begin, params_.packageSize);
    }
}

void SdrEmulator::start() {
    if(params_.bufferSize == 0) { throw std::runtime_error("Buffer size is zero ):");
    }
    if(native_buffer_ == nullptr) {

        throw std::runtime_error("Initialize() emulator first!");
    }
    if(state_ == State::receiving) {
        throw std::runtime_error("Emulator actually emulating");
    }
    if(!sample_rate_) {
        throw std::runtime_error("Set correct sample rate");
    }
    auto startInRxThread = [&] ()  {
        size_t valid_length = params_.bufferSize;
        size_t total_bytes_rx = 0;
        if(params_.type == TransferParams::Type::loop) {
                is_rx_.test_and_set();
                while (is_rx_.test()) {
                    receivePackage(native_buffer_, valid_length);
                    total_bytes_rx += params_.packageSize;
                    if(valid_length < params_.packageSize) {
                        valid_length = params_.bufferSize - valid_length;
                    }
                    else {
                        valid_length -= params_.packageSize;
                    }
                }
        }
        else if (params_.type == TransferParams::Type::single) {
            receivePackage(native_buffer_, valid_length);
            if (bytes_read_) {
                printf("Bytes read: %lu\n", *bytes_read_);
            }
        }
        *bytes_read_ = total_bytes_rx;
    };
    state_ = State::receiving;
    rx_thread_ = new std::thread(startInRxThread);
}



void SdrEmulator::startCounter() {

    if(params_.bufferSize == 0) {
        throw std::runtime_error("Buffer size is zero ):");
    }
    if(native_buffer_ == nullptr) {
        throw std::runtime_error("Initialize() emulator first!");
    }
    if(state_ == State::receiving) {
        throw std::runtime_error("Emulator actually emulating");
    }
    if(!sample_rate_) {
        throw std::runtime_error("Set correct sample rate");
    }
    if (!params_.packageSize) {
        throw std::runtime_error("Wrong packet count");
    }

    auto startCounterInOtherThread = [&] () {
        is_rx_.test_and_set();
        size_t valid_length = params_.bufferSize;
        for (int i = 0; i !=  params_.packageCount && is_rx_.test(); i++) {

            receivePackage(native_buffer_ + i*params_.packageSize, valid_length);

            //eсли доступная длина буфера меньше размера пакета, то "переносимся в начало" буфера
            if(valid_length < params_.packageSize) {
                valid_length = params_.bufferSize - valid_length;
            }
            //иначе просто уменьшаем доступную длину на размер полученного пакета
            else {
                valid_length -= params_.packageSize;
            }
        }
    };
    state_ = SdrEmulator::State::receiving;
    rx_thread_ = new std::thread(startCounterInOtherThread);
}

void SdrEmulator::stop() {

    if(state_ != State::receiving) {
        throw std::runtime_error("Emulator not receiving");
    }

    is_rx_.clear();
    rx_thread_->join();
    state_ = State::waiting;

    if (bytes_read_) {
        printf("Bytes read: %lu", *bytes_read_);
    }
}

void SdrEmulator::finalize() {
    if(state_ == State::receiving) {
        stop();
    }
    delete rx_thread_;
    delete [] native_buffer_;
}

 SdrEmulator::State SdrEmulator::getState() {
    return state_;
}

void * SdrEmulator::toContext() noexcept {

}

void SdrEmulator::setSampleRate(uint64_t samples) {
    sample_rate_ = samples;
}

void SdrEmulator::setPacketSize(std::size_t packetSize) {
    params_.packageSize = packetSize;
}

