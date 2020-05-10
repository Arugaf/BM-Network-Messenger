//
// Created by arugaf on 10.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_FRAMETRANSFORMER_H
#define BM_NETWORK_MESSENGER_FRAMETRANSFORMER_H

#include "Frame.h"
#include "HammingEncoder.hpp"
#include "HammingDecoder.hpp"

#include <memory>

namespace BM_Network {
    class FrameTransformer {
        static byte* transform(const std::shared_ptr<IFrame>&, const std::shared_ptr<IEncoder>&);
        static byte* transform(byte*, const std::shared_ptr<IDecoder<byte>>&);
    };
}

#endif //BM_NETWORK_MESSENGER_FRAMETRANSFORMER_H
