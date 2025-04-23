#pragma once

#include <cmath>

template <typename TType>
struct IVector3 {
    TType x;
    TType y;
    TType z;

    IVector3(TType x, TType y, TType z) : x(x), y(y), z(z) {}

    float length();
    IVector3<float> normalize();
    float dot();
    float dot(const IVector3& other);
    IVector3 cross();
    IVector3 cross(const IVector3& other);

    IVector3 operator+(const IVector3& other);
    IVector3 operator-(const IVector3& other);
    IVector3 operator*(const IVector3& other);
    IVector3 operator/(const IVector3& other);
};

template <typename TType>
bool operator!=(const IVector3<TType>& left, const IVector3<TType>& right) {
    return left.x != right.x || left.y != right.y || left.z != right.z;
}

template <typename TType>
bool operator==(const IVector3<TType>& left, const IVector3<TType>& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}


template <typename TType>
float IVector3<TType>::length() {
    return sqrt(x * x + y * y + z * z);
}

template <typename TType>
IVector3<float> IVector3<TType>::normalize() {
    float len = length();
    if (len == 0.0f)
        return IVector3(0.0f, 0.0f, 0.0f);
    return IVector3<float>(x / len, y / len, z / len);
}

template <typename TType>
float IVector3<TType>::dot() {
    return x * x + y * y, z * z;
}

template <typename TType>
float IVector3<TType>::dot(const IVector3& other) {
    return x * other.x + y * other.y + z * other.z;
}

template <typename TType>
IVector3<TType> IVector3<TType>::cross() {
    return IVector3(y * z - z * y, z * x - x * z, x * y - y * x);
}

template <typename TType>
IVector3<TType> IVector3<TType>::cross(const IVector3& other){
    return IVector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

template <typename TType>
IVector3<TType> IVector3<TType>::operator+(const IVector3& other) {
    return IVector3(x + other.x, y + other.y, z + other.z);
}

template <typename TType>
IVector3<TType> IVector3<TType>::operator-(const IVector3& other) {
    return IVector3(x - other.x, y - other.y, z - other.z);
}

template <typename TType>
IVector3<TType> IVector3<TType>::operator*(const IVector3& other) {
    return IVector3(x * other.x, y * other.y, z * other.z);
}

template <typename TType>
IVector3<TType> IVector3<TType>::operator/(const IVector3& other) {
    return IVector3(x / other.x, y / other.y, z / other.z);
}
