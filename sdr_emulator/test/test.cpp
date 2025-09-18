//
// Created by misha on 13.09.2025.
//
#include "sdr_rf_interface/sdr_rf.h"
#include "transfer_interface/transfer.h"
#include "gtest/gtest.h"
#include "sdr_emulator.h"
#include <thread>

ITransferControl::Handler handler = [](void* buff, size_t buff_sz) {
    static int handler_called;
    handler_called++;
    printf("We can handle data: %i\n", handler_called);
};

class SdrEmulatorTest : public testing::Test {
protected:
    SdrEmulator emulator1{1, TransferParams(0, TransferParams::Type::loop, 1000, 100 )};


};

TEST(EmulatorTest, EmulatorCanBeCreated) {
    TransferParams params(1, TransferParams::Type::single, 1000, 100);
    SdrEmulator emulator2(1, params);
    ASSERT_EQ(emulator2.getState(), SdrEmulator::State::waiting);

}

TEST_F(SdrEmulatorTest, RxTest) {
    //buffSize == 1000
    //chunkSize == 100
    emulator1.initialize();
    emulator1.setHandler(handler);
    emulator1.setSampleRate(10000);
    emulator1.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    emulator1.stop();
}
