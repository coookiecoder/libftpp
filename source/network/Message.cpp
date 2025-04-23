#include <network/Message.hpp>

Message::Message(int type) : typeBuffer(type) {

}

Message::Message(Message&& message)  noexcept {
    this->typeBuffer = message.typeBuffer;
    this->buffer = std::move(message.buffer);
}

Message& Message::operator=(Message&& message)  noexcept {
    this->typeBuffer = message.typeBuffer;
    this->buffer = std::move(message.buffer);
    return *this;
}

int Message::type() const {
    return typeBuffer;
}

std::string Message::serialize() const {
    std::ostringstream oss;

    oss << typeBuffer << " ";
    oss << buffer.rdbuf();
    return oss.str();
}

Message Message::deserialize(std::stringstream &ss) {
    Type type;
    ss >> type;

    Message message(type);
    std::string str;
    message.buffer << ss.rdbuf();
    return message;
}