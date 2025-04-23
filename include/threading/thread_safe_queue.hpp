#pragma once
#include <deque>
#include <thread>
#include <mutex>

template<typename TType>
class ThreadSafeQueue {
    private:
        std::mutex mutex;
        std::deque<TType> queue;
    public:
        void push_back(const TType& newElement);
        void push_front(const TType& newElement);
        TType pop_back();
        TType pop_front();
        bool empty() const;
};

template<typename TType>
void ThreadSafeQueue<TType>::push_back(const TType& newElement) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push_back(newElement);
}

template <typename TType>
void ThreadSafeQueue<TType>::push_front(const TType& newElement) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push_front(newElement);
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_back() {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty())
        throw std::runtime_error("There is no element in queue");

    TType tmp = queue.back();
    queue.pop_back();
    return tmp;
}

template <typename TType>
TType ThreadSafeQueue<TType>::pop_front() {
    std::lock_guard<std::mutex> lock(mutex);
    if (queue.empty())
        throw std::runtime_error("There is no element in queue");

    TType tmp = queue.front();
    queue.pop_front();
    return (tmp);
}

template <typename TType>
bool ThreadSafeQueue<TType>::empty() const {
    return queue.empty();
}
