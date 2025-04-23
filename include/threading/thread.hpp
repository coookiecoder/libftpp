#pragma once
#include <string>
#include <thread>
#include <functional>

class Thread {
    private:
        std::string _name;
        std::function<void()> _functToExecute;
        std::thread _thread;

        static void _start(const std::string& name, const std::function<void()> &functToExecute);
    public:
        Thread(std::string  name, std::function<void()> functToExecute);
        void start();
        void stop();
};