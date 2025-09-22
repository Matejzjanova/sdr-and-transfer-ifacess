//
// Created by misha on 17.09.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_TEST_TRANSFEREMULATOR_H
#define RFI_BASE_ON_LIBHACKRF_TEST_TRANSFEREMULATOR_H
#include "sdr_rf_interface/sdr_rf.h"
#include "transfer_interface/transfer.h"

#include "string"
#include <memory>
#include <atomic>
#include <thread>
class SdrEmulator final :  ISDRStreamTransfer, ISdrDevice {
public:
    enum class State : int {
        waiting = 1,
        receiving = 2,
    };
    /**
     * @param id номер эмулятора для отладочной печати
     * @param params параметры "приема" для эмулятора
     */
    explicit SdrEmulator(size_t id = __LINE__,
                         const TransferParams &params = TransferParams());
    SdrEmulator() = delete;
    void *toContext() noexcept override;

    void setHandler(ITransferControl::Handler handler) override;

    void setPacketSize(std::size_t packetCount) override;

    void setPacketCount(std::size_t packetCount) override;

    void setType(TransferParams::Type t) override;

    [[nodiscard]] std::size_t getPacketSize() const override;

    void initialize() override;

    void finalize() override;

    void start() override;

    void startCounter() override;

    void stop() override;

    void setSampleRate(uint64_t samples);

    State getState();

private:
    //получает и сразу вызывает функцию обработчик 1 пакета =>
    //=> эмулятор работает только пакетно, при вызове stop() ждем пока заполнится и обработается 1 пакет
     void receivePackage(uint8_t * package_begin, size_t valid_length);


    TransferParams params_;
    Handler handler_;
    size_t sdr_id_;
    State state_;
    uint64_t sample_rate_ = 1000;
    uint8_t * native_buffer_;

    std::atomic_flag is_rx_ ;
    std::thread * rx_thread_ = nullptr;
    size_t * bytes_read_ = nullptr;
};

#endif //RFI_BASE_ON_LIBHACKRF_TEST_TRANSFEREMULATOR_H
