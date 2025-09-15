//
// Created by misha on 12.05.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H
#define RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H

#include "functional"
#include "ITransferParams.h"
class ITransferControl {
public:
    using Handler = std::function<void(void* ptr, std::size_t sz)>;
    ITransferControl(const TransferParams& params) {};
    virtual ~ITransferControl() = default;
    /**
	 * @brief setHandler
	 * @param h обработчик прерывания, вызываемый при получении прерывания от источника (например, драйвера)
	 */
    virtual void setHandler(Handler handler) = 0;

    /**
     * @brief setPacketCount
     * @param packetCount количество пакетов в одном прерывании
     */
    virtual void setPacketCount(std::size_t packetCount) = 0;

    /**
     * @brief getPacketSize
     * @return
     */
    virtual std::size_t getPacketSize() const = 0;

    /**
     * @brief initalize
     */
    virtual void initialize() = 0;

    /**
     * @brief finalize
     */
    virtual void finalize() = 0;

    /**
     * @brief setType
     * @param t
     */
    virtual void setType(TransferParams::Type t) = 0;
};
#endif //RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H
