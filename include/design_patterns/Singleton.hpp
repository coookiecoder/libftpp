#pragma once
#include <iostream>

template <typename TType>
class Singleton {
    private:
        static TType *singleton;
    public:
        Singleton() = delete;
        Singleton(const Singleton &) = delete;
        Singleton(Singleton &&) = delete;

        Singleton &operator=(const Singleton &) = delete;
        Singleton &operator=(Singleton &&) = delete;

        ~Singleton() = delete;

        static TType* instance();

        template<typename ... TArgs>
        static void instantiate(TArgs&& ...p_args);
};

template <typename TType>
TType* Singleton<TType>::singleton = nullptr;

template <typename TType>
TType* Singleton<TType>::instance() {
    if (singleton == nullptr)
        throw std::runtime_error("Exception: Instance not yet created");

    return singleton;
}

template <typename TType>
template <typename... TArgs>
void Singleton<TType>::instantiate(TArgs&&... p_args) {
    if (singleton)
        throw std::runtime_error("Exception: Instance already created");

    singleton = new TType(std::forward<TArgs>(p_args)...);
}
