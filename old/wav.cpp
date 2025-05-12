//
// Created by misha on 21.04.2025.

#include "wav.h"
void writeWavHeader(std::ofstream& file, uint32_t sampleRate, uint32_t dataSize) {
    struct WavHeader {
        char     riff[4] = {'R', 'I', 'F', 'F'};
        uint32_t fileSize;
        char     wave[4] = {'W', 'A', 'V', 'E'};

        // fmt subchunk
        char     fmt[4] = {'f', 'm', 't', ' '};
        uint32_t fmtSize = 16;
        uint16_t audioFormat = 1;
        uint16_t numChannels = 1;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample = 16;

        // data subchunk
        char     data[4] = {'d', 'a', 't', 'a'};
        uint32_t dataSize;
    } header;

    header.fileSize = dataSize + sizeof(WavHeader) - 8;
    header.sampleRate = sampleRate;
    header.byteRate = sampleRate * header.numChannels * header.bitsPerSample / 8;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    header.dataSize = dataSize;

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
}