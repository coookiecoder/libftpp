#include <network/Client.hpp>

void Client::connect(const std::string& address, const size_t& port) {
    std::string addressTmp = address;
    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = htons(port);

    if (addressTmp == "localhost") {
        addressTmp = "127.0.0.1";
    }

    if (inet_pton(AF_INET, addressTmp.c_str(), &this->serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported: " << address << std::endl;
        close(this->sock);
        return;
    }

    if (::connect(this->sock, reinterpret_cast<struct sockaddr*>(&this->serv_addr), sizeof(this->serv_addr)) < 0) {
        std::cerr << "Connection to " << address << ":" << port << " failed." << std::endl;
        close(this->sock);
        return;
    }

    std::cout << "Connected to " << address << ":" << port << std::endl;
}

void Client::disconnect() {
    std::cout << "Disconnecting." << std::endl;
}

void Client::defineAction(const Message::Type& messageType, const std::function<void(const Message&)>& action) {
    std::cout << "DefineAction." << messageType << std::endl;
    this->actions[messageType] = action;
}

void Client::send(const Message& message) {
    const std::string serializedMessage = message.serialize();
    const size_t messageSize = serializedMessage.size();
    const std::string sizeStr = std::to_string(messageSize) + "\r\n";

    if (::send(this->sock, sizeStr.c_str(), sizeStr.size(), 0) == -1) {
        std::cerr << "Failed to send message size: " << strerror(errno) << std::endl;
        return;
    }

    if (::send(this->sock, serializedMessage.data(), messageSize, 0) == -1) {
        std::cerr << "Failed to send message: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "message sent" << std::endl;
}

void Client::update() {
    char buffer[1024];
    ssize_t bytesRead = recv(this->sock, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string receivedData(buffer, bytesRead);
        this->receiveMessage(receivedData);
        auto messagesQueue = this->getMessages();
        while (messagesQueue->empty() == false) {
            auto message = messagesQueue->front();
            messagesQueue->pop();

            auto actionIt = actions.find(message->type());
            if (actionIt != actions.end()) {
                actionIt->second(*message);
            } else {
                std::cerr << "No action defined for message type: " << message->type() << std::endl;
            }
        }
    } else if (bytesRead == 0) {
        std::cerr << "Server closed the connection." << std::endl;
        disconnect();
    } else {
        std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
    }
}

bool Client::receiveMessage(std::string const& buffer) {
    this->buffer.append(buffer.c_str(), buffer.size());
    while (true) {
        if (this->messageSize == 0) {
            auto sizeEndIt = this->buffer.find("\r\n");
            if (sizeEndIt != std::string::npos) {
                this->messageSize = std::stoi(this->buffer);
                this->buffer.erase(0, (sizeEndIt + 2));
            }
        }
        if (this->messageSize != 0 && this->buffer.size() >= this->messageSize) {
            std::string message = this->buffer.substr(0, this->messageSize);
            this->buffer.erase(0, this->messageSize);
            std::stringstream ss;
            ss << message;
            this->messageQueue.push(new Message(Message::deserialize(ss)));
            this->messageSize = 0;
            continue;
        }
        break;
    }
    return true;
}

std::queue<Message*>* Client::getMessages() {
    return (&messageQueue);
}

