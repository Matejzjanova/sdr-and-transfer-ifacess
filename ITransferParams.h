//
// Created by misha on 23.06.2025.
//
#include "cstdlib"
#include "tuple"
#ifndef RFI_BASE_ON_LIBHACKRF_ITRANSFERPARAMS_H
#define RFI_BASE_ON_LIBHACKRF_ITRANSFERPARAMS_H
struct TransferParams {
    enum class Type { single, loop };

    TransferParams() = default;
    TransferParams(size_t id, Type type, size_t buffSize) : id(id), type(type), bufferSize(buffSize)
    {}
    size_t id;
    Type type;
    std::size_t bufferSize;
    std::function<void(void* ptr, std::size_t sz)> handler;
};

inline bool operator == (const TransferParams& left,const TransferParams& right){
    return std::tie(left.bufferSize,left.id,left.type) == std::tie(right.bufferSize,right.id,right.type);
};
#endif //RFI_BASE_ON_LIBHACKRF_ITRANSFERPARAMS_H
