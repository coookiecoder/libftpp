#pragma once

#include <sstream>

#include <data_structures/DataBuffer.hpp>

class Message {
    private:
        std::stringstream buffer;
        int typeBuffer = 0;

    public:
        using Type = int;

        Message() = default;
        explicit Message(int type);
        Message(const Message& message) = delete;
        Message(Message&& message) noexcept;

        Message& operator=(const Message& message) = delete;
        Message& operator=(Message&& message) noexcept;

        template <typename T>
        Message& operator<<(const T& obj);

        template <typename T>
        const Message& operator>>(T& obj) const;

        int type() const;

        std::string serialize() const;
        static Message deserialize(std::stringstream &ss);

        ~Message() = default;
};

template <typename T>
Message& Message::operator<<(const T& obj) {
    buffer << obj;
    return *this;
}

template <typename T>
const Message& Message::operator>>(T& obj) const{
    const_cast<std::stringstream&>(buffer) >> obj;
    return *this;
}
