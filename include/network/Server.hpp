#pragma once

#include <network/Message.hpp>
#include <functional>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Server {
public:
    struct ServerComponents {
        int server_fd;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
    };

    class Client {
        private:
            size_t messageSize;
            int socket_fd;
            std::string buffer;
            std::queue<Message *> messageQueue;

        public:
            explicit Client(int clientSocket);

            Client(const Client&) = delete;
            Client(Client&&) = delete;

            Client& operator=(const Client&) = delete;
            Client& operator=(Client&&) = delete;

            ~Client();

        bool receiveMessage(std::string const &buffer);
        int getSocket() const;
        std::queue<Message *> *getMessages();
    };

private:
    std::unordered_map<long long, Client *> clientSockets;
    ServerComponents servComponents = {};
    std::unordered_map<Message::Type, std::function<void(long long&, const Message&)>> actions;

public:
    void start(const size_t& p_port);
    void defineAction(const Message::Type& messageType, const std::function<void(long long& clientID, const Message& msg)>& action);
    void sendTo(const Message& message, long long clientID);
    void sendToArray(const Message& message, std::vector<long long> clientIDs);
    void sendToAll(const Message& message);
    void update();
};