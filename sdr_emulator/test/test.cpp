//
// Created by misha on 13.09.2025.
//
#include "sdr_emulator.h"
#include "sdr_rf_interface/sdr_rf.h"
#include "transfer_interface/transfer.h"
#include "gtest/gtest.h"
#include <thread>

// Обрабочик, выводящий в stdout количество раз, которое он был вызван
ITransferControl::Handler handler_verbose = [](void *buff, size_t buff_sz) {
  static int handler_called;
  handler_called++;
  printf("We can handle data: %i\n", handler_called);
};

ITransferControl::Handler handler = [](void *buff, size_t buff_sz) {
  static int handler_called;
  handler_called++;
};

class SdrEmulatorTest : public testing::Test {
protected:
  SdrEmulator emulator1{1};

  SdrEmulator emulator2{2};

  // объект для тестирования одиночного режима запуска
  SdrEmulator single_emulator{3};

  // объект для тестирования пакетного режима запуска
  SdrEmulator packet_emulator{4};

  SdrEmulatorTest() {

    // все эмуляторы работают на Fd = 1 мегасемпл
    emulator1.setType(TransferParams::Type::loop);
    emulator1.setHandler(handler_verbose);
    emulator1.setSampleRate(1e3);

    emulator2.setHandler(handler_verbose);
    emulator2.setSampleRate(1e3);

    single_emulator.setHandler(handler_verbose);
    single_emulator.setSampleRate(1e3);

    packet_emulator.setHandler(handler_verbose);
    packet_emulator.setSampleRate(1e3);
  }
};

TEST(EmulatorTest, EmulatorCanBeCreated) {

  TransferParams params(1, TransferParams::Type::single, 1000, 100);
  SdrEmulator emulator2(1);
  emulator2.setParams(params);
  ASSERT_EQ(emulator2.getState(), SdrEmulator::State::waiting);
}

TEST_F(SdrEmulatorTest, RxTest) {
  // buffSize = 1000
  // chunkSize = 100
  // Fd = 1e6
  TransferParams params(1, TransferParams::Type::loop, 1000, 100);
  emulator1.setParams(params);
  emulator1.initialize();
  emulator1.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  emulator1.stop();
  emulator1.finalize();
}

TEST_F(SdrEmulatorTest, RxTestUs) {

  // Обычный режим работы

  TransferParams params(1, TransferParams::Type::loop, 1000, 100);
  emulator1.setParams(params);
  emulator1.initialize();
  EXPECT_NO_THROW(emulator1.start());
  ASSERT_EQ(emulator1.getState(), SdrEmulator::State::receiving);
  std::this_thread::sleep_for(std::chrono::microseconds(50));
  emulator1.stop();
  emulator1.finalize();
}

TEST_F(SdrEmulatorTest, RxTest2) {

  TransferParams params(1, TransferParams::Type::loop, 1000, 100);
  emulator2.setParams(params);
  // Обычный режим работы
  emulator2.initialize();
  emulator2.start();
  std::this_thread::sleep_for(std::chrono::microseconds(5000));
  emulator2.stop();
  emulator2.finalize();
}

TEST_F(SdrEmulatorTest, RxTestSingle) {

  // одиночный режим работы

  TransferParams params(1, TransferParams::Type::single, 1000, 100);
  EXPECT_NO_THROW(single_emulator.setParams(params));
  single_emulator.initialize();
  single_emulator.setSampleRate(1000);
  auto start = std::chrono::steady_clock::now();
  EXPECT_NO_THROW(single_emulator.start());
  single_emulator.stop();
  auto stop = std::chrono::steady_clock::now();
  ASSERT_EQ(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start),
            std::chrono::milliseconds(100));
  single_emulator.finalize();
}

TEST_F(SdrEmulatorTest, EmulatorIn2DiffModes) {
  TransferParams params(1, TransferParams::Type::loop, 1000, 100);
  emulator2.setParams(params);
  emulator1.setSampleRate(1e3);
  emulator1.initialize();
  emulator1.start();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  emulator1.stop();

  emulator1.setSampleRate(1e3);
  emulator1.setPacketSize(350);
  emulator1.setType(TransferParams::Type::single);
  emulator1.start();
  std::this_thread::sleep_for(std::chrono::seconds(3));
  emulator1.finalize();
}

TEST_F(SdrEmulatorTest, EmulatorIn3DiffModes) {

  TransferParams params(1, TransferParams::Type::loop, 1000, 100);
  emulator1.setParams(params);
  emulator1.setSampleRate(1e6);
  emulator1.initialize();
  emulator1.start();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  emulator1.stop();

  emulator1.setSampleRate(1e4);
  emulator1.setPacketSize(350);
  emulator1.setType(TransferParams::Type::single);
  emulator1.start();
  std::this_thread::sleep_for(std::chrono::seconds(3));

  emulator1.setType(TransferParams::Type::loop);
  emulator1.start();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  emulator1.stop();
}
