#pragma once

#include <data_structures/DataBuffer.hpp>

class Memento {
    public:
        using Snapshot = DataBuffer;

        Memento() = default;
        virtual ~Memento() = default;

        Snapshot save();
        void load(const Snapshot& state);

        virtual void _saveToSnapshot(Snapshot& Snapshot) const = 0;
        virtual void _loadFromSnapshot(Snapshot& Snapshot) = 0;
};