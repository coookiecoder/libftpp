#pragma once

#include <new>
#include <utility>

template <class TType>
class PoolBuffer {
    private:
        TType* data = nullptr;
        bool*  used = nullptr;
        size_t size = 0;
    public:
        PoolBuffer();
        PoolBuffer(size_t size);
        PoolBuffer(const PoolBuffer& other) = delete;
        PoolBuffer(PoolBuffer&& other) = delete;

        PoolBuffer& operator=(const PoolBuffer& other) = delete;
        PoolBuffer& operator=(PoolBuffer&& other) = delete;

        ~PoolBuffer();

        void resize(size_t size);

        template <typename ... TArgs>
        TType* get_first(TArgs&&... p_args);
        void deconstruct(TType* data);
};

template <class TType>
PoolBuffer<TType>::PoolBuffer() {
    this->size = size;

    this->data = static_cast<TType*>(operator new(sizeof(TType) * 1));
    this->used = new bool[1];

    for (int index = 0; index < this->size; ++index)
        this->used[index] = false;
}

template <class TType>
PoolBuffer<TType>::PoolBuffer(size_t size) {
    this->size = size;

    this->data = static_cast<TType*>(operator new(sizeof(TType) * size));
    this->used = new bool[size];

    for (int index = 0; index < this->size; ++index)
        this->used[index] = false;
}

template <class TType>
PoolBuffer<TType>::~PoolBuffer() {
    operator delete(this->data);
    delete[] this->used;
    this->data = nullptr;
    this->size = 0;
}

template <class TType>
void PoolBuffer<TType>::resize(size_t size) {
    this->size = size;

    operator delete(this->data);
    this->data = static_cast<TType*>(operator new(sizeof(TType) * size));

    delete[] this->used;
    this->used = new bool[size];

    for (int index = 0; index < this->size; ++index)
        this->used[index] = false;
}

template <class TType>
template <typename ... TArgs>
TType* PoolBuffer<TType>::get_first(TArgs&&... p_args) {
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
void PoolBuffer<TType>::deconstruct(TType* data) {
    for (int index = 0; index < this->size; ++index) {
        if (this->data + index == data) {
            this->used[index] = false;
            (this->data + index)->~TType();
        }
    }
}
