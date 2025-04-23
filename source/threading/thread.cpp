#include <threading/thread.hpp>
#include <thread_safe_iostream.hpp>
#include <utility>

Thread::Thread(std::string  name, std::function<void()> functToExecute): _name(std::move(name)),  _functToExecute(std::move(functToExecute)) {

};

void Thread::start() {
    _thread = std::thread(&Thread::_start, _name, _functToExecute);
};

void Thread::_start(const std::string& name, const std::function<void()> &functToExecute) {
    threadSafeCout.setPrefix(name + " ");
    functToExecute();
};

void Thread::stop() {
    _thread.join();
};