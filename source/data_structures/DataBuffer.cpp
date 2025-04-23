#include <data_structures/DataBuffer.hpp>

DataBuffer::DataBuffer(DataBuffer&& other)  noexcept {
    this->buffer = std::move(other.buffer);
}

DataBuffer& DataBuffer::operator=(DataBuffer&& other) noexcept {
    this->buffer = std::move(other.buffer);
    return *this;
}


DataBuffer::DataBuffer() {
    this->buffer.resize(0);
}

void DataBuffer::clear() {
    this->buffer.clear();
}
