//
// Created by arugaf on 09.05.2020.
//

#include "Frame.h"

BM_Network::Frame::Frame(BM_Network::byte dest_adress, BM_Network::byte sender_address, BM_Network::byte frame_type,
                         BM_Network::byte data_size, const BM_Network::byte* data, BM_Network::byte start_byte,
                         BM_Network::byte stop_byte) : memory_area(data_size ? new byte[data_size + 6] : new byte[5]),
                                                       frame_size(data_size ? data_size + 6 : 5) {
    this->start_byte = memory_area;
    this->dest_address = memory_area + 1;
    this->sender_address = memory_area + 2;
    this->frame_type = memory_area + 3;
    data_size ? this->data_size = memory_area + 4 : this->data_size = nullptr;
    data_size ? this->data = memory_area + 5 : this->data = nullptr;
    data_size ? this->stop_byte = memory_area + 5 + data_size : this->stop_byte = memory_area + 4;

    *this->start_byte = start_byte;
    *this->dest_address = dest_adress;
    *this->sender_address = sender_address;
    *this->frame_type = frame_type;
    data_size ? *this->data_size = data_size : data_size;
    if (data_size) {
        std::copy(data, data + data_size, this->data);
    }
    *this->stop_byte = stop_byte;
}

BM_Network::Frame::Frame(const BM_Network::byte* frame, size_t frame_size) : memory_area(new byte[frame_size]), frame_size(frame_size) {
    std::copy(frame, frame + frame_size, memory_area);

    this->start_byte = memory_area;
    this->dest_address = memory_area + 1;
    this->sender_address = memory_area + 2;
    this->frame_type = memory_area + 3;

    frame_size > 5 ? this->data_size = memory_area + 4 : this->data_size = nullptr;
    frame_size > 5 ? this->data = memory_area + 5 : this->data = nullptr;
    this->stop_byte = memory_area + frame_size - 1;
}

const BM_Network::byte* BM_Network::Frame::getFrame() const {
    return memory_area;
}

size_t BM_Network::Frame::getSize() const {
    return frame_size;
}

BM_Network::Frame::~Frame() {
    delete[] memory_area;
}

void BM_Network::Frame::visualize() const {
    for (size_t i = 0; i < frame_size; ++i) {
        std::cout << static_cast<short>(memory_area[i]) << ' ' << std::bitset<8>(memory_area[i]) << std::endl;
    }
}

BM_Network::byte BM_Network::Frame::getDestination() const {
    return *dest_address;
}

BM_Network::byte BM_Network::Frame::getSender() const {
    return *sender_address;
}

BM_Network::byte BM_Network::Frame::getType() const {
    return *frame_type;
}

std::string BM_Network::Frame::getData() const {
    if (data_size) {
        std::cout << 'l';
    }
    return data_size ? std::string(data, *data_size) : std::string();
}
