#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

class DataBuffer {
    private:
        std::vector<unsigned char> buffer;
    public:
        DataBuffer();
        DataBuffer(const DataBuffer& other) = delete;
        DataBuffer(DataBuffer&& other) noexcept;

        DataBuffer& operator=(const DataBuffer& other) = delete;
        DataBuffer& operator=(DataBuffer&& other) noexcept;

        template <class T>
        DataBuffer& operator<<(const T& value);

        template <class T>
        DataBuffer& operator>>(T& value);

        ~DataBuffer() = default;

        void clear();
};

template <class T>
DataBuffer& DataBuffer::operator<<(const T& value) {
    // static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");

    buffer.resize(buffer.size() + sizeof(T));
    std::memcpy(buffer.data() + buffer.size() - sizeof(T), &value, sizeof(T));
    return *this;
}

template <class T>
DataBuffer& DataBuffer::operator>>(T& value) {
    //static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");

    if (buffer.size() < sizeof(T)) {
        throw std::runtime_error("DataBuffer::operator>>: buffer too small");
    }

    std::memcpy(&value, buffer.data(), sizeof(T));
    buffer.erase(buffer.begin(), buffer.begin() + sizeof(T));
    return *this;
}
