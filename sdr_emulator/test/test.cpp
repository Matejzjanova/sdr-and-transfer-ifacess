//
// Created by misha on 13.09.2025.
//
#include "sdr_rf_interface/sdr_rf.h"
#include "transfer_interface/transfer.h"
#include "gtest/gtest.h"
#include "sdr_emulator.h"
#include <thread>

//Обрабочик, выводящий в stdout количество раз, которое он был вызван
ITransferControl::Handler handler_verbose = [](void* buff, size_t buff_sz) {
    static int handler_called;
    handler_called++;
    printf("We can handle data: %i\n", handler_called);
};

ITransferControl::Handler handler = [](void* buff, size_t buff_sz) {
    static int handler_called;
    handler_called++;
};

class SdrEmulatorTest : public testing::Test {
protected:

    SdrEmulator emulator1{1, TransferParams(0, TransferParams::Type::loop, 1000, 100)};

    SdrEmulator emulator2{2, TransferParams(0, TransferParams::Type::loop, 1000, 100)};

    //объект для тестирования одиночного режима запуска
    SdrEmulator single_emulator{3, TransferParams(0,TransferParams::Type::single, 1000, 100)};

    //объект для тестирования пакетного режима запуска
    SdrEmulator packet_emulator{3, TransferParams(0,TransferParams::Type::single, 1000, 100)};

    SdrEmulatorTest() {

        //все эмуляторы работают на Fd = 1 мегасемпл
        emulator1.setHandler(handler_verbose);
        emulator1.setSampleRate(1e6);

        emulator2.setHandler(handler_verbose);
        emulator2.setSampleRate(1e6);

        single_emulator.setHandler(handler_verbose);
        single_emulator.setSampleRate(1e6);

        packet_emulator.setHandler(handler_verbose);
        packet_emulator.setSampleRate(1e6);
    }

};

TEST(EmulatorTest, EmulatorCanBeCreated) {

    TransferParams params(1, TransferParams::Type::single, 1000, 100);
    SdrEmulator emulator2(1, params);
    ASSERT_EQ(emulator2.getState(), SdrEmulator::State::waiting);
}

TEST_F(SdrEmulatorTest, RxTest) {
    //buffSize = 1000
    //chunkSize = 100
    //Fd = 1e6
    emulator1.initialize();
    emulator1.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    emulator1.stop();
}

TEST_F(SdrEmulatorTest, RxTestUs){

    //Обычный режим работы
    emulator1.initialize();
    emulator1.start();
    std::this_thread::sleep_for(std::chrono::microseconds(50));
    emulator1.stop();
    emulator1.finalize();
}

TEST_F(SdrEmulatorTest, RxTest2){

    //Обычный режим работы
    emulator2.initialize();
    emulator2.start();
    std::this_thread::sleep_for(std::chrono::microseconds(5000));
    emulator2.stop();
    emulator2.finalize();
}

TEST_F(SdrEmulatorTest, RxTestSingle){

    //одиночный режим работы
    single_emulator.initialize();
    single_emulator.start();
    std::this_thread::sleep_for(std::chrono::seconds (5));
    single_emulator.finalize();
}

TEST_F(SdrEmulatorTest, RxTest1Package) {

    //пакетный режим с 1 пакетом
    emulator1.setPacketSize(300);
    emulator1.setPacketCount(3);
    emulator1.initialize();
    emulator1.startCounter();
    std::this_thread::sleep_for(std::chrono::seconds (5));
    emulator1.finalize();
}

TEST_F(SdrEmulatorTest, RxTestPackage) {

    //пакетный режим с размером буфера, не кратным размеру пакета
    emulator1.setSampleRate(1000);
    emulator1.setPacketCount(4);
    emulator1.setPacketSize(300);
    emulator1.initialize();
    emulator1.startCounter();
    std::this_thread::sleep_for(std::chrono::seconds (5));
    emulator1.finalize();
}
TEST_F(SdrEmulatorTest, RxTestManyPackage) {

    //прием большего числа пакетов
    emulator1.setSampleRate(1e6);
    emulator1.setPacketCount(100);
    emulator1.setPacketSize(300);
    emulator1.initialize();
    emulator1.startCounter();
    std::this_thread::sleep_for(std::chrono::seconds (15));
    emulator1.finalize();
}
