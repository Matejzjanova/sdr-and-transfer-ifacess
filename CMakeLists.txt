cmake_minimum_required(VERSION 3.29)
project(RFI_base_on_libhackrf)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(hackrf)
include_directories(hackrf/firmware/common)
include_directories(hackrf/firmware/common/rad1o)
include_directories(hackrf/firmware/common/xapp058)
include_directories(hackrf/firmware/hackrf_usb)
include_directories(hackrf/hardware)
include_directories(hackrf/host)
include_directories(hackrf/host/hackrf-tools/getopt)
include_directories(hackrf/host/libhackrf)
include_directories(hackrf/host/libhackrf/src)
find_path(LIBUSB_INCLUDE_DIR
        NAMES libusb.h
        PATH_SUFFIXES "include" "libusb" "libusb-1.0")
find_library(LIBUSB_LIBRARY
        NAMES usb
        PATH_SUFFIXES "lib" "lib32" "lib64")
add_executable(RFI_base_on_libhackrf
        HackRFControl.cpp
        IRFControl.h)
