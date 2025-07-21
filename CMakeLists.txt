cmake_minimum_required(VERSION 3.29)
project(RFI_base_on_libhackrf LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)


find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)


add_library(libhackrf STATIC
        hackrf/host/libhackrf/src/hackrf.c
        old/HackRFControl.h
        old/HackRFControl.cpp
)
target_include_directories(libhackrf PUBLIC
        hackrf/host/libhackrf/src
        ${LIBUSB_INCLUDE_DIRS}
)
target_link_libraries(libhackrf PRIVATE
        Threads::Threads
        ${LIBUSB_LINK_LIBRARIES}
)
add_executable(RFI_base_on_libhackrf
         ITransferParams.h ITransferControl.h ISDRStreamTransfer.h  SDREmulator.h SDREmulator.cpp TransferEmulator.h TransferEmulator.cpp  main.cpp
)
target_link_libraries(RFI_base_on_libhackrf PRIVATE
)


