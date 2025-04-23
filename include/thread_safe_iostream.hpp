#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

class ThreadSafeIOStream {
    private:
        std::ostringstream buffer;
        std::string threadLocalPrefix;
        static std::mutex mutex;

    public:
        void setPrefix(const std::string& prefix);

        template <typename T>
        ThreadSafeIOStream &operator<<(const T &obj);

        ThreadSafeIOStream &operator<<(std::ostream& (*manip)(std::ostream&));

        template <typename T>
        ThreadSafeIOStream &operator>>(T &obj);

        template <typename T>
        void prompt(const std::string& question, T& dest);
};

template <typename T>
ThreadSafeIOStream &ThreadSafeIOStream::operator<<(const T &obj) {
    buffer << obj;
    return (*this);
}

template <typename T>
ThreadSafeIOStream &ThreadSafeIOStream::operator>>(T &obj) {
    const std::lock_guard<std::mutex> lock(mutex);
    std::cout << threadLocalPrefix;

    std::cin >> obj;
    return (*this);
}

template<typename T>
void ThreadSafeIOStream::prompt(const std::string& question, T& dest) {
    const std::lock_guard<std::mutex> lock(mutex);
    std::cout << threadLocalPrefix;

    std::cout << question;
    std::cin >> dest;
};

extern thread_local ThreadSafeIOStream threadSafeCout;