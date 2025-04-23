#include <mathematics/Random2DCoordinateGenerator.hpp>

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long seed) : seedBuffer(seed) {}

long long Random2DCoordinateGenerator::seed() const {
    return seedBuffer;
}

long long Random2DCoordinateGenerator::hash(long long x, long long y) const {
    // A simple hash combining the x, y coordinates with the seed
    long long result = seedBuffer;
    result ^= (x + 0x9e3779b9 + (result << 6) + (result >> 2)); // XOR with x and apply some bit shifts
    result ^= (y + 0x9e3779b9 + (result << 6) + (result >> 2)); // XOR with y and apply some bit shifts
    return result;
}

long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y) const {
    return hash(x, y);
}
