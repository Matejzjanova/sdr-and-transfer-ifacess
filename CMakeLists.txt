cmake_minimum_required(VERSION 3.29)
project(RFI_base_on_libhackrf_test LANGUAGES C CXX)

add_subdirectory(transfer_interface)
add_subdirectory(sdr_emulator)
add_subdirectory(sdr_rf_interface)
