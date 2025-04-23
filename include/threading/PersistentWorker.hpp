#pragma once

#include <functional>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <threading/thread_safe_queue.hpp>

class PersistentWorker {
    private:
        std::mutex guard;
        std::unordered_map<std::string, std::thread> setOfTasks;
        std::unordered_map<std::string, bool> setOfFinish;

    public:
        PersistentWorker() = default;
        PersistentWorker(const PersistentWorker&) = delete;
        PersistentWorker(PersistentWorker&&) = delete;
        PersistentWorker& operator=(const PersistentWorker&) = delete;
        PersistentWorker& operator=(PersistentWorker&&) = delete;

        ~PersistentWorker();

        void addTask(const std::string& name, const std::function<void()>& jobToExecute);
        void removeTask(const std::string& name);
};