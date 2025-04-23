#include <threading/WorkerPool.hpp>

WorkerPool::WorkerPool(int n) : stopExecuting(false) {
    workers.reserve(n);
    for (size_t i = 0; i < static_cast<size_t>(n); ++i) {
        workers.emplace_back(&WorkerPool::startThread, this);
    }
}

WorkerPool::~WorkerPool() {
    do {
        std::lock_guard<std::mutex> lock(guard);
        stopExecuting = true;
    } while (false);

    condition.notify_all();
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void WorkerPool::addJob(const std::function<void()>& jobToExecute) {
    queue.push_back(jobToExecute);
    condition.notify_one();
};

void WorkerPool::startThread() {
    while (true) {
        std::function<void()> job;
        std::unique_lock<std::mutex> lock(queueMutex);

        condition.wait(lock, [&]() { return (stopExecuting || queue.empty() == false);}); {
            std::lock_guard<std::mutex> lock(guard);
            if (stopExecuting == true) {
                return ;
            }
        }
        job = queue.pop_front();
        try {
            job();
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};