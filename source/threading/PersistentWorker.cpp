#include <threading/PersistentWorker.hpp>

PersistentWorker::~PersistentWorker() {
    for(auto &item : setOfFinish) {
        item.second = true;
    }

    for(auto &item : setOfTasks) {
        if (item.second.joinable())
            item.second.join();
    }
}

void PersistentWorker::addTask(const std::string& name, const
    std::function<void()>& jobToExecute) {
    setOfTasks.insert({name, std::thread([&]() {
            while (true) {
                bool is_finished;

                do {
                    std::lock_guard<std::mutex> lock(guard);
                    is_finished = setOfFinish[name];
                } while (false);

                if (is_finished)
                    return ;

                jobToExecute();
            }
        })});
    {
        std::lock_guard<std::mutex> lock(guard);
        setOfFinish.insert({name, false});
    }
}

void PersistentWorker::removeTask(const std::string& name) {
    do {
        std::lock_guard<std::mutex> lock(guard);
        setOfFinish[name] = true;
    } while (false);

    auto &task = setOfTasks[name];

    if (task.joinable())
        task.join();

    setOfTasks.erase(name);
}