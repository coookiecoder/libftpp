#pragma once

#include <cstddef>

#include "PoolBuffer.hpp"

#define DEFAULT_SIZE 1

template <class TType>
class Pool {
    private:
        PoolBuffer<TType> data;
    public:
        Pool();
        Pool(size_t size);
        Pool(const Pool<TType>& other) = delete;
        Pool(Pool<TType>&& other) = delete;

        Pool& operator=(const Pool<TType>& other) = delete;
        Pool& operator=(Pool<TType>&& other) = delete;

        ~Pool();

        class Object {
            private:
                TType *data;
                PoolBuffer<TType> *dataBuffer;
            public:
                Object() = delete;
                Object(TType* data, PoolBuffer<TType> *dataBuffer);
                Object(const Object& other) = delete;
                Object(Object&& other) = delete;

                Object& operator=(const Object& other) = delete;
                Object& operator=(Object&& other) = delete;

                ~Object();

                TType* operator->();
        };

        void resize(const size_t& numberOfObjectStored);

        template<typename ... TArgs>
        Object acquire(TArgs&&... p_args);
};

template <class TType>
Pool<TType>::Pool() : data(DEFAULT_SIZE) {}

template <class TType>
Pool<TType>::Pool(size_t size) : data(size) {}

template <class TType>
Pool<TType>::~Pool() = default;

template <class TType>
Pool<TType>::Object::Object(TType* data, PoolBuffer<TType> *dataBuffer) {
    this->data = data;
    this->dataBuffer = dataBuffer;
}

template <class TType>
Pool<TType>::Object::~Object() {
    this->dataBuffer->deconstruct(data);
}


template <class TType>
TType* Pool<TType>::Object::operator->() {
    return this->data;
}

template <class TType>
void Pool<TType>::resize(const size_t& numberOfObjectStored) {
    this->data.resize(numberOfObjectStored);
}

template <class TType>
template <typename ... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args) {
    return Object(this->data.get_first(std::forward<TArgs>(p_args)...), &this->data);
}
