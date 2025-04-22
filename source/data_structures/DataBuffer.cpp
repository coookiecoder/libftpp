#include <data_structures/DataBuffer.hpp>

DataBuffer::DataBuffer() {
    this->buffer.resize(0);
}

void DataBuffer::clear() {
    this->buffer.clear();
}
