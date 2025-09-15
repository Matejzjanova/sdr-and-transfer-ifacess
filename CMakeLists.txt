cmake_minimum_required(VERSION 3.29)
project(RFI_base_on_libhackrf_test LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)


find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

add_executable(tests test.cpp main.cpp)
target_link_libraries(tests PUBLIC sdrTransfer)
target_compile_features(tests  PRIVATE cxx_std_17)
# add_subdirectory(doctest)
target_include_directories(tests PRIVATE doctest/doctest)
add_library(sdrTransfer STATIC ITransferParams.h
        ITransferControl.h
        ISDRStreamTransfer.h
        SDREmulator.h
        TransferEmulator.h
        TransferEmulator.cpp
        TransferEmulator.h
        SDREmulator.cpp TransferEmulator.cpp)

# add_library(libhackrf STATIC
#         hackrf/host/libhackrf/src/hackrf.c
#         old/HackRFControl.h
#         old/HackRFControl.cpp
# )
# target_include_directories(libhackrf PUBLIC
#         hackrf/host/libhackrf/src
#         ${LIBUSB_INCLUDE_DIRS}
# )
# target_link_libraries(libhackrf PRIVATE
#         Threads::Threads
#         ${LIBUSB_LINK_LIBRARIES}
# )
# add_executable(RFI_base_on_libhackrf
#          ITransferParams.h ITransferControl.h ISDRStreamTransfer.h  SDREmulator.h SDREmulator.cpp TransferEmulator.h TransferEmulator.cpp
# )
# target_link_libraries(RFI_base_on_libhackrf PRIVATE
# )


