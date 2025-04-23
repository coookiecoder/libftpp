#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <stdexcept>

template <typename TState>
class StateMachine {
    public:
        template <typename T, typename U>
        struct states {
            T start;
            U final;

            bool operator==(const states &rhs) const {
                return this->start == rhs.start && this->final == rhs.final;
            }
        };

        template <typename T, typename U>
        struct statesHash {
            std::size_t operator()(const states<T, U>& p) const {
                return std::hash<T>()(p.start);
            }
        };

    private:
        std::unordered_map<TState, std::vector<const std::function<void()> *>> availableState;
        std::unordered_map<states<TState, TState>, std::vector<const std::function<void()> *>, statesHash<TState, TState>> statesTransitions;
        TState *currentState = nullptr;

    public:
        StateMachine() = default;

        void addState(const TState& state);
        void addAction(const TState& state, const std::function<void()>& lambda);
        void addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda);
        void transitionTo(const TState& state);
        void update();
};

template <typename TState>
void StateMachine<TState>::addState(const TState& state) {
    if (!currentState) {
        currentState = new TState(state);
    }
    this->availableState[state];
}

template <typename TState>
void StateMachine<TState>::addAction(const TState& state, const std::function<void()>& lambda) {
    this->availableState[state].push_back(&lambda);
}

template <typename TState>
void StateMachine<TState>::addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda) {
    statesTransitions[{startState, finalState}].push_back(&lambda);
}

template <typename TState>
void StateMachine<TState>::transitionTo(const TState& state) {
    auto iteratorAvailableState = this->availableState.find(state);

    if (iteratorAvailableState == this->availableState.end())
        throw std::invalid_argument("Exception: There is no such state");

    if (*currentState == state) {
        this->update();
        return ;
    }

    auto iteratorStatesTransition = statesTransitions.find({*currentState, state});

    delete currentState;
    currentState = new TState(state);
    if (iteratorStatesTransition == statesTransitions.end()) {
        throw std::invalid_argument("Exception: Handle state not found");
    }

    for (auto func : iteratorStatesTransition->second) {
        (*func)();
    }

}

template <typename TState>
void StateMachine<TState>::update() {
    for (auto func : this->availableState[*currentState]) {
        (*func)();
    }
};