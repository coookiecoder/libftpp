#include <network/Server.hpp>
#include <network/Message.hpp>
#include <iostream>
#include <unordered_map>
#include <data_structures/DataBuffer.hpp>

Server::Client::Client(int clientSocket) : messageSize(0), socket_fd(clientSocket) {

}

Server::Client::~Client() {
    if (socket_fd != -1)
        close(socket_fd);
}

bool Server::Client::receiveMessage(std::string const& buffer)       {
    this->buffer.append(buffer.c_str(), buffer.size());
    while (true) {
        if (messageSize == 0) {
            auto sizeEndIt = this->buffer.find("\r\n");
            if (sizeEndIt != std::string::npos) {
                messageSize = std::stoi(this->  buffer);
                this->buffer.erase(0, (sizeEndIt + 2));
            }
        }
        if (messageSize != 0 && this->buffer.size() >= messageSize) {
            std::string message = this->buffer.substr(0, messageSize);
            this->buffer.erase(0, messageSize);
            std::stringstream ss;
            ss << message;
            messageQueue.push(new Message(Message::deserialize(ss)));
            this->messageSize = 0;
            continue;
        }
        break;
    }
    return true;
}

int Server::Client::getSocket() const {
    return socket_fd;
}

std::queue<Message*>* Server::Client::getMessages() {
        return &messageQueue;
}

void logError(const std::string& message) {
    std::cerr << message << ": " << strerror(errno) << std::endl;
}

void Server::start(const size_t &p_port) {

    if ((servComponents.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    if (setsockopt(servComponents.server_fd, SOL_SOCKET,
            SO_REUSEADDR, &servComponents.opt, sizeof(servComponents.opt))) {
        logError("Failed to set socket options.");
        close(servComponents.server_fd);
        return;
    }

    servComponents.address.sin_family = AF_INET;
    servComponents.address.sin_addr.s_addr = INADDR_ANY; // Use localhost
    servComponents.address.sin_port = htons(p_port);

    if (bind(servComponents.server_fd, (struct sockaddr*)&servComponents.address, sizeof(servComponents.address)) < 0) {
        std::cerr << "Failed to bind socket to port " << p_port << "." << std::endl;
        close(servComponents.server_fd);
        return;
    }

    if (listen(servComponents.server_fd, 3) < 0) {
        std::cerr << "Failed to listen on port " << p_port << "." << std::endl;
        close(servComponents.server_fd);
        return;
    }

    std::cout << "Server started on localhost:" << p_port << std::endl;
}

void Server::defineAction(const Message::Type &messageType, const std::function<void(long long& clientID, const Message& msg)>& action) {
    actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID) {
    auto it = clientSockets.find(clientID);
    if (it == clientSockets.end()) {
        std::cerr << "Client ID " << clientID << " not connected." << std::endl;
        return;
    }

    int socket_fd = it->second->getSocket();
    std::string serializedMessage = message.serialize();
    std::string messageSize = std::to_string(serializedMessage.size()) + "\r\n";

    if (send(socket_fd, messageSize.c_str(), messageSize.size(), 0) == -1) {
        logError("Failed to send messageSize to client " + std::to_string(clientID));
    }

    if (send(socket_fd, serializedMessage.c_str(), serializedMessage.size(), 0) == -1) {
        logError("Failed to send message to client " + std::to_string(clientID));
    }
}

void Server::sendToArray(const Message& message, std::vector<long long> clientIDs) {
    for (const auto& clientID : clientIDs) {
        sendTo(message, clientID);
    }
}

void Server::sendToAll(const Message& message) {
    std::cout << "Sending message of type " << message.type()
              << " to all connected clients" << std::endl;

    std::string serializedMessage = message.serialize();

    for (const auto& [clientID, client] : clientSockets) {
        if (send(client->getSocket(), serializedMessage.c_str(), serializedMessage.size(), 0) == -1) {
            logError("Failed to send message to client " + std::to_string(clientID));
        }
    }
}

void Server::update() {
    fd_set readfds;
    FD_ZERO(&readfds);

    int max_fd = servComponents.server_fd;
    FD_SET(servComponents.server_fd, &readfds);

    for (const auto& [clientID, client] : clientSockets) {
        int socket_fd = client->getSocket();
        FD_SET(socket_fd, &readfds);
        if (socket_fd > max_fd) {
            max_fd = socket_fd;
        }
    }
    fd_set tmpReadfds = readfds;
    // std::cout << "select start" << std::endl;
    int activity = select(max_fd + 1, &tmpReadfds, nullptr, nullptr, nullptr);
    // std::cout << "select end" << std::endl;

    if (activity < 0 && errno != EINTR) {
        logError("Select error");
        return;
    }

    if (FD_ISSET(servComponents.server_fd, &tmpReadfds)) {
        int new_socket = accept(servComponents.server_fd,
                                (struct sockaddr*)&servComponents.address,
                                (socklen_t*)&servComponents.addrlen);
        if (new_socket < 0) {
            logError("Failed to accept new connection");
            return;
        }

        long long clientID = new_socket;
        clientSockets[clientID] = new Client(new_socket);

        std::cout << "New client connected with ID: " << clientID << std::endl;
    }

    for (auto it = clientSockets.begin(); it != clientSockets.end(); ) {
        long long clientID = it->first;
        int socket_fd = it->second->getSocket();

        if (FD_ISSET(socket_fd, &readfds)) {
            char buffer[1024] = {0};
            ssize_t bytesReceived = recv(socket_fd, buffer, sizeof(buffer), 0);

            if (bytesReceived <= 0) {
                if (bytesReceived == 0) {
                    std::cout << "Client " << clientID << " disconnected." << std::endl;
                } else {
                    logError("Recv error for client " + std::to_string(clientID));
                }

                close(socket_fd);
                delete it->second;
                it = clientSockets.erase(it);
                continue;
            }
            it->second->receiveMessage(buffer);

            auto messagesQueue = it->second->getMessages();

            while (messagesQueue->empty() == false) {
                auto message = messagesQueue->front();
                messagesQueue->pop();
                auto actionIt = actions.find(message->type());
                if (actionIt != actions.end()) {
                    actionIt->second(clientID, *message);
                } else {
                    std::cerr << "No action defined for message type: " << message->type() << std::endl;
                }
            }
        }
        ++it;
    }
}