#pragma once

#include <new>
#include <utility>

template <class TType>
class DataBuffer {
    private:
        TType* data = nullptr;
        bool*  used = nullptr;
        size_t size = 0;
    public:
        DataBuffer();
        DataBuffer(size_t size);
        DataBuffer(const DataBuffer& other) = delete;
        DataBuffer(DataBuffer&& other) = delete;

        DataBuffer& operator=(const DataBuffer& other) = delete;
        DataBuffer& operator=(DataBuffer&& other) = delete;

        ~DataBuffer();

        void resize(size_t size);

        template <typename ... TArgs>
        TType* get_first(TArgs&&... p_args);
        void deconstruct(TType* data);
};

template <class TType>
DataBuffer<TType>::DataBuffer() {
    this->data = static_cast<TType*>(operator new(sizeof(TType) * 1));
    this->used = new bool[1];
    this->size = 1;
}

template <class TType>
DataBuffer<TType>::DataBuffer(size_t size) {
    this->data = static_cast<TType*>(operator new(sizeof(TType) * size));
    this->used = new bool[size];
    this->size = size;
}

template <class TType>
DataBuffer<TType>::~DataBuffer() {
    operator delete(this->data);
    delete[] this->used;
    this->data = nullptr;
    this->size = 0;
}

template <class TType>
void DataBuffer<TType>::resize(size_t size) {
    operator delete(this->data);
    this->data = static_cast<TType*>(operator new(sizeof(TType) * size));

    delete[] this->used;
    this->used = new bool[size];
    this->size = size;
}

template <class TType>
template <typename ... TArgs>
TType* DataBuffer<TType>::get_first(TArgs&&... p_args) {
    for (int index = 0; index < this->size; ++index) {
        if (this->used[index] == false) {
            this->used[index] = true;
            new (this->data + index) TType(std::forward<TArgs>(p_args)...);
            return this->data + index;
        }
    }

    throw std::bad_alloc();
}

template <class TType>
void DataBuffer<TType>::deconstruct(TType* data) {
    for (int index = 0; index < this->size; ++index) {
        if (this->data + index == data) {
            this->used[index] = false;
            (this->data + index)->~TType();
        }
    }
}
