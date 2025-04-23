#pragma once

#include <cmath>

template <typename TType>
struct IVector2 {
    TType x;
    TType y;

    IVector2(TType x, TType y) : x(x), y(y) {}

    float length();
    IVector2<float> normalize();
    float dot();
    float dot(const IVector2& other);
    IVector2 cross();
    IVector2 cross(const IVector2& other);

    IVector2 operator+(const IVector2& other);
    IVector2 operator-(const IVector2& other);
    IVector2 operator*(const IVector2& other);
    IVector2 operator/(const IVector2& other);
};

template <typename TType>
bool operator!=(const IVector2<TType>& left, const IVector2<TType>& right) {
    return left.x != right.x || left.y != right.y;
}

template <typename TType>
bool operator==(const IVector2<TType>& left, const IVector2<TType>& right) {
    return left.x == right.x && left.y == right.y;
}

template <typename TType>
float IVector2<TType>::length() {
    return sqrt(x * x + y * y);
}

template <typename TType>
IVector2<float> IVector2<TType>::normalize() {
    float len = length();
    if (len == 0.0f)
        return IVector2<float>(0.0f, 0.0f);
    return IVector2<float>(x / len, y / len);
}

template <typename TType>
float IVector2<TType>::dot() {
    return x * x + y * y;
}

template <typename TType>
float IVector2<TType>::dot(const IVector2& other) {
    return x * other.x + y * other.y;
}

template <typename TType>
IVector2<TType> IVector2<TType>::cross() {
    return IVector2(0, x * y - y * x);
}

template <typename TType>
IVector2<TType> IVector2<TType>::cross(const IVector2& other){
    return IVector2(x * other.y - y * other.x);
}

template <typename TType>
IVector2<TType> IVector2<TType>::operator+(const IVector2& other) {
    return IVector2(x + other.x, y + other.y);
}

template <typename TType>
IVector2<TType> IVector2<TType>::operator-(const IVector2& other) {
    return IVector2(x - other.x, y - other.y);
}

template <typename TType>
IVector2<TType> IVector2<TType>::operator*(const IVector2& other) {
    return IVector2(x * other.x, y * other.y);
}

template <typename TType>
IVector2<TType> IVector2<TType>::operator/(const IVector2& other) {
    return IVector2(x / other.x, y / other.y);
}
