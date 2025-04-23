#pragma once

#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <iostream>

#include <threading/ThreadSafeQueue.hpp>

class Jobs {
public:
    virtual void addJob(const std::function<void()>& jobToExecute) = 0;
    virtual ~Jobs() = default;
};

class WorkerPool : public Jobs {
    private:
        void startThread();
        bool stopExecuting;
        std::vector<std::thread> workers;
        ThreadSafeQueue<std::function<void()>> queue;
        std::mutex queueMutex;
        std::mutex guard;
        std::condition_variable condition;

    public:
        WorkerPool() = delete;
        explicit WorkerPool(int n);
        WorkerPool(const WorkerPool&) = delete;
        WorkerPool(WorkerPool&&) = delete;

        WorkerPool& operator=(const WorkerPool&) = delete;
        WorkerPool& operator=(WorkerPool&&) = delete;

        ~WorkerPool();

        void addJob(const std::function<void()>& jobToExecute) override;
};