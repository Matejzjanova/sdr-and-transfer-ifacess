//
// Created by misha on 13.09.2025.
//
#include "TransferEmulator.h"
#include "doctest/doctest/doctest.h"

TEST_CASE("SDREmulator object can be created"){
    SUBCASE("Constructor"){
    TransferEmulator em = TransferEmulator(TransferParams());
    }
}

TEST_CASE("SDREmulator can emulate TX") {
    size_t sz = 10e6;
    uint8_t * buff = new uint8_t[sz];
    TransferEmulator em = TransferEmulator(TransferParams(1,
                                                          TransferParams::Type::loop,
                                                          sz));
    SUBCASE("SDR not init") {

        CHECK_THROWS_AS(em.stop(), std::exception);
    }
    SUBCASE("SDR can init") {
        em.initialize();
    }
    SUBCASE("SDR can give a state") {
        em.initialize();
        em.getState();
    }
}

void cb( void* buff, size_t sz) {
    //uint8_t * buff_8 = static_cast<uint8_t *>(buff);
    std::cout << "Receiving" << '\n';
}
TEST_CASE("Emulator can tx"){
    TransferParams params(1,TransferParams::Type::loop, 10e6  );
    auto em = TransferEmulator(params);
    em.initialize();
    em.setHandler(cb);
    em.start();
}