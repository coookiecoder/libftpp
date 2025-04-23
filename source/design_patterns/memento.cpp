#include <design_patterns/memento.hpp>

Memento::Snapshot Memento::save() {
    Memento::Snapshot result;
    _saveToSnapshot(result);
    return result;
}

void Memento::load(const Snapshot& state) {
    _loadFromSnapshot(const_cast<Memento::Snapshot&>(state));
}
