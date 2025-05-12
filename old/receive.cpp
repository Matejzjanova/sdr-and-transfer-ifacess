//
// Created by misha on 19.04.2025.
//
#include "HackRFControlPast.cpp"
int main() {
    auto * device = new HackRFControl();
//    cout << "Дошли" << endl;
    device->setVGA(16);
    device->setLNA(16);
    device->setFrequency(92e6);
    device->setSampleRate(22050);
    cout << "Try to receive" << endl;
    device->receiveFmWav(3);
    device->stop();
}