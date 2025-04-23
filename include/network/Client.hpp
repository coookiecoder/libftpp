#pragma once

#include <functional>
#include <queue>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

#include <network/Message.hpp>

class Client {
    private:
        int sock = 0;
        struct sockaddr_in serv_addr = {};
        std::unordered_map<Message::Type, std::function<void(const Message&)>> actions;
        std::string buffer;
        size_t messageSize = 0;
        std::queue<Message *> messageQueue;

    public:
        Client() = default;
        Client(const Client&) = delete;
        Client(Client&&) = delete;

        Client& operator=(const Client&) = delete;
        Client& operator=(Client&&) = delete;

        ~Client() = default;

        void connect(const std::string& address, const size_t& port);
        void disconnect();
        void defineAction(const Message::Type& messageType, const std::function<void(const Message&)>& action);
        void send(const Message& message);
        void update();

    private:
        bool receiveMessage(std::string const &buffer);
        std::queue<Message *> *getMessages();
};