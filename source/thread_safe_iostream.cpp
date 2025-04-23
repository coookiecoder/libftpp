#include <thread_safe_iostream.hpp>

thread_local ThreadSafeIOStream threadSafeCout;
std::mutex ThreadSafeIOStream::mutex;

void ThreadSafeIOStream::setPrefix(const std::string& prefix) {
    const std::lock_guard<std::mutex> lock(mutex);
    threadLocalPrefix = prefix;
}

ThreadSafeIOStream &ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
    const std::lock_guard<std::mutex> lock(mutex);
    std::ostringstream buffer;

    buffer << threadLocalPrefix << this->buffer.str() << manip;

    std::cout << buffer.str();
    this->buffer.str("");
    this->buffer.clear();
    return (*this);
}