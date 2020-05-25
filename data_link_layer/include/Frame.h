//
// Created by arugaf on 09.05.2020.
//

#ifndef BM_NETWORK_MESSENGER_FRAME_H
#define BM_NETWORK_MESSENGER_FRAME_H

#include "include/utils.h"

#include <algorithm>
#include <bitset>
#include <iostream>

namespace BM_Network {
    class Frame {
    public:
        Frame(byte dest_adress,
              byte sender_address,
              byte frame_type,
              byte data_size = 0,
              const byte* data = nullptr,
              byte start_byte = 0xFF,
              byte stop_byte = 0xFF);
        Frame(const byte* frame, size_t frame_size);
        ~Frame();

        [[nodiscard]] const byte* getFrame() const;
        [[nodiscard]] size_t getSize() const;

        [[nodiscard]] byte getDestination() const;
        [[nodiscard]] byte getSender() const;
        [[nodiscard]] byte getType() const;
        [[nodiscard]] std::string getData() const;

        void visualize() const;

    private:
        byte* start_byte;
        byte* dest_address;
        byte* sender_address;
        byte* frame_type;
        byte* data_size;
        byte* data;
        byte* stop_byte;

        byte* memory_area;
        size_t frame_size;
    };

    // TODO: достать информацию до разделителя
}

#endif //BM_NETWORK_MESSENGER_FRAME_H
